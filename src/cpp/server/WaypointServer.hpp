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
 * @file WaypointServer.hpp
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

#ifndef WAYPOINTSERVER_HPP_
#define WAYPOINTSERVER_HPP_


#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <jsoncpp/json/json.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <csignal>

#include "waypointserverstub.h"
#include "WaypointCollection.hpp"

using namespace jsonrpc;
using namespace std;

class WaypointServer: public waypointserverstub {
public:
	WaypointServer(AbstractServerConnector &connector, int port);
	virtual std::string serviceInfo();
  //virtual bool getAll();
	virtual bool saveToJsonFile();
	virtual bool resetFromJsonFile();
	virtual bool add(const Json::Value& waypointName);
	virtual bool remove(const std::string& waypointName);
	virtual Json::Value get(const std::string& waypointName);
	virtual Json::Value getNames();
	

private:
	WaypointCollection * library;
	int portNum;
};


#endif /* WAYPOINTSERVER_HPP_ */
