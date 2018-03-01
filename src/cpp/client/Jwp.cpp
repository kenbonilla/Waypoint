
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
 * Handles json implementation for waypoints using jsoncpp
 * <p>
 * @author Kenneth Bonilla kfbonill@asu.edu
 * @version January, 2018
 **/





#include"Jwp.hpp"
#include <vector>
#include <jsonrpccpp/client/connectors/httpclient.h>


using namespace std;

	Jwp::Jwp(){
	}

	Jwp::~Jwp(){
	}

	std::vector<Waypoint> Jwp::readJson(){
		//assumes that json is properly formed
		//undetermined values if bad json supplied
		//open file and init json functions
		ifstream ifs("waypoints.json");

		Json::Reader reader;
		Json::Value root;
		reader.parse(ifs, root);

		//iterate through top level and push wp onto Json value vector
		for (auto const& name : root.getMemberNames()) {
			for(auto const& place : root[name]){
				jVec.push_back(place);
			}
		}
		//convert json to wp and push to stack
		for(unsigned int i = 0; i < jVec.size();){
			const string ad = jVec[i].asString();
			i++;
			const string el = jVec[i].asString();
			double del =  atof(el.c_str());
			i++;
			const string la = jVec[i].asString();
			double dla =  atof(la.c_str());
			i++;
			const string lo = jVec[i].asString();
			double dlo =  atof(lo.c_str());
			i++;
			const string na = jVec[i].asCString();
			i++;
			Waypoint twp;
			twp.setValues(dla, dlo, del, na, ad);
			theWps.push_back(twp);
			//cout<<na<<" - pushed to stack"<<endl;
		}

		ifs.close();
		return theWps;
	}


	std::vector<Waypoint> Jwp::readJson(string host){
		jsonrpc::HttpClient httpclient(host);
		waypointcollectionstub ww(httpclient);
		Json::Value names_ = ww.getNames();
		Json::FastWriter fastWriter;
		std::string output = fastWriter.write(names_); //puts json of names to string
		//capture words inside of ""
		std::regex rgx("\"([^\"]*)\""); // will "capture words" in "quotes" --> "capture words", "quotes"
		auto words_begin = std::sregex_iterator(output.begin(), output.end(), rgx);
		auto words_end = std::sregex_iterator();
		vector<string> theNames;

		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			std::smatch match = *i;
			std::string match_str = match.str();
			match_str = match_str.substr(1, match_str.size() - 2);
			//regex didn't strip "" for some reason? - remove first and last character
			//cout << match_str << "\n"; //this was for debugging
			theNames.push_back(match_str);
		}
		for (auto i = theNames.begin(); i != theNames.end(); ++i){
			//std::cout << ww.get(*i) << endl; //this was for debugging
			Json::Value x = ww.get(*i);
			/*cout << "for " << x.get("name", "invalid_name").asString() << " the ele is " <<
					x.get("ele", 0).asString()<<endl;*/ //just testing the loop
			double ele_ = x.get("ele", 0).asDouble();
			double lat_ = x.get("lat", 0).asDouble();
			double lon_ = x.get("lon", 0).asDouble();
			string name_ = x.get("name", "NoName").asString();
			string addr_ = x.get("address", "").asString();
			Waypoint twp;
			twp.setValues(lat_, lon_, ele_, name_, addr_);
			theWps.push_back(twp);
		}
		return theWps;
	}

	void Jwp::writeJson(vector<Waypoint> wps) {
		//output wp vector to styled json
		Json::Value root;
		Json::StyledWriter writer;
		for(unsigned i = 0; i < wps.size(); i++) {
			Json::Value inwp;
			inwp["name"] = wps[i].name;
			inwp["address"] = wps[i].addr;
			inwp["ele"] = wps[i].ele;
			inwp["lat"] = wps[i].lat;
			inwp["lon"] = wps[i].lon;
			root[wps[i].name] = inwp;
		}
		ofstream out("waypoints.json");
		const string result = writer.write(root);
		out << result;
		out.close();

	}

