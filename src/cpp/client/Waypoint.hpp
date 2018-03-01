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

#include <string>
#include <cmath>
#include <math.h>
#include <vector>
#include <jsoncpp/json/json.h>



using namespace std;

class Waypoint {
protected:
   static constexpr double pi = 3.14159265;
   static double toRadians(double deg){
      return (deg*pi)/180.0;
   }
   static double toDegrees(double rad){
      return (rad*180.0)/pi;
   }
public:

   //magic numbers
   static constexpr int KMETER = 0;
   static constexpr int NAUTICAL = 1;
   static constexpr int STATUTE = 2;
   static constexpr int RADIUSEARTH = 6371;
   static constexpr double TOSTATUTE = 0.62137119; //convert km to statute
   static constexpr double TONAUTICAL = 0.5399568; //convert km to nautical

   //waypoint variables
   double lat;
   double lon;
   double ele;
   string name;
   string addr;

   //functions
   Waypoint();
   Waypoint(double aLat, double aLon, double anElevation, string aName, string anAddr);
   ~Waypoint();
   void setValues(double aLat, double aLon, double anElevation, string aName, string anAddr);
   static double distanceGCTo(vector<Waypoint> wp, int scale);
   static double bearingGCInitTo(vector<Waypoint> wp);
   void print();
	string toJsonString();
	Json::Value toJson();

};
