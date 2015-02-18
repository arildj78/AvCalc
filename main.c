/*
 * main.c
 * -------------------------------------------------------------------------------
 *                          Aviation Formulary V1.46
 *                              By Ed Williams
 *
 * -------------------------------------------------------------------------------
 *
 * Rewritten to C by
 *
 * Arild M Johannessen
 * Air Navigator
 * 330 squadron
 * Royal Norwegian Air Force
 *
 * -------------------------------------------------------------------------------
 * Implementation notice
 * -------------------------------------------------------------------------------
 * East/West longitude is rewritten to conform with the defacto standard
 * used in most other applications throughout the world.
 * North and East is positive numbers
 *
 *
 */

#include <math.h>
#include <Windows.h>  //Kun for testing
#include <stdio.h>    //Kun for testing (tror jeg)
#include <inttypes.h> //Kun for testing
#include "main.h"





/*
 *--------------------------------------------------------------------------------
 * Some great circle formulae:
 * ----------------------------------------------------------------------------
 */



/*
 * Distance between points
 *
 * The great circle distance d between two points with coordinates {lat1,lon1}
 * and {lat2,lon2} is given by:
 * returns a pointer to a double containing distance expressed as radians
 * ----------------------------------------------------------------------------
 */
double _stdcall Distance(double* lat1, double* lon1, double* lat2, double* lon2)
{
	return R2D * 2 * asin(sqrt(pow(sin(D2R*(*lat1-*lat2)/2),2) +
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
	  return fmod(
				  atan2(sin(*lon2-*lon1) * cos(*lat2),
						cos(*lat1) * sin(*lat2) - sin(*lat1) * cos(*lat2) * cos(*lon2-*lon1)
					   ),
				  2*M_PI
				 );
  }
}

long long SpeedTest(){
	LARGE_INTEGER StartingTime, EndingTime, Frequency;
	uint64_t  ElapsedNanoseconds1, ElapsedNanoseconds2;
	uint64_t  duration1, duration2;

	QueryPerformanceFrequency(&Frequency);

	unsigned long Repetitions = 20e6;



	double x;
	double lat1 = 69;
	double lon1 = 17;
	double lat2 = 70;
	double lon2 = 18;
	x=0;

	//Single run through
	QueryPerformanceCounter(&StartingTime);
	//procedure to be tested
	x=DistanceD(lat1, lon1, lat2, lon2);
	QueryPerformanceCounter(&EndingTime);
	duration1 = EndingTime.QuadPart - StartingTime.QuadPart;




	//20 mill runthroughs
	QueryPerformanceCounter(&StartingTime);
	unsigned long n;
	for(n = 0;n<Repetitions; n++){
		//procedure to be tested
		x=Distance(lat1, lon1, lat2, lon2);
	}
	QueryPerformanceCounter(&EndingTime);
	duration2 = EndingTime.QuadPart - StartingTime.QuadPart;

	ElapsedNanoseconds1 = 1e9 * duration1;
	ElapsedNanoseconds1 /= Frequency.QuadPart;

	ElapsedNanoseconds2 = 1e9 * duration2;
	ElapsedNanoseconds2 /= Frequency.QuadPart;


	double Average = (double) ElapsedNanoseconds2/(double) Repetitions;
	double RepetitionsD = Repetitions;
	printf("Single  runtime = %.2fns\n",(double) ElapsedNanoseconds1 );
	printf("Average runtime over %.2e repetitions = %.8fns\n",RepetitionsD, Average );

}

int main(){
	SpeedTest();
	return 0;
}
