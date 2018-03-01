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
 * Drives the GUI using the defs in WaypointGUI.cpp
 * <p>
 * @author Kenneth Bonilla kfbonill@asu.edu
 * @version January, 2018
 **/

/**
 * Copyright (c) 2018 Tim Lindquist,
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
 * Purpose: C++ FLTK client UI for Waypoint management.
 * This class extends the Gui component class WaypointGUI and demonstrates
 * sample control functions that respond to button clicks drop-down selects.
 * This software is meant to run on Linux and MacOS using g++.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @file    samplemain.cpp
 * @date    January, 2018
 **/

#include "WaypointGUI.cpp"
#include "Jwp.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Multiline_Input.H>

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <jsoncpp/json/json.h>
//#include <json/json.h>
#include "waypointcollectionstub.h"
#include <regex>
#include <iterator>

using namespace std;

vector<Waypoint> wps;	//holds the waypoints
int unitFlag = 0; // 0 km, 1 nautical, 2 statute
string host = "http://localhost:8080";
//jsonrpc::HttpClient httpclient(host);
//waypointcollectionstub ww(httpclient);



class Client: public WaypointGUI {


	static void ClickedX(Fl_Widget * w, void * userdata) {
		//std::cout << "You clicked Exit" << std::endl;
		exit(1);
	}

	//############# REMOVE WP
	static void ClickedRemoveWP(Fl_Widget * w, void * userdata) {
		//open the server
		jsonrpc::HttpClient httpclient(host);
		waypointcollectionstub ww(httpclient);
		//
		Client* anInstance = (Client*) userdata;
		Fl_Input_Choice * theWPChoice = anInstance->frWps;
		Fl_Input_Choice * toWps = anInstance->toWps;
		Fl_Input * message = anInstance->distBearIn;
		std::string selected(theWPChoice->value());
		//std::cout << "You clicked the remove waypoint button with "<< selected<< std::endl;
		//removes the wp @ frWps from menus "to" and "from"
		for (int i = 0; i < theWPChoice->menubutton()->size(); i++) {
			const Fl_Menu_Item &item = theWPChoice->menubutton()->menu()[i];
			if (!selected.compare(item.label())) {  // if they are equal
				theWPChoice->menubutton()->remove(i);
				toWps->menubutton()->remove(i);
				//cout << "removed " << selected << endl;
				break;
			}
		}
		//if current selection, clear the field "from"
		if (theWPChoice->menubutton()->size() > 0) {
			theWPChoice->value(theWPChoice->menubutton()->menu()[0].label());
		} else {
			theWPChoice->value("");
		}
		//if current selection, clear the field "to"
		if (toWps->menubutton()->size() > 0) {
			toWps->value(toWps->menubutton()->menu()[0].label());
		} else {
			toWps->value("");
		}
		//remove the wp from the vector
		if (wps.size() != 0) {
			for (unsigned i = 0; i < wps.size(); i++) {
				if (selected == wps[i].name) {
					wps.erase(wps.begin() + i);
					string success = "Removed " + selected;
					message->value(success.c_str());
				}
			}
		}
		ww.remove(selected);

	}

	//############# ADD WP
	static void ClickedAddWP(Fl_Widget * w, void * userdata) {
		//open the server
		jsonrpc::HttpClient httpclient(host);
		waypointcollectionstub ww(httpclient);
		//
		Client* anInstance = (Client*) userdata;
		Fl_Input_Choice * fromWPChoice = anInstance->frWps;
		Fl_Input_Choice * toWPChoice = anInstance->toWps;
		Fl_Input * theLat = anInstance->latIn;
		Fl_Input * theLon = anInstance->lonIn;
		Fl_Input * theEle = anInstance->eleIn;
		Fl_Input * theName = anInstance->nameIn;
		Fl_Input * theAddr = anInstance->addrIn;
		Fl_Input * message = anInstance->distBearIn;
		// moved formatting lat lon ele values to separate function
		std::string lat(theLat->value());
		string latCppStr(DoubleFormater(lat));

		std::string lon(theLon->value());
		string lonCppStr(DoubleFormater(lon));

		std::string ele(theEle->value());
		string eleCppStr(DoubleFormater(ele));

		std::string name(theName->value());
		std::string addr(theAddr->value());

		fromWPChoice->add(name.c_str());
		toWPChoice->add(name.c_str());
		fromWPChoice->value(name.c_str()); //sets from to recently added selection
		//DONE: implement WAYPOINT storage

		Waypoint twp;
		twp.setValues(makeDouble(lat), makeDouble(lon), makeDouble(ele), name,
				addr);
		wps.push_back(twp);
		Json::Value jsonToPass = twp.toJson();
		bool sx_pass = ww.add(jsonToPass);
		string success = "Added " + name;
		if(sx_pass)message->value(success.c_str());

	}

