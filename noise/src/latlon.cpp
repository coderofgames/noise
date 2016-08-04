// latlon.cpp
//
// Copyright (C) 2003, 2004 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#include "latlon.h"

using namespace noise;

void noise::LatLonToXYZ (double lat, double lon, double& x, double& y,
  double& z)
{
  double r = cos (DEG_TO_RAD * lat);
  x = r * cos (DEG_TO_RAD * lon);
  y =     sin (DEG_TO_RAD * lat);
  z = r * sin (DEG_TO_RAD * lon);
}

//	lat = asin( y ) * RAD_TO_DEG;
//	lon = acos( x/r ) * RAD_TO_DEG;
//  lon = acos( x/ cos( DEG_TO_RAD* lat) ) * RAD_TO_DEG;
//
//  lon = acos( x/ cos( asin( y )) ) * RAD_TO_DEG
// r^2 = x^2 + y^2 + z^2
//
// doh
// lon = atan2( y / x ) * RAD_TO_DEG;
//
// r^2 = r^2*cos^2(DEG_TO_RAD*lon) + sin^2(DEG_TO_RAD*lat) + r^2*sin^2(DEG_TO_RAD*lon)
// r^2 = r^2 + sin^2(DEG_TO_RAD*lat) 
// 2*r^2 = sin^2(DEG_TO_RAD*lat)
// r = sqrt(1/2)sin(DEG_TO_RAD*lat)

void noise::XYZToLatLon( double x, double y, double z, double &lat, double &lon )
{
	lat = asin( y ) * RAD_TO_DEG;
	//lon = acos( x / cos( DEG_TO_RAD * lat ) ) * RAD_TO_DEG;
	lon = atan( y / x ) * RAD_TO_DEG;
}