#include "unity.h"
#include "../AvCalc.h"
#include <math.h>

void setUp(void) {
    // Run before each test
}

void tearDown(void) {
    // Run after each test
}

void test_Distance(void) {
    // Test various known distances between latitude/longitude points
    // Expected distances have been pre-calculated using Ed Williams 
    // Great Circle Calculator (http://edwilliams.org/gccalc.htm)
    // This test assumes a spherical Earth, 1' of arc = 1 NM and 1 NM = 1852 meters.

    // Distances to be tested (NM) on a spherical Earth
    const double zeroToSeventy       = 4996.937192149175;  // From 00°N 000°E to 70°N 070°E
    const double seventyToSeventy    = 9993.874384298348;  // From 70°N 070°E to 70°S 070°W
    const double eightyToFourty      = 8400.0;             // From 80°N 000°E to 40°S 180°E
    const double ninetyToEightyNine  = 60.0;               // From 90°N 000°E to 89°N 000°E
    const double poleToPole          = 10800.0;            // From 90°N 000°E to 90°S 000°E


    const double expectedAccuracy  = 0.000001;           // Expected accuracy in NM (within 1.852 mm)

    typedef struct {
        double lat;
        double lon;
    } LatLon;
    struct {
        LatLon from;
        LatLon to;
        double expected_dist;
        const char *description;        
    } test_cases[] = {
        //Test same point (zero distance)
        {{0.0, 0.0}, {  0.0,   0.0},    0.00000, "Start and end at 00°N 000°E"},
        
        //0- long
        //Test from 00°N 000°E to various points as well as opposite
        {{0.0, 0.0}, { 70.0,  70.0}, zeroToSeventy, "Start at 00°N 000°E going north east"},
        {{0.0, 0.0}, {-70.0,  70.0}, zeroToSeventy, "Start at 00°N 000°E going south east"},
        {{0.0, 0.0}, {-70.0, -70.0}, zeroToSeventy, "Start at 00°N 000°E going south west"},
        {{0.0, 0.0}, { 70.0, -70.0}, zeroToSeventy, "Start at 00°N 000°E going north west"},

        {{ 70.0,  70.0}, {0.0, 0.0}, zeroToSeventy, "Start at north east going to 00°N 000°E"},
        {{-70.0,  70.0}, {0.0, 0.0}, zeroToSeventy, "Start at south east going to 00°N 000°E"},
        {{-70.0, -70.0}, {0.0, 0.0}, zeroToSeventy, "Start at south west going to 00°N 000°E"},
        {{ 70.0, -70.0}, {0.0, 0.0}, zeroToSeventy, "Start at north west going to 00°N 000°E"},

        //Test between various quadrants through 00°N 000°E
        {{-70.0, -70.0}, { 70.0,  70.0}, seventyToSeventy, "Start at south west going north east through 00°N 000°E"},
        {{ 70.0, -70.0}, {-70.0,  70.0}, seventyToSeventy, "Start at north west going south east through 00°N 000°E"},
        {{ 70.0,  70.0}, {-70.0, -70.0}, seventyToSeventy, "Start at north east going south west through 00°N 000°E"},
        {{-70.0,  70.0}, { 70.0, -70.0}, seventyToSeventy, "Start at south east going north west through 00°N 000°E"},


        //180- long
        //Test from 00°N 180°E to various points as well as opposite
        {{0.0, 180.0}, { 70.0, -110.0}, zeroToSeventy, "Start at 00°N 180°E going north east"},
        {{0.0, 180.0}, {-70.0, -110.0}, zeroToSeventy, "Start at 00°N 180°E going south east"},
        {{0.0, 180.0}, {-70.0,  110.0}, zeroToSeventy, "Start at 00°N 180°E going south west"},
        {{0.0, 180.0}, { 70.0,  110.0}, zeroToSeventy, "Start at 00°N 180°E going north west"},

        {{0.0, -180.0}, { 70.0, -110.0}, zeroToSeventy, "Start at 00°N 180°W going north east"},
        {{0.0, -180.0}, {-70.0, -110.0}, zeroToSeventy, "Start at 00°N 180°W going south east"},
        {{0.0, -180.0}, {-70.0,  110.0}, zeroToSeventy, "Start at 00°N 180°W going south west"},
        {{0.0, -180.0}, { 70.0,  110.0}, zeroToSeventy, "Start at 00°N 180°W going north west"},

        {{ 70.0, -110.0}, {0.0, 180.0}, zeroToSeventy, "Start at north east going to 00°N 180°E"},
        {{-70.0, -110.0}, {0.0, 180.0}, zeroToSeventy, "Start at south east going to 00°N 180°E"},
        {{-70.0,  110.0}, {0.0, 180.0}, zeroToSeventy, "Start at south west going to 00°N 180°E"},
        {{ 70.0,  110.0}, {0.0, 180.0}, zeroToSeventy, "Start at north west going to 00°N 180°E"},

        //Test between various quadrants through 00°N 180°E
        {{-70.0,  110.0}, { 70.0, -110.0}, seventyToSeventy, "Start at south west going north east through 00°N 180°E"},
        {{ 70.0,  110.0}, {-70.0, -110.0}, seventyToSeventy, "Start at north west going south east through 00°N 180°E"},
        {{ 70.0, -110.0}, {-70.0,  110.0}, seventyToSeventy, "Start at north east going south west through 00°N 180°E"},
        {{-70.0, -110.0}, { 70.0,  110.0}, seventyToSeventy, "Start at south east going north west through 00°N 180°E"},

        //Test over poles
        {{ 80.0, 0.0}, {-40.0, 180.0}, eightyToFourty, "Over north pole"},
        {{-80.0, 0.0}, { 40.0, 180.0}, eightyToFourty, "Over south pole"},

        //To and from poles
        {{ 90.0,   0.0}, { 89.0, -180.0}, ninetyToEightyNine, "From north pole"},
        {{ 89.0, 180.0}, { 90.0, -180.0}, ninetyToEightyNine, "To north pole"},
        {{-90.0,   0.0}, {-89.0,  180.0}, ninetyToEightyNine, "From south pole"},
        {{-89.0,-180.0}, {-90.0,  180.0}, ninetyToEightyNine, "To south pole"},

        //Between poles
        {{ 90.0,    0.0}, {-90.0, 0.0}, poleToPole, "From north pole to south pole"},
        {{-90.0, -180.0}, { 90.0, 0.0}, poleToPole, "From south pole to north pole"},

        //Between antipodal points
        {{ -40.402194, 176.311146}, {40.402194, -3.688854}, 10800.0, "From Weber (New Zealand) to Madrid (Spain)"},      // (10801.2245335 NM in WGS84)
       
        //Test length of ENBO runway
        {{67 + 15.0/60 + 59.76/3600, 14 + 20.0/60 +  2.71/3600},
         {67 + 16.0/60 + 18.78/3600, 14 + 23.0/60 + 50.65/3600}, 2781.289 / 1852, "Length of ENBO runway"}, //ENBO rwy is 2781.289 meters long (2794.125 meters in WGS84)

        //Test width of ENBO runway
        {{67.268974906, 14.364956043},
         {67.269363413, 14.364747837}, 44.087 / 1852, "Width of ENBO runway"}, //ENBO rwy is 44.087 meters wide (44.250 meters in WGS84)

        //Test width of jet bridge 16 at ENBO airport
        {{67.271781408, 14.367604724},
         {67.271782569, 14.367654544}, 2.142 / 1852, "Width of jet bridge 16 at ENBO airport"}, //Jet bridge 16 at ENBO airport is 2142 mm wide (2153 mm in WGS84)

        //Test very small distance at ENBO airport
        {{67.269559640, 14.369525520},
         {67.269559610, 14.369525560}, 0.003750/ 1852, "Very small distance"} //Positions are 3.750 mm apart (3.764 mm WGS84, 3.803 in ArcGIS) 
    };
    
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    char message[100];

    for (int i = 0; i < num_cases; i++) {
        double dist = Distance(&test_cases[i].from.lat, &test_cases[i].from.lon,
                               &test_cases[i].to.lat, &test_cases[i].to.lon);
        sprintf(message, "Case %d: %s", i, test_cases[i].description);
        TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
            expectedAccuracy,                 // Test for value within expected accuracy of expected value
            test_cases[i].expected_dist, 
            dist,
            message
        );
    }
    
    double lat1 = 33.95;
    double lon1 = -118.4;
    double lat2 = 40.633333;
    double lon2 = -73.783333;
    
    double dist = Distance(&lat1, &lon1, &lat2, &lon2);
    
    TEST_ASSERT_DOUBLE_WITHIN(5.0, 2144.0, dist);
}

