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
  Argument 1: INPUT - Pointer to double containing Latitude  of point 1
  Argument 2: INPUT - Pointer to double containing Longitude of point 1
  Argument 3: INPUT - Pointer to double containing Latitude  of point 2
  Argument 4: INPUT - Pointer to double containing Longitude of point 2

  RETURN: Double containing distance in nautical miles (1nm = 1852m)
--------------------------------------------------------------------------*/
double _stdcall Distance(const double* lat1, const double* lon1, const double* lat2, const double* lon2)
{
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
*----------------------------------------------------------------------------*/


double _stdcall Course (double *lat1, double *lon1, double *lat2, double *lon2)
{
  if (cos(*lat1) < EPS) {     // EPS a small number ~ machine precision
	  if (*lat1 > 0) {
		  return M_PI;        //  Starting from N pole, return true course south
	  } else {
		  return 2*M_PI;      //  starting from S pole, return true course north
	  }
  } else {
      // Calculate and return the true course
      return fmod(atan2(sin(*lon2-*lon1) * cos(*lat2),
                        cos(*lat1) * sin(*lat2) - sin(*lat1) * cos(*lat2) * cos(*lon2-*lon1)
                       ),
                  2*M_PI
                 );
  }
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