	static double makeDouble(string s) {
		return atof(s.c_str());;
	}

	static string DoubleFormater(string s) {
		//formatting for lat and lon value passed
		double num = makeDouble(s);
		char numFormat[10];
		sprintf(numFormat, "%4.4f", num);
		string result(numFormat);
		return result;
	}

	//############# SELECT WP
	static void SelectedFromWP(Fl_Widget * w, void * userdata) {
		/*
		 * loads the wp from vector based on name
		 */

		Client* anInstance = (Client*) userdata;

		Fl_Input_Choice * frWps = anInstance->frWps;
		Fl_Input * theLat = anInstance->latIn;
		Fl_Input * theLon = anInstance->lonIn;
		Fl_Input * theEle = anInstance->eleIn;
		Fl_Input * theName = anInstance->nameIn;
		Fl_Input * theAddr = anInstance->addrIn;

		std::string selected(frWps->value());
		int fidx = FetchWPIdx(selected);
		if (fidx == -1)
			return; //wp not found
		Waypoint twp = wps.at(fidx);
		std::string lat(to_string(twp.lon));
		std::string lon(to_string(twp.lat));
		std::string ele(to_string(twp.ele));
		std::string name(twp.name);
		std::string addr(twp.addr);
		//cout<<lat<<"\n"<<lon<<"\n"<<name<<endl;
		theLat->value(lat.c_str());
		theLon->value(lon.c_str());
		theEle->value(ele.c_str());
		theName->value(name.c_str());
		theAddr->value(addr.c_str());

	}

	static void SelectedToWP(Fl_Widget * w, void * userdata) {
		/*
		 * loads the wp from vector based on name
		 */

		Client* anInstance = (Client*) userdata;
		Fl_Input_Choice * toWps = anInstance->toWps;
		Fl_Input * theLat = anInstance->latIn;
		Fl_Input * theLon = anInstance->lonIn;
		Fl_Input * theEle = anInstance->eleIn;
		Fl_Input * theName = anInstance->nameIn;
		Fl_Input * theAddr = anInstance->addrIn;

		std::string selected(toWps->value());
		//std::cout << "You selected from waypoint "<< selected<< std::endl;

		int fidx = FetchWPIdx(selected);
		if (fidx == -1) return; //wp not found
		Waypoint twp = wps.at(fidx);
		std::string lat(to_string(twp.lon));
		std::string lon(to_string(twp.lat));
		std::string ele(to_string(twp.ele));
		std::string name(twp.name);
		std::string addr(twp.addr);
		//cout<<lat<<"\n"<<lon<<"\n"<<name<<endl;
		theLat->value(lat.c_str());
		theLon->value(lon.c_str());
		theEle->value(ele.c_str());
		theName->value(name.c_str());
		theAddr->value(addr.c_str());
	}

	static void ClickedModWP(Fl_Widget * w, void * userdata) {
		//open the server
		jsonrpc::HttpClient httpclient(host);
		waypointcollectionstub ww(httpclient);
		//
		Client* anInstance = (Client*) userdata;
		Fl_Input * theLat = anInstance->latIn;
		Fl_Input * theLon = anInstance->lonIn;
		Fl_Input * theEle = anInstance->eleIn;
		Fl_Input * theName = anInstance->nameIn;
		Fl_Input * theAddr = anInstance->addrIn;

		std::string lat(theLat->value());
		string latCppStr(DoubleFormater(lat));

		std::string lon(theLon->value());
		string lonCppStr(DoubleFormater(lon));

		std::string ele(theEle->value());
		string eleCppStr(DoubleFormater(ele));

		std::string name(theName->value());
		std::string addr(theAddr->value());

		int fidx = FetchWPIdx(name);
		if (fidx == -1)
			return; //wp not found

		Waypoint twp;
		twp.setValues(makeDouble(lat), makeDouble(lon), makeDouble(ele), name,
				addr);
		wps.at(fidx) = twp; //replace the current
		ww.remove(name);
		Json::Value wpToPass = twp.toJson();
		ww.add(wpToPass);

	}

	//############# IMPORT JSON
	static void ClickedImportJson(Fl_Widget * w, void * userdata) {
		//open the server
		jsonrpc::HttpClient httpclient(host);
		waypointcollectionstub ww(httpclient);
		//
		Client* anInstance = (Client*) userdata;
		Fl_Input_Choice * frWps = anInstance->frWps;
		Fl_Input_Choice * toWps = anInstance->toWps;
		Fl_Input * theLat = anInstance->latIn;
		Fl_Input * theLon = anInstance->lonIn;
		Fl_Input * theEle = anInstance->eleIn;
		Fl_Input * theName = anInstance->nameIn;
		Fl_Input * theAddr = anInstance->addrIn;
		Fl_Input * message = anInstance->distBearIn;
		//init json reader
		//read the json from server and pass back wp collection
		Jwp jwp;
		wps = jwp.readJson(host);

		//clear fields
		frWps->clear();
		toWps->clear();
		//frWps->value("");
		//toWps->value("");
		for (auto const& i : wps) {
			frWps->add(i.name.c_str());
			toWps->add(i.name.c_str());
		}
		theLat->value("");
		theLon->value("");
		theEle->value("");
		theName->value("");
		theAddr->value("");
		message->value("Json Imported");
	}

