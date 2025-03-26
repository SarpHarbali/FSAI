#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../src/steering.c"

#define EPSILON 0.001 // Precision tolerance

void test_signed_right_angle() {
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){0, 1}, (Vector2){1, 0}), -90, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){1, 0}, (Vector2){0, 1}), 90, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){1, 1}, (Vector2){-1, 1}), 90, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-3.2, 2.2}, (Vector2){2.2, 3.2}), -90, EPSILON);
}

void test_signed_zero_angle() {
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){2.3, 7.1}, (Vector2){2.3, 7.1}), 0, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-4, 5.5}, (Vector2){-4, 5.5}), 0, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){1, -1}, (Vector2){1, -1}), 0, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-3.2, -2.2}, (Vector2){-3.2, -2.2}), 0, EPSILON);
}

void test_straight_angle() {
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){2.3, 7.1}, (Vector2){-2.3, -7.1}), 180, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-4, 5.5}, (Vector2){4, -5.5}), 180, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){1, -1}, (Vector2){-1, 1}), 180, EPSILON);
    printf("\n%f\n", signedAngle((Vector2){-3.2, -2.2}, (Vector2){3.2, 2.2}));
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-3.2, -2.2}, (Vector2){3.2, 2.2}), 180, EPSILON);
}

void test_signed_angle() {
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-0.556544f, 0.8308181f}, (Vector2){0.0957871f, 0.9954019f}), -39.31376f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-0.9654807f, 0.2604742f}, (Vector2){0.2496527f, 0.9683355f}), -89.35876f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-0.5360287f, 0.8441997f}, (Vector2){-0.7013699f, -0.7127974f}), 103.0493f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-0.1346436f, -0.9908941f}, (Vector2){0.7187564f, 0.695262f}), 141.7861f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-0.05180694f, 0.9986571f}, (Vector2){0.956363f, -0.2921814f}), -109.9582f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-0.7075015f, 0.7067118f}, (Vector2){-0.09367919f, -0.9956024f}), 129.5927f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){0.7431677f, 0.6691052f}, (Vector2){0.6163496f, 0.7874727f}), 9.951907f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-0.9950259f, -0.0996162f}, (Vector2){0.7792555f, 0.6267064f}), -146.9095f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){-0.6584399f, -0.7526333f}, (Vector2){0.4203782f, 0.907349f}), -163.6775f, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(signedAngle((Vector2){0.4808996f, 0.8767757f}, (Vector2){0.9998393f, 0.01792597f}), -60.22869f, EPSILON);
}

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // Create a test suite
    CU_pSuite suite = CU_add_suite("SignedAngleSuite", NULL, NULL);
    if (!suite)
        return CU_get_error();

    if (!CU_add_test(suite, "Right Angles", test_signed_right_angle) ||
        !CU_add_test(suite, "Zero Angles", test_signed_zero_angle) ||
        !CU_add_test(suite, "Straight Angles", test_straight_angle) ||
        !CU_add_test(suite, "Random Angles", test_signed_angle)) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    // Run tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Cleanup
    CU_cleanup_registry();
    return 0;
}