void test_CourseInitial_LAX_to_JFK(void) {
    double lat1 = 33.95;
    double lon1 = -118.4;
    double lat2 = 40.633333;
    double lon2 = -73.783333;
    
    double course = CourseInitial(&lat1, &lon1, &lat2, &lon2);
    
    TEST_ASSERT_DOUBLE_WITHIN(0.5, 66.0, course);
}

void test_IntermediatePoint(void) {
    double lat1 = 33.95;
    double lon1 = -118.4;
    double lat2 = 40.633333;
    double lon2 = -73.783333;
    double fraction = 0.4;
    double lat_result, lon_result;
    
    IntermediatePoint(&lat1, &lon1, &lat2, &lon2, &fraction, &lat_result, &lon_result);
    
    TEST_ASSERT_DOUBLE_WITHIN(0.02, 38.66945, lat_result);
    TEST_ASSERT_DOUBLE_WITHIN(0.05, -101.62617, lon_result);
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
        TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(
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
    
    TEST_ASSERT_DOUBLE_WITHIN(0.5, 636.08, cs);
}

void test_Density_at_sea_level(void) {
    double h = 0.0;
    double oat = 15.0;
    double rho = Density_at_altitude(&h, &oat);
    
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, rho_0, rho);
}

void test_Pressure_at_sea_level(void) {
    double h = 0.0;
    double p = Pressure_at_altitude(&h);
    
    TEST_ASSERT_DOUBLE_WITHIN(0.1, P_0, p);
}

void test_Pressure_at_tropopause(void) {
    double h = 36089.24;
    double p = Pressure_at_altitude(&h);
    
    TEST_ASSERT_DOUBLE_WITHIN(1.0, 22632.06, p);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_Distance);
    RUN_TEST(test_CourseInitial_LAX_to_JFK);
    RUN_TEST(test_IntermediatePoint);

    RUN_TEST(test_Standard_temperature);
    RUN_TEST(test_Speed_of_sound);
    RUN_TEST(test_Density_at_sea_level);
    RUN_TEST(test_Pressure_at_sea_level);
    RUN_TEST(test_Pressure_at_tropopause);
    
    return UNITY_END();
}