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
 * @file Waypoint.cpp
 **/
/**
 * Copyright (c) 2016 Tim Lindquist,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ class to represent a student. This class is part of a
 * student collection distributed application that uses JsonRPC.
 * Meant to run on OSX, Debian Linux, and Raspberry Pi Debian.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu),ASU-IAFSE,Software Engineering
 * --file    Student.cpp
 * @date    July, 2016
 * @license See above
 **/

#include "Waypoint.hpp"
#include <iostream>
#include <stdlib.h>

Waypoint::Waypoint() {
	lat = lon = ele = 0;
	name = "";
	addr = "";
}

Waypoint::Waypoint(double aLat, double aLon, double anElevation, string aName,
		string anAddr) {
	lat = aLat;
	lon = aLon;
	ele = anElevation;
	name = aName;
	addr = anAddr;
}

Waypoint::Waypoint(const Json::Value& jsonObj) {
	string nameString = "name";
	string latString = "lat";
	string lonString = "lon";
	string eleString = "ele";
	string addrString = "address";
	Json::Value::Members mbr = jsonObj.getMemberNames();
	for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end(); i++) {
		Json::Value jsonMbr = jsonObj[*i];
		if (nameString.compare(*i) == 0) {
			name = jsonMbr.asString();
		} else if (latString.compare(*i) == 0) {
			lat = jsonMbr.asDouble();
		} else if (lonString.compare(*i) == 0) {
			lon = jsonMbr.asDouble();
		} else if (eleString.compare(*i) == 0) {
			ele = jsonMbr.asDouble();
		} else if (addrString.compare(*i) == 0) {
			addr = jsonMbr.asString();
		}
	}
}

Waypoint::Waypoint(string jsonString) {
	string nameString = "name";
	string latString = "lat";
	string lonString = "lon";
	string eleString = "ele";
	string addrString = "address";
	Json::Reader reader;
	Json::Value root;
	bool parseSuccess = reader.parse(jsonString, root, false);
	if (parseSuccess) {
		Json::Value::Members mbr = root.getMemberNames();
		for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end();
				i++) {
			Json::Value jsonMbr = root[*i];
			if (nameString.compare(*i) == 0) {
				name = jsonMbr.asString();
			} else if (latString.compare(*i) == 0) {
				lat = jsonMbr.asDouble();
			} else if (lonString.compare(*i) == 0) {
				lon = jsonMbr.asDouble();
			} else if (eleString.compare(*i) == 0) {
				ele = jsonMbr.asDouble();
			} else if (addrString.compare(*i) == 0) {
				addr = jsonMbr.asString();
			}
		}
	} else {
		cout << "Failed to parse Waypoint JSON with: " << jsonString << endl;
	}
}

Waypoint::~Waypoint() {
	lat = lon = ele = 0;
	name = addr = "";
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

//void fromJson(Json::Value json) {
//
//}

void Waypoint::setValues(double aLat, double aLon, double anElevation,
		string aName, string anAddr) {
	lat = aLat;
	lon = aLon;
	ele = anElevation;
	name = aName;
	addr = anAddr;
}

void Waypoint::print() {
	cout << "\n\nWaypoint " << name << " lat " << lat << " lon " << lon << "\n";
}