	//############# EXPORT JSON
	static void ClickedExportJson(Fl_Widget * w, void * userdata) {
		//open the server
		jsonrpc::HttpClient httpclient(host);
		waypointcollectionstub ww(httpclient);
		//
		Client* anInstance = (Client*) userdata;
		Fl_Input * message = anInstance->distBearIn;
		//Jwp jwp;
		//jwp.writeJson(wps);
		bool hasSaved = ww.saveToJsonFile();
		if(hasSaved) message->value("Json Exported");
	}

	//############# DISTANCE AND BEARING
	static void ClickedDistBear(Fl_Widget * w, void * userdata) {
		// calculates the distance and bearing
		Client* anInstance = (Client*) userdata;
		Fl_Input_Choice * toWps = anInstance->toWps;
		Fl_Input_Choice * frWps = anInstance->frWps;
		Fl_Input * theDandB = anInstance->distBearIn;

		//get the two waypoints
		string sel1(frWps->value());
		//cout << "Waypoint 1: " << sel1 << endl;
		string sel2(toWps->value());
		//cout << "Waypoint 2: " << sel2 << endl;

		int fidx = FetchWPIdx(sel1);
		if (fidx == -1)
			return; //wp not found
		Waypoint twp1 = wps.at(fidx);

		fidx = FetchWPIdx(sel2);
		if (fidx == -1)
			return; //wp not found
		Waypoint twp2 = wps.at(fidx);

		//build vector with the two waypoints and pass to calculators
		vector<Waypoint> dAndBWPs;
		dAndBWPs.push_back(twp1);
		dAndBWPs.push_back(twp2);
		string distance, bearing;
		distance = DtoS(Waypoint::distanceGCTo(dAndBWPs, unitFlag));
		bearing = DtoS(Waypoint::bearingGCInitTo(dAndBWPs));
		//build result to string
		stringstream result;
		result << distance << " / " << bearing << " \u00b0";
		string s = result.str();

		//pass string to display
		theDandB->value(s.c_str());
	}

	//############# SELECT UNITS
	static void SelectedUnits(Fl_Widget * w, void * userdata) {
		/*
		 * Selects the proper unit for conversion
		 * 0 km (default
		 * 1 nautical miles
		 * 2 statute
		 */
		Client* anInstance = (Client*) userdata;
		Fl_Input_Choice * uLab = anInstance->uLab;

		std::string selected(uLab->value());
		if (selected == "Kilometers") {
			unitFlag = 0;
		} else if (selected == "Nautical") {
			unitFlag = 1;
		} else if (selected == "Statute") {
			unitFlag = 2;
		}
	}

	static string DtoS(double num) {
		/*
		 * takes in a string and converts it to a double
		 * with fixed decimal and precision of 4
		 */
		ostringstream streamDtoS;
		streamDtoS << fixed;
		streamDtoS << setprecision(4);
		streamDtoS << num;
		string result = streamDtoS.str();
		return result;
	}

	static int FetchWPIdx(string s) {
		unsigned sz = wps.size();
		for (unsigned i = 0; i < sz; i++) {
			if (wps[i].name == s) {
				return i;
			}
		}
		return -1;
	}
protected:


public:


	//Callbacks needed to implement actions from button presses
	//such as ClickedRemoveWP assigned to action on removeWPButt

	Client(const char * name = 0) :
			WaypointGUI(name) {


		frWps->callback(SelectedFromWP, (void*) this);
		toWps->callback(SelectedToWP, (void*) this);
		removeWPButt->callback(ClickedRemoveWP, (void*) this);
		addWPButt->callback(ClickedAddWP, (void*) this);
		modWPButt->callback(ClickedModWP, (void*) this);
		importJsonButt->callback(ClickedImportJson, (void*) this);
		exportJsonButt->callback(ClickedExportJson, (void*) this);
		distBearButt->callback(ClickedDistBear, (void*) this);
		uLab->callback(SelectedUnits, (void*) this);
		callback(ClickedX);
	}



};

int main(int argc, char *argv[]) {
	//string host = "http://localhost:8080";
	if (argc > 1) {
		host = string(argv[1]);
	}





	Client cm(host.c_str());
	return (Fl::run());

}

