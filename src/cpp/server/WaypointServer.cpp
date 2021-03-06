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
 * @file WaypointServer.cpp
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
 * Purpose: C++ class which serves as server for a collection of students. This
 * class is part of a student collection distributed application that uses JsonRPC.
 * Meant to run on OSX, Debian Linux, and Raspberry Pi Debian.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version July 2016
 */

#include "WaypointServer.hpp"
#include <iostream>
#include <stdlib.h>

WaypointServer::WaypointServer(AbstractServerConnector &connector, int port) :
		waypointserverstub(connector){
	library = new WaypointCollection();
	library->resetFromJsonFile("waypoints.json");
	portNum = port;
}

string WaypointServer::serviceInfo() {
	std::string msg = "Waypoint collection management service.";
	stringstream ss;
	ss << portNum;
	cout << "serviceInfo called:" << endl;
	return msg.append(ss.str());
}

bool WaypointServer::saveToJsonFile() {
	cout<<"saving collection to waypoints.json"<<endl;
	bool ret = library->saveToJsonFile("waypoints.json");
	return ret;
}

bool WaypointServer::resetFromJsonFile() {
	cout << "restoring collection from waypoints.json"<<endl;
	bool result = library->resetFromJsonFile("waypoints.json");
	return result;
}

bool WaypointServer::add(const Json::Value& waypointName) {
	cout << "Adding " << waypointName << endl;
	bool result = library->add(waypointName);
	return result;
}

bool WaypointServer::remove(const string & waypointName) {
	cout<<"Removing " << waypointName << endl;
	bool result = library->remove(waypointName);
	return result;
}

Json::Value WaypointServer::get(const string & waypointName) {
	cout << "Getting "<<waypointName << endl;
	return library->get(waypointName);
}

Json::Value WaypointServer::getNames() {
	Json::FastWriter fw;
	std::string names =  fw.write(library->getNames());
	cout<<"Get names returning: "<<names<<endl;
	return library->getNames();
}




void exiting(){
   std::cout << "Server has been terminated. Exiting normally" << endl;
   //ss.StopListening();
}

int main(int argc, char * argv[]) {
   // invoke with ./bin/waypointRPCServer 8080
   int port = 8080;
   if(argc > 1){
      port = atoi(argv[1]);
   }
   HttpServer httpserver(port);
   WaypointServer ss(httpserver, port);
   std::atexit(exiting);
   auto ex = [] (int i) {cout << "server terminating with signal " << i << endl;
                         // ss.StopListening();
                         exit(0);
                         //return 0;
                        };
   // ^C
   std::signal(SIGINT, ex);
   // abort()
   std::signal(SIGABRT, ex);
   // sent by kill command
   std::signal(SIGTERM, ex);
   // ^Z
   std::signal(SIGTSTP, ex);
   cout << "Waypoint collection server listening on port " << port
      //<< " press return/enter to quit." << endl;
        << " use ps to get pid. To quit: kill -9 pid " << endl;
   ss.StartListening(); // TODO figure out where this is coming from, maybe gen from stub?
   while(true){
   }
   //int c = getchar();
   ss.StopListening();
   return 0;
}






















