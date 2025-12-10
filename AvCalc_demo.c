#include <stdio.h>
#include "AvCalc.h"

int main() {
    printf("AvCalc - Aviation Calculation Library v0.01\n");
    printf("Based on Aviation Formulary V1.46 by Ed Williams\n");
    printf("Converted to C by Arild M Johannessen\n");
    printf("--------------------------------------------------\n\n");
    
    // Quick test examples
    printf("Example 1: Distance LAX to JFK\n");
    double lat1 = 33.95, lon1 = -118.4;
    double lat2 = 40.633333, lon2 = -73.783333;
    double dist = Distance(&lat1, &lon1, &lat2, &lon2);
    printf("Distance: %.2f nm\n\n", dist);
    
    printf("Example 2: Initial Course\n");
    double course = CourseInitial(&lat1, &lon1, &lat2, &lon2);
    printf("Initial course: %.2f degrees\n\n", course);
    
    printf("Example 3: Speed of Sound at -6.72°C\n");
    double oat = -6.72;
    double cs = Speed_of_sound(&oat);
    printf("Speed of sound: %.2f knots\n\n", cs);
    
    printf("Example 4: Standard Temperature at 152000 ft\n");
    double pa = 152000.0;
    double temp = Standard_temperature(&pa);
    printf("Standard temp: %.3fC\n\n", temp);
    
    printf("Press any key to exit...\n");
    getchar();
    return 0;
}