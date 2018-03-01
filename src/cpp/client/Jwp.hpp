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
 * Handles json implementation for waypoints
 * <p>
 * @author Kenneth Bonilla kfbonill@asu.edu
 * @version January, 2018
 **/

#ifndef SRC_JWP_HPP_
#define SRC_JWP_HPP_

#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>
#include "Waypoint.hpp"
#include "waypointcollectionstub.h"
#include <regex>

using namespace std;

class Jwp{
public:
	vector<Waypoint> theWps;
	vector<Json::Value> jVec;
	vector<string> jstring;

	Jwp();
	~Jwp();
	vector<Waypoint> readJson();
	vector<Waypoint> readJson(string host);

	void writeJson(vector<Waypoint> wps);
};

#endif /* SRC_JWP_HPP_ */
