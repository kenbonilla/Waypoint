
/**
 * Copyright 2018 Kenneth Bonilla,
 * <p>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * <p>
 * Store and manipulate wps, calculate dist and bearing
 * <p>
 * @author Kenneth Bonilla kfbonill@asu.edu
 * @version January, 2018
 **/
/**
 * Copyright 2018 Tim Lindquist,
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Purpose: demonstrate classes, alloc, init, in the context of
 * distance and direction calculations from one earth position (waypoint)
 * to another.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version January 2018
 */

#include "Waypoint.hpp"

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cmath>


Waypoint::Waypoint(){
   lat = lon = ele = 0;
   name = "";
   addr = "";
}

Waypoint::Waypoint(double aLat, double aLon, double anElevation, string aName, string anAddr) {
   lat = aLat;
   lon = aLon;
   ele = anElevation;
   name = aName;
   addr = anAddr;
}

Waypoint::~Waypoint() {
   lat=lon=ele=0; name=addr="";
}

void Waypoint::setValues(double aLat, double aLon, double anElevation,
                         string aName, string anAddr) {
   lat = aLat;
   lon = aLon;
   ele = anElevation;
   name = aName;
   addr = anAddr;
}

void Waypoint::print(){
   cout << "\n\nWaypoint " << name << " lat "
        << lat << " lon " << lon << "\n";
}
string Waypoint::toJsonString() {
	string result = "{}";
	Json::Value jsonLib;
	jsonLib["name"] = name;
	jsonLib["lat"] = lat;
	jsonLib["lon"] = lon;
	jsonLib["ele"] = ele;
	jsonLib["address"] = addr;

	result = jsonLib.toStyledString();
	return result;
}

Json::Value Waypoint::toJson() {
	Json::Value jsonLib;
	jsonLib["name"] = name;
	jsonLib["lat"] = lat;
	jsonLib["lon"] = lon;
	jsonLib["ele"] = ele;
	jsonLib["address"] = addr;

	return jsonLib;
}

double Waypoint::distanceGCTo(vector<Waypoint> wp, int scale){
	/* Haversine formula
	 *
	 * - http://www.movable-type.co.uk/scripts/latlong.html
	 *
	 * a = sin²(Δφ/2) + cos φ1 ⋅ cos φ2 ⋅ sin²(Δλ/2)
	 * c = 2 ⋅ atan2( √a, √(1−a) )
	 * d = R ⋅ c
	 * where φ is latitude, λ is longitude, R is earth’s radius
	 *
	 * latitude must be in Radians
	 */
	int vSize = wp.size();
	if (vSize != 2) return 0; // sanity check, must have two points

	double lat1 = wp[0].lat;
	double lat2 = wp[1].lat;
	double lon1 = wp[0].lon;
	double lon2 = wp[1].lon;

	double radLat1 = toRadians(lat1);
	double radLat2 = toRadians(lat2);
	double radLat = toRadians(lat2 - lat1);
	double radLon = toRadians(lon2 - lon1);

	double a = pow(sin(radLat/2), 2);
	a += cos(radLat1) * cos(radLat2) * pow(sin(radLon/2), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1-a));
	double result = RADIUSEARTH * c;

	switch(scale) {
		case STATUTE:
			result *= TOSTATUTE;
			break;
		case NAUTICAL:
			result *= TONAUTICAL;
			break;
		case KMETER:
			break;
		default:
			break;
		}

	return result;

}

double Waypoint::bearingGCInitTo(vector<Waypoint> wp){
    /* Bearing formula
     *
     * - http://www.movable-type.co.uk/scripts/latlong.html
     *
     * Formula:    θ = atan2( sin Δλ ⋅ cos φ2 , cos φ1 ⋅ sin φ2 − sin φ1 ⋅ cos φ2 ⋅ cos Δλ )
     * where   φ1,λ1 is the start point, φ2,λ2 the end point (Δλ is the difference in longitude)
     *
     * ATAN2(COS(lat1)*SIN(lat2)-SIN(lat1)*COS(lat2)*COS(lon2-lon1),
     * SIN(lon2-lon1)*COS(lat2))
     */

	int vSize = wp.size();
	if (vSize != 2) return 0; // sanity check, must have two points

	double lat1 = wp[0].lat;
	double lat2 = wp[1].lat;
	double lon1 = wp[0].lon;
	double lon2 = wp[1].lon;

	double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lon2 - lon1);
	double y = sin(lon2 - lon1) * cos(lat2);
	double result = toDegrees(atan2(y, x));
	if(result < 0) {
	            result *= -1;
	            result += 180;
	        }

	return result;
}









