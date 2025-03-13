#ifndef TRACK_GENERATOR_H
#define TRACK_GENERATOR_H

#include "path_config.h"
#include "path_generator.h"
#include <complex.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Simple 3D vector (we only use x and z for our track, but include y for completeness) */
typedef struct {
    double x;
    double y;
    double z;
} Vector3;

/* A transform holds a position and a rotation (in radians) */
typedef struct {
    Vector3 position;
    double rotation;
} Transform;

/* 
 * TrackResult holds the generated cone and checkpoint transforms.
 * The caller is responsible for freeing each dynamically allocated array.
 */
typedef struct {
    int nLeftCones;
    Transform* leftCones;
    int nRightCones;
    Transform* rightCones;
    int nCheckpoints;
    Transform* checkpoints;
} TrackResult;

/* 
 * Generates the track data (left cones, right cones, and checkpoints) based on the provided
 * configuration and path result. This function uses a spacing algorithm similar in spirit to the
 * original C# PlaceCones routine.
 */
TrackResult generate_track(const PathConfig *config, const PathResult *path);

#ifdef __cplusplus
}
#endif

#endif // TRACK_GENERATOR_H
