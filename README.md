# Waypoint

Kenneth Bonilla 2018

Waypoint clients in cpp and java, with waypoint server in cpp.


Targets are clean, prepare, build.cpp.server, build.cpp.client, build.java.client, 
execute.cpp.server, execute.java.client, execute.cpp.client, c50, j50, targets


Build the servers with build.cpp.client or build.java.client
Will build with default values of localhost:8080
j50 and c50 are client launchers hardcoded to 192.168.0.50:8080

for custom host and port
execute cpp server with: ./bin/waypointCppServer port
execute cpp client with: ./bin/waypointCppClient http://host:port
invoke java http client with: java -cp classes: waypoint.client.WaypointGUIDriver host port
If stub generation creates an error:
sudo apt install libjsonrpccpp-tools -y

ant will not compile project if there are any special characters or whitespace in pathname
