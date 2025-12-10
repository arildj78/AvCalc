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
 * AvCalc.c
 * -------------------------------------------------------------------------------
 *                          Aviation Formulary V1.46
 *                              By Ed Williams
 *             Converted to C by Arild M Johannessen (December 2025)
 *
 * -------------------------------------------------------------------------------
 * Change from the original work and implementation notice
 * -------------------------------------------------------------------------------
 * Eastern longitudes are now positive values, western are negative.
 * North/south is unchanged with north as positive
 * Input is changed from radians to degrees and nautical miles.
 *
 * The following functions are re-implemented to conform to
 * ICAO Doc 7488 - MANUAL OF THE ICAO STANDARD ATMOSPHERE
 * from -5km to 80km altitude:
 *          Standard_temperature()
 * 
 *
 *
 * Calling convention for all methods is: stdcall (compatible with Win32 API)
 * -----------------------------------------------------------------------------*/

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>  //To be removed when speedtesting is complete
#include <stdio.h>    //To be removed when speedtesting is complete
#include <inttypes.h> //To be removed when speedtesting is complete
#include "AvCalc.h"



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
double AVCALCCALL Distance(const double* lat1, const double* lon1, const double* lat2, const double* lon2)
{
    // multiplication by D2R converts input in degrees to radians for the trig functions
    // multiplication by R2D converts radians to degrees
    // multiplication by  60 converts degrees to nautical miles
    return 60 * R2D * 2 * asin(sqrt(pow(sin(D2R*(*lat1-*lat2)/2),2) +
                                    pow(sin(D2R*(*lon2-*lon1)/2),2) * cos(D2R* *lat1) * cos(D2R* *lat2)
                                   )
                              );
};


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


double AVCALCCALL CourseInitial (double *lat1, double *lon1, double *lat2, double *lon2)
{
    double radLat1 = D2R * *lat1;
    double radLon1 = D2R * *lon1;
    double radLat2 = D2R * *lat2;
    double radLon2 = D2R * *lon2;

    if (cos(radLat1) < EPS) {     // EPS a small number ~ machine precision
        if (radLat1 > 0) {
            return R2D * M_PI;      //  Starting position is North pole, return true course south
        } else {
            return R2D * 2*M_PI;    //  Starting position is South pole, return true course north
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
void AVCALCCALL IntermediatePoint (const double *lat1, const double *lon1, const double *lat2, const double *lon2, const double *fraction, double *latresult, double *lonresult)
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

double AVCALCCALL TAS_2(const double *CAS, const double *pressure_alt, const double *oat){
    return -1; //To be implemented
}
double AVCALCCALL CAS_2(const double *TAS, const double *pressure_alt, const double *oat){
    return -1; //To be implemented	
}

double AVCALCCALL Speed_of_sound(const double *oat){
    return 38.967854 * sqrt(273.15 + *oat); //Speed of sound in ft/s
}

double AVCALCCALL Pressure_at_altitude(const double *h){
    double p = 0.0;
	  double p_Tr = 0.0;

    if (*h < 36089.24) {
        p = P_0 * pow(1.0 - 6.8755856e-6 * (*h), 5.2558797);
        return p;
    } else if ((*h >= 36089.24) && (*h < 65616.8)) {
        p_Tr = 22632.06;  //Pressure at the tropopause
        p = p_Tr * exp(-4.806346e-5 * (*h - 36089.24));
        return p;
    } else {
        return -1; //Error condition	
    }
}

double AVCALCCALL Density_at_altitude(const double *h, const double *oat){
    double rho = 0.0;
    double rho_Tr = 0.0;

    if (*h < 36089.24) {
        rho	= rho_0*pow(1.0 - 6.8755856e-6 * (*h), 4.2558797);
        return rho;
    } else if ((*h >= 36089.24) && (*h < 65616.8)) {
        rho_Tr = 0.2970756 * rho_0;
        rho=rho_Tr*exp(-4.806346e-5 * (*h-36089.24));
        return rho;
    } else {
        return -1; //Error condition
    }
}

double AVCALCCALL Standard_temperature(const double *pressure_alt){
    const double h = *pressure_alt; // feet

    // Band boundaries (feet)
    const double h0 = -5000 / 0.3048;  // -5 km  (below sea level)
    const double h1 = 0.0;             //  0 km  (sea level)
    const double h2 = 11000 / 0.3048;  // 11 km  (lower tropopause)
    const double h3 = 20000 / 0.3048;  // 20 km  (upper tropopause)
    const double h4 = 32000 / 0.3048;  // 32 km  (middle stratosphere)
    const double h5 = 47000 / 0.3048;  // 47 km  (lower stratopause)
    const double h6 = 51000 / 0.3048;  // 51 km  (upper stratopause)
    const double h7 = 71000 / 0.3048;  // 71 km  (middle mesosphere)
    const double h8 = 80000 / 0.3048;  // 80 km  (lower mesopause)

    // Reject out-of-range inputs
    if (h < h0 || h > h8) {
        return NAN; // Out of modeled range [-5 km, 80 km]
    }

    // Lapse rates (°C per foot), converted from °C/km
    const double L0 = -6.5 / 3280.84; // -5 km to 0 km  (troposphere)
    const double L1 = -6.5 / 3280.84; //  0 km to 11 km (troposphere)
    const double L2 =  0.0;           // 11 km to 20 km (tropsopause, isothermal)
    const double L3 =  1.0 / 3280.84; // 20 km to 32 km (stratosphere, lower)
    const double L4 =  2.8 / 3280.84; // 32 km to 47 km (stratosphere, upper)
    const double L5 =  0.0;           // 47 km to 51 km (stratopause, isothermal)
    const double L6 = -2.8 / 3280.84; // 51 km to 71 km (mesosphere, lower)
    const double L7 = -2.0 / 3280.84; // 71 km to 80 km (mesosphere, upper)

    // Anchor temperatures at band starts (continuous)
    const double T0 = 15.0 - L0 * (h1 - h0);           // at -5 km (≈47.5°C)
    const double T1 = 15.0;                            // at 0 ft
    const double T2 = T1 + L1 * (h2 - h1);             // at 11 km
    const double T3 = T2 + L2 * (h3 - h2);             // at 20 km
    const double T4 = T3 + L3 * (h4 - h3);             // at 32 km
    const double T5 = T4 + L4 * (h5 - h4);             // at 47 km
    const double T6 = T5 + L5 * (h6 - h5);             // at 51 km
    const double T7 = T6 + L6 * (h7 - h6);             // at 71 km
    const double T8 = T7 + L7 * (h8 - h7);             // at 80 km

    // Calculate temperature based on altitude band
    if (h < h1) return T0 + L0 * (h - h0);        // -5 km to 0 km
    if (h < h2) return T1 + L1 * (h - h1);        // 0 to 11 km
    if (h < h3) return T2 + L2 * (h - h2);        // 11 to 20 km (iso)
    if (h < h4) return T3 + L3 * (h - h3);        // 20 to 32 km
    if (h < h5) return T4 + L4 * (h - h4);        // 32 to 47 km
    if (h < h6) return T5 + L5 * (h - h5);        // 47 to 51 km (iso)
    if (h < h7) return T6 + L6 * (h - h6);        // 51 to 71 km
    /* h <= h8 */ return T7 + L7 * (h - h7);      // 71 to 80 km
}