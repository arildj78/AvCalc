/*
 * main.h
 *
 *  Created on: 17. feb. 2015
 *      Author: Arild
 */

#ifndef MAIN_H_
#define MAIN_H_

#define EPS 1e-15 // EPS a small number ~ machine precision
#define R2D 57.295779513082320876798154814105  //multiply radian with R2D to get degrees
#define D2R 0.01745329251994329576923690768489 //multiply degrees with D2R to get radians

double _stdcall Distance(const double* lat1, const double* lon1, const double* lat2, const double* lon2);
double _stdcall CourseInitial (double *lat1, double *lon1, double *lat2, double *lon2);
void _stdcall IntermediatePoint (const double *lat1, const double *lon1, const double *lat2, const double *lon2, const double *fraction, double *latresult, double *lonresult)

long long SpeedTest();
int main();




#endif /* MAIN_H_ */
