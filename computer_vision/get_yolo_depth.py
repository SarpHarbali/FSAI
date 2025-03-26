import pyzed.sl as sl
import torch
import cv2
import numpy as np
import sys
import pyzed.sl as sl
import cv2
import argparse
import os
import numpy as np
import math
from models.experimental import attempt_load
from utils.general import non_max_suppression


def detect_cones(image, model, original_size):
    """ Run YOLOv7 to detect cones and return bounding boxes. """
    img = cv2.resize(image, (640, 640))  # Resize for YOLOv7
    img = img.transpose(2, 0, 1)  # Change channel order
    img = np.expand_dims(img, axis=0)  # Add batch dimension
    img = torch.from_numpy(img).float() / 255.0  # Normalize
    img = img.to(torch.device("cuda" if torch.cuda.is_available() else "cpu"))

    with torch.no_grad():
        predictions = model(img)[0]  # Run inference
        detections = non_max_suppression(predictions, conf_thres=0.50, iou_thres=0.45)[0]  # Extract bounding boxes




    orig_h, orig_w = original_size  # Original ZED image size
    scale_w = orig_w / 640.0
    scale_h = orig_h / 640.0

    for det in detections:
        det[0] *= scale_w  # Scale x1
        det[1] *= scale_h  # Scale y1
        det[2] *= scale_w  # Scale x2
        det[3] *= scale_h  # Scale y2

    return detections



def get_3d_position(u, v, depth_map):
    """ Convert (u, v) image position to (X, Y, Z) world position using depth. """
    depth_value = depth_map.get_value(u, v)[1]  # Get depth at pixel
    if np.isnan(depth_value) or depth_value == 0:
        return None  # Ignore invalid depths

    fx, fy, cx, cy = 527.0759887695312, 527.0759887695312, 629.3443603515625, 370.0404968261719  # Replace with actual ZED intrinsics
    X = (u - cx) * depth_value / fx
    Y = (v - cy) * depth_value / fy
    Z = depth_value
    return np.array([X, Y, Z])

def get_depth(u, v, depth_map, image):
    """ Get depth (Z) information at pixel (u, v) by averaging over a region. """
    
    depth_h, depth_w = depth_map.get_height(), depth_map.get_width()
    image_h, image_w = image.get_height(), image.get_width()

    scale_x = depth_w / image_w
    scale_y = depth_h / image_h

    u = int(u * scale_x)
    v = int(v * scale_y)

    depth_values = []

    for i in range(-2, 3):
        for j in range(-2, 3):
            err, d = depth_map.get_value(u + i, v + j)
            if err == sl.ERROR_CODE.SUCCESS and d > 0:  # Only take valid depth values
                depth_values.append(d)

    if len(depth_values) == 0:
        return None  # No valid depth found

    depth_avg = np.mean(depth_values)  # Compute average depth
    return depth_avg

def transform_to_global(P_camera, R, t):
    """ Convert a local 3D position to global coordinates using car pose. """
    return np.dot(R, P_camera) + t


def main():
    model = attempt_load("yolov7.pt", map_location=torch.device("cuda" if torch.cuda.is_available() else "cpu"))
    model.eval()
    
    init_params = sl.InitParameters(camera_resolution=sl.RESOLUTION.HD720,
                                 coordinate_units=sl.UNIT.METER,
                                 coordinate_system=sl.COORDINATE_SYSTEM.RIGHT_HANDED_Y_UP)
    
   
    zed = sl.Camera()
    status = zed.open(init_params)
    if status != sl.ERROR_CODE.SUCCESS:
        print("Camera Open", status, "Exit program.")
        exit(1)

    tracking_params = sl.PositionalTrackingParameters() #set parameters for Positional Tracking
    tracking_params.enable_imu_fusion = True
    tracking_params.mode = sl.POSITIONAL_TRACKING_MODE.GEN_1
    status = zed.enable_positional_tracking(tracking_params) #enable Positional Tracking
    if status != sl.ERROR_CODE.SUCCESS:
        print("[Sample] Enable Positional Tracking : "+repr(status)+". Exit program.")
        zed.close()
        exit()

    runtime = sl.RuntimeParameters()
    camera_pose = sl.Pose()

    cone_map = []  # Stores global cone positions

    tracking_params = sl.PositionalTrackingParameters()
    zed.enable_positional_tracking(tracking_params)

    image = sl.Mat()
    depth_map = sl.Mat()

    camera_info = zed.get_camera_information()
    intrinsics = camera_info.camera_configuration.calibration_parameters.left_cam
    print("___________________________________________________")
    print(f"Focal Length (fx, fy): {intrinsics.fx}, {intrinsics.fy}")
    print(f"Principal Point (cx, cy): {intrinsics.cx}, {intrinsics.cy}")

    while True:

        if zed.grab() == sl.ERROR_CODE.SUCCESS:
            zed.retrieve_image(image, sl.VIEW.LEFT)  # Get RGB image
            zed.retrieve_measure(depth_map, sl.MEASURE.DEPTH)  # Get depth map
            
            tracking_state = zed.get_position(camera_pose,sl.REFERENCE_FRAME.WORLD)
            tracking_status = zed.get_positional_tracking_status()
            if tracking_state != sl.POSITIONAL_TRACKING_STATE.OK:
                print("SLAM tracking lost")
                continue  # Skip if SLAM is lost

            

            translation = camera_pose.get_translation().get()
            rotation = camera_pose.get_rotation_matrix().r  # Rotation matrix


            image_np = image.get_data()
            image_np = cv2.cvtColor(image_np, cv2.COLOR_BGRA2BGR)  # Convert BGRA to BGR
            image_np = np.ascontiguousarray(image_np, dtype=np.uint8)   # Convert to numpy
            original_size = (image.get_height(), image.get_width())  # Get ZED image size
            detections = detect_cones(image_np, model, original_size)

            for detection in detections:
                detection = detection.cpu().numpy()  # Convert to NumPy array
                x1, y1, x2, y2, conf, cls = detection[:6]  # Extract coordinates, confidence, and class
                u, v = int((x1 + x2) / 2), int((y1 + y2) / 2)  # Bounding box center

                depth = get_depth(u, v, depth_map, image)
                depth_text = f"Depth: {depth:.2f}m" if depth is not None else "Depth: N/A"

                local_3d_pos = get_3d_position(u, v, depth_map)
                

                # Draw bounding box
                cv2.rectangle(image_np, (int(x1), int(y1)), (int(x2), int(y2)), (0, 255, 0), 2)

                u, v = (x1 + x2) // 2, (y1 + y2) // 2

                cv2.rectangle(image_np, (int(u-2), int(v-2)), (int(u+3), int(v+3)), (0, 255, 0), 2)

                # Construct label with class name and confidence
                class_name = model.names[int(cls)] if hasattr(model, 'names') else 'Unknown'
                label = f"{class_name} {conf:.2f}"
                
                if local_3d_pos is not None:
                    global_pos = transform_to_global(local_3d_pos, rotation, translation)
                    cone_map.append([global_pos[0], global_pos[1], global_pos[2], class_name])
                
                # Display the label above the bounding box
                cv2.putText(image_np, label, (int(x1), int(y1) - 30),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)
                
                # Overlay depth value below the label
                cv2.putText(image_np, depth_text, (int(x1), int(y1) - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 255), 2)


            cv2.imshow("Cone Detection", image_np)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
    with open ("cone_map.txt", "w") as f:
        for pos in cone_map:
            f.write(f"{pos[0]:.2f}, {pos[1]:.2f}, {pos[2]:.2f}, {pos[3]}\n")

if __name__ == "__main__":
    main()