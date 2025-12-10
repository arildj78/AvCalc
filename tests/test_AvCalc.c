#include "unity.h"
#include "../AvCalc.h"
#include <math.h>

void setUp(void) {
    // Run before each test
}

void tearDown(void) {
    // Run after each test
}

void test_Distance_LAX_to_JFK(void) {
    double lat1 = 33.95;
    double lon1 = -118.4;
    double lat2 = 40.633333;
    double lon2 = -73.783333;
    
    double dist = Distance(&lat1, &lon1, &lat2, &lon2);
    
    TEST_ASSERT_FLOAT_WITHIN(5.0, 2144.0, dist);
}

void test_CourseInitial_LAX_to_JFK(void) {
    double lat1 = 33.95;
    double lon1 = -118.4;
    double lat2 = 40.633333;
    double lon2 = -73.783333;
    
    double course = CourseInitial(&lat1, &lon1, &lat2, &lon2);
    
    TEST_ASSERT_FLOAT_WITHIN(0.5, 66.0, course);
}

void test_IntermediatePoint(void) {
    double lat1 = 33.95;
    double lon1 = -118.4;
    double lat2 = 40.633333;
    double lon2 = -73.783333;
    double fraction = 0.4;
    double lat_result, lon_result;
    
    IntermediatePoint(&lat1, &lon1, &lat2, &lon2, &fraction, &lat_result, &lon_result);
    
    TEST_ASSERT_FLOAT_WITHIN(0.02, 38.66945, lat_result);
    TEST_ASSERT_FLOAT_WITHIN(0.05, -101.62617, lon_result);
}

void test_Standard_temperature(void) {
    // Standard atmosphere temperatures at various altitudes
    // Format: altitude in feet, expected temperature in Celsius
    
    struct {
        double feet;
        double expected_temp;
        const char *description;        
    } test_cases[] = {
        {-16404.19,  47.500,   "-5 km  (below sea level)"},
        {     0.00,  15.000,   "0 km  (sea level)"},
        { 36089.24, -56.500,   "11 km  (lower tropopause)"},
        { 65616.80, -56.500,   "20 km  (upper tropopause)"},
        {104986.88, -44.500,   "32 km  (middle stratosphere)"},
        {154199.48,  -2.500,   "47 km  (lower stratopause)"},
        {167322.83,  -2.500,   "51 km  (upper stratopause)"},
        {232939.63, -58.500,   "71 km  (middle mesosphere)"},
        {262467.19, -76.500,   "80 km  (lower mesopause)"},
         
        { -3000.0,  20.944,   "-3 000 feet  (troposhpere)"},   
        {  5000.0,   5.094,   "5 000 feet  (troposhpere)"},   
        { 25000.0, -34.530,   "25 000 feet  (troposhpere)"},   
        { 50000.0, -56.500,   "50 000 feet  (tropopause)"},  
        { 70000.0, -55.164,   "70 000 feet  (lower stratoshpere)"},   
        {125000.0, -27.420,   "125 000 feet  (upper stratoshpere)"},   
        {160000.0,  -2.500,   "160 000 feet  (stratopause)"},   
        {200000.0, -30.388,   "200 000 feet  (lower mesosphere)"},
        {240000.0, -62.804,   "240 000 feet  (upper mesosphere)"},

        {-16500.0,     NAN,   "-16 500 feet  (below valid range)"},
        {262500.0,     NAN,   "262 500 feet  (above valid range)"}
    };
    
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    char message[100];

    for (int i = 0; i < num_cases; i++) {
        double temp = Standard_temperature(&test_cases[i].feet);
        sprintf(message, "Case %d: %s", i, test_cases[i].description);
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(
            0.001,                          // Test for value within 0.001 of expected value
            test_cases[i].expected_temp, 
            temp,
            message
        );
    }
}

void test_Speed_of_sound(void) {
    double oat = -6.72;
    double cs = Speed_of_sound(&oat);
    
    TEST_ASSERT_FLOAT_WITHIN(0.5, 636.08, cs);
}

void test_Density_at_sea_level(void) {
    double h = 0.0;
    double oat = 15.0;
    double rho = Density_at_altitude(&h, &oat);
    
    TEST_ASSERT_FLOAT_WITHIN(1e-6, rho_0, rho);
}

void test_Pressure_at_sea_level(void) {
    double h = 0.0;
    double p = Pressure_at_altitude(&h);
    
    TEST_ASSERT_FLOAT_WITHIN(0.1, P_0, p);
}

void test_Pressure_at_tropopause(void) {
    double h = 36089.24;
    double p = Pressure_at_altitude(&h);
    
    TEST_ASSERT_FLOAT_WITHIN(1.0, 22632.06, p);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_Distance_LAX_to_JFK);
    RUN_TEST(test_CourseInitial_LAX_to_JFK);
    RUN_TEST(test_IntermediatePoint);

    RUN_TEST(test_Standard_temperature);
    RUN_TEST(test_Speed_of_sound);
    RUN_TEST(test_Density_at_sea_level);
    RUN_TEST(test_Pressure_at_sea_level);
    RUN_TEST(test_Pressure_at_tropopause);
    
    return UNITY_END();
}