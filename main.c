/* Aviation Calculation - AvCalc API v0.01
   Copyright (C) 2015  Arild M Johannessen
 
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */

/*
 * main.c
 * -------------------------------------------------------------------------------
 *                          Aviation Formulary V1.46
 *                              By Ed Williams
 *             Converted to C by Arild M Johannessen (February 2015)
 *
 * -------------------------------------------------------------------------------
 * Change from the original work and implementation notice
 * -------------------------------------------------------------------------------
 * Eastern longitudes are now positive values, western are negative.
 * North/south is unchanged with north as positive
 * Input is changed from radians to degrees and nautical miles.

 * Calling convention for all methods is: stdcall (compatible with Win32 API)
 * -----------------------------------------------------------------------------*/

#include <math.h>
#include <Windows.h>  //To be removed when speedtesting is complete
#include <stdio.h>    //To be removed when speedtesting is complete
#include <inttypes.h> //To be removed when speedtesting is complete
#include "main.h"



/*--------------------------------------------------------------------------
  Distance between points

  The great circle distance d between two points with coordinates {lat1,lon1}
  and {lat2,lon2} is given by:
----------------------------------------------------------------------------
  Implementation
  Argument 1: INPUT - Pointer to double containing Latitude  of point 1 in degrees
  Argument 2: INPUT - Pointer to double containing Longitude of point 1 in degrees
  Argument 3: INPUT - Pointer to double containing Latitude  of point 2 in degrees
  Argument 4: INPUT - Pointer to double containing Longitude of point 2 in degrees

  RETURN: Double containing distance in nautical miles (1nm = 1852m)
--------------------------------------------------------------------------*/
double _stdcall Distance(const double* lat1, const double* lon1, const double* lat2, const double* lon2)
{
    // multiplication by D2R converts input in degrees to radians for the trig functions
    // multiplication by R2D converts radians to degrees
    // multiplication by  60 converts degrees to nautical miles
    return 60 * R2D * 2 * asin(sqrt(pow(sin(D2R*(*lat1-*lat2)/2),2) +
                                    pow(sin(D2R*(*lon2-*lon1)/2),2) * cos(D2R* *lat1) * cos(D2R* *lat2)
                                   )
                              );
}


/*----------------------------------------------------------------------------
* Course between points

* We obtain the initial course, tc1, (at point 1) from point 1 to point 2
* by the following. The formula fails if the initial point is a pole. We can
* special case this with as IF statement
----------------------------------------------------------------------------
  Implementation
  Argument 1: INPUT - Pointer to double containing Latitude  of point 1 in degrees
  Argument 2: INPUT - Pointer to double containing Longitude of point 1 in degrees
  Argument 3: INPUT - Pointer to double containing Latitude  of point 2 in degrees
  Argument 4: INPUT - Pointer to double containing Longitude of point 2 in degrees

  RETURN: Double containing initial course in degrees from point1 to point 2
--------------------------------------------------------------------------*/


double _stdcall CourseFinal (double *lat1, double *lon1, double *lat2, double *lon2)
{
    double radLat1 = D2R * *lat1;
    double radLon1 = D2R * *lon1;
    double radLat2 = D2R * *lat2;
    double radLon2 = D2R * *lon2;

    if (cos(radLat1) < EPS) {     // EPS a small number ~ machine precision
	  if (radLat1 > 0) {
		  return R2D * M_PI;      //  Starting from N pole, return true course south
	  } else {
		  return R2D * 2*M_PI;    //  starting from S pole, return true course north
	  }
  } else {
      // Calculate and return the true course
      return R2D * fmod(atan2(sin(radLon2-radLon1) * cos(radLat2),
                              cos(radLat1) * sin(radLat2) - sin(radLat1) * cos(radLat2) * cos(radLon2-radLon1)
                             ),
                        2*M_PI
                       );
  }
}


/*----------------------------------------------------------------------------
Intermediate points on a great circle:

In previous sections we have found intermediate points on a great
circle given either the crossing latitude or longitude. Here we find
points (lat,lon) a given fraction of the distance (d) between
them. Suppose the starting point is (lat1,lon1) and the final point
(lat2,lon2) and we want the point a fraction f along the great circle
route. f=0 is point 1. f=1 is point 2.  The two points cannot be
antipodal ( i.e. lat1+lat2=0 and abs(lon1-lon2)=pi) because then the
route is undefined.  The intermediate latitude and longitude is then given by:
----------------------------------------------------------------------------
  Implementation
  Argument 1: INPUT - Pointer to double containing Latitude  of point 1 in degrees
  Argument 2: INPUT - Pointer to double containing Longitude of point 1 in degrees
  Argument 3: INPUT - Pointer to double containing Latitude  of point 2 in degrees
  Argument 4: INPUT - Pointer to double containing Longitude of point 2 in degrees

  RETURN: Double containing initial course in degrees from point1 to point 2
--------------------------------------------------------------------------*/
void _stdcall IntermediatePoint (const double *lat1, const double *lon1, const double *lat2, const double *lon2, const double *fraction, double *latresult, double *lonresult)
{
    double A, B, x, y, z, d;

    double radLat1 = D2R * *lat1;
    double radLon1 = D2R * *lon1;
    double radLat2 = D2R * *lat2;
    double radLon2 = D2R * *lon2;

    //d = distance in radians between point 1 and point 2
    d = 2 * asin(sqrt(pow(sin((radLat1-radLat2)/2),2) +
                      pow(sin((radLon2-radLon1)/2),2) * cos(radLat1) * cos(radLat2)
                     )
                );

    A = sin((1-*fraction)*d)/sin(d);
    B = sin(*fraction*d)/sin(d);
    x = A*cos(radLat1)*cos(radLon1) +  B*cos(radLat2)*cos(radLon2);
    y = A*cos(radLat1)*sin(radLon1) +  B*cos(radLat2)*sin(radLon2);
    z = A*sin(radLat1)              +  B*sin(radLat2);
    *latresult = R2D * atan2(z,sqrt(pow(x,2)+pow(y,2)));
    *lonresult = R2D * atan2(y,x);
}



long long SpeedTest(){
	LARGE_INTEGER StartingTime, EndingTime, Frequency;
	uint64_t  ElapsedNanoseconds;
	uint64_t  duration;

	QueryPerformanceFrequency(&Frequency);

	unsigned long Repetitions = 20e6;



	double x;
	double lat1 = 69;
	double lon1 = 17;
	double lat2 = 70;
	double lon2 = 18;
	x=0;


	//20 mill runthroughs
	QueryPerformanceCounter(&StartingTime);
	unsigned long n;
	for(n = 0;n<Repetitions; n++){
		//procedure to be tested
		x=Distance(&lat1, &lon1, &lat2, &lon2);
	}
	QueryPerformanceCounter(&EndingTime);
	duration = EndingTime.QuadPart - StartingTime.QuadPart;

	ElapsedNanoseconds = 1e9 * duration;
	ElapsedNanoseconds /= Frequency.QuadPart;

	double Average = (double) ElapsedNanoseconds/(double) Repetitions;

	printf("Average runtime over %.2e repetitions = %.8fns\n",(double) Repetitions, Average );

}

int main(){
	SpeedTest();
	return 0;
}
