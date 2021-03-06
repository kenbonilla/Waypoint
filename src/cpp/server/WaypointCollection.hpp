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
 * Modified from example studentCollectionJsonRPC
 * <p>
 * @author Kenneth Bonilla kfbonill@asu.edu
 * @version February, 2018
 * @file WaypointCollection.hpp
 **/
/**
 * Copyright 2016 Tim Lindquist,
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
 * <p/>
 * Purpose: C++ class to represent a collection of students. This class is
 * part of a student collection distributed application that uses JsonRPC.
 * Meant to run on OSX, Debian Linux, and Raspberry Pi Debian.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version July 2016
 */

#ifndef WAYPOINTCOLLECTION_HPP_
#define WAYPOINTCOLLECTION_HPP_

#include "Waypoint.hpp"
#include <string>
#include <map>
#include <vector>
#include <jsoncpp/json/json.h>

using namespace std;

class WaypointCollection {
protected:
	std::map<std::string, Waypoint> library;
public:
	WaypointCollection();
	~WaypointCollection();
	bool resetFromJsonFile(string jsonFileName);
	bool saveToJsonFile(string jsonFileName);
	bool add(const Json::Value& aWaypointJson);
	bool remove(string waypointName);
	Json::Value get(string waypointName); //returns the WP's Json
	Json::Value getNames(); // returns a Json array of strings
	
};


#endif /* WAYPOINTCOLLECTION_HPP_ */
