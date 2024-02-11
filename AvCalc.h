/*
 * AvCalc.h
 *
 *  Created on: 17. feb. 2015
 *      Author: Arild M Johannessen
 */

#ifndef AVCALC_H_
#define AVCALC_H_


/* You should define ADD_EXPORTS *only* when building the DLL. */
#ifdef AVCALC_EXPORTS
  #define AVCALCAPI __declspec(dllexport)
#else
  #define AVCALCAPI __declspec(dllimport)
#endif

/* Define calling convention in one place, for convenience. */
#define AVCALCCALL __stdcall

/* Make sure functions are exported with C linkage under C++ compilers. */

#ifdef __cplusplus
extern "C"
{
#endif





#define EPS 1e-15 // EPS a small number ~ machine precision
#define R2D 57.295779513082320876798154814105  //multiply radian with R2D to get degrees
#define D2R 0.01745329251994329576923690768489 //multiply degrees with D2R to get radians



AVCALCAPI double AVCALCCALL Distance(const double* lat1, const double* lon1, const double* lat2, const double* lon2);
AVCALCAPI double AVCALCCALL CourseInitial (double *lat1, double *lon1, double *lat2, double *lon2);
AVCALCAPI void AVCALCCALL IntermediatePoint (const double *lat1, const double *lon1, const double *lat2, const double *lon2, const double *fraction, double *latresult, double *lonresult);

long long SpeedTest();
int main();




#ifdef __cplusplus
} // __cplusplus defined.
#endif

#endif /* AVCALC_H_ */
