# Waypoint

Kenneth Bonilla 2018

Calculates distance of great circle between two waypoints given latitude and longitude

Waypoint clients in cpp and java, with waypoint server in cpp.



Build the servers with build.cpp.client or build.java.client


for custom host and port:

execute cpp server with: ./bin/waypointCppServer port

execute cpp client with: ./bin/waypointCppClient http://host:port

invoke java http client with: java -cp classes: waypoint.client.WaypointGUIDriver host port

If stub generation creates an error:
sudo apt install libjsonrpccpp-tools -y

ant will not compile project if there are any special characters or whitespace in pathname
