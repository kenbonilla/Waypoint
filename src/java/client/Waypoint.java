package waypoint.client;

import java.util.Vector;
import java.io.Serializable;
import org.json.*;

/**
 * Copyright 2018 Kenneth Bonilla,
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
 * 
 * @author Kenneth Bonilla kfbonill@asu.edu
 * @version January, 2018
 **/

public class Waypoint extends Object implements Serializable{
	

    /**
     * 
     */
    private static final long serialVersionUID = 1L;
    //magic numbers
    private final static int STATUTE = 0;
    private final static int NAUTICAL = 1;
    private final static int KMETER = 2;
    private final static double RADIUSEARTH = 6371; //radius of earth in KM
    private final static double TOSTATUTE = 0.62137119; //convert km to statute
    private final static double TONAUTICAL = 0.5399568; //convert km to nautical
    
    //Latitude and longitude in DD.D format
    private double lat;      // + north, - south
    private double lon;      // + east, - west
    private double ele;      //elevation in feet MSL (Mean sea level)
    private String name;     //waypoint name
    private String address;  //wapypoint address
    
    
    /**
     * 
     * @param lat       latitude in DD.D format + north, - south
     * @param lon       longitude in DD.D format + east, - west
     * @param ele       elevation in feet MSL (Mean sea level)
     * @param name      name of waypoint e.x. Paris
     */
    public Waypoint(double lat, double lon, double ele, String name, String address) {
        this.lat = lat;
        this.lon = lon;
        this.ele = ele;
        this.name = name;
        this.address = address;
    }
    
    public Waypoint() {
        this.lat = 0;
        this.lon = 0;
        this.ele = 0;
        this.name = "invalid";
        this.address = "invalid";
    }
    
    public Waypoint(JSONObject obj) {
        this.lat = obj.getDouble("lat");
        this.lon = obj.getDouble("lon");
        this.ele = obj.getDouble("ele");
        this.name = obj.getString("name");
        this.address = obj.getString("address");
        
    }
    
    public Waypoint(Waypoint w) {
        lat = w.getLat();
        lon = w.getLon();
        ele = w.getEle();
        name = w.getName();
        address = w.getAddress();
    }
    
    
    // GETTERS AND SETTERS FOLLOW
    
    

    /**
     * @return the lat
     */
    public double getLat() {
        return lat;
    }



    /**
     * @param lat the lat to set
     */
    public void setLat(double lat) {
        this.lat = lat;
    }




    /**
     * @return the lon
     */
    public double getLon() {
        return lon;
    }



    /**
     * @param lon the lon to set
     */
    public void setLon(double lon) {
        this.lon = lon;
    }



    /**
     * @return the ele
     */
    public double getEle() {
        return ele;
    }



    /**
     * @param ele the ele to set
     */
    public void setEle(double ele) {
        this.ele = ele;
    }



    /**
     * @return the name
     */
    public String getName() {
        return name;
    }



    /**
     * @param name the name to set
     */
    public void setName(String name) {
        this.name = name;
    }

    

    // END GETTERS AND SETTERS
    
    /**
     * @return the address
     */
    public String getAddress() {
        return address;
    }




    /**
     * @param address the address to set
     */
    public void setAddress(String address) {
        this.address = address;
    }




    public void print () {
        System.out.println("Waypoint "+name+": lat "+lat+" lon "+lon+
                           " elevation "+ele);
     }


    public JSONObject toJSONObject() {
        JSONObject obj = new JSONObject();
        obj.put("lat", lat);
        obj.put("lon", lon);
        obj.put("address", address);
        obj.put("name", name);
        obj.put("ele", ele);
        
        return obj;
    }
    
    
    public static double distanceGCTo(Vector<Waypoint> wp, int scale) {
        
        
        /* Haversine formula
         * 
         * - http://www.movable-type.co.uk/scripts/latlong.html
         * 
         * a = sin²(Δφ/2) + cos φ1 ⋅ cos φ2 ⋅ sin²(Δλ/2) 
         * c = 2 ⋅ atan2( √a, √(1−a) )   
         * d = R ⋅ c
         * where φ is latitude, λ is longitude, R is earth’s radius 
         * 
         * latitude must be in Radians
         */
        int vSize = wp.size();
        //System.out.println("VSIZE = " + vSize);
        if (vSize < 2) return 0;
        double startLat = wp.elementAt(0).getLat();
        double endLat = wp.elementAt(1).getLat();
        double startLon = wp.elementAt(0).getLon();
        double endLon = wp.elementAt(1).getLon();
        
        double radLat1 = Math.toRadians(startLat);
        double radLat2 = Math.toRadians(endLat);
        double radLat = Math.toRadians(endLat - startLat);
        double radLon = Math.toRadians(endLon - startLon);
        
        double a = Math.pow(Math.sin(radLat/2), 2);
        a += Math.cos(radLat1) * Math.cos(radLat2) * Math.pow(Math.sin(radLon/2), 2);
        double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
        double result = RADIUSEARTH * c;
        
        // convert the results into appropriate scale
        // default is KM
        switch(scale) {
        case STATUTE:
            result *= TOSTATUTE;
            break;
        case NAUTICAL:
            result *= TONAUTICAL;
            break;
        case KMETER:
            break;
        default:
            break;
        }
        
        return result;
    }
    
    
    
    public static double bearingGCInintTo(Vector<Waypoint> wp) {
        
        /* Bearing formula
         * 
         * - http://www.movable-type.co.uk/scripts/latlong.html
         *  
         * Formula:    θ = atan2( sin Δλ ⋅ cos φ2 , cos φ1 ⋅ sin φ2 − sin φ1 ⋅ cos φ2 ⋅ cos Δλ )
         * where   φ1,λ1 is the start point, φ2,λ2 the end point (Δλ is the difference in longitude)
         * 
         * ATAN2(COS(lat1)*SIN(lat2)-SIN(lat1)*COS(lat2)*COS(lon2-lon1), 
         * SIN(lon2-lon1)*COS(lat2))
         */
        int vSize = wp.size();
        if (vSize < 2) return 0;
        double lat1 = wp.elementAt(0).getLat();
        double lat2 = wp.elementAt(1).getLat();
        double lon1 = wp.elementAt(0).getLon();
        double lon2 = wp.elementAt(1).getLon();
        
        double x = Math.cos(lat1) * Math.sin(lat2) - Math.sin(lat1) * Math.cos(lat2) * Math.cos(lon2 - lon1);
        double y = Math.sin(lon2 - lon1) * Math.cos(lat2);
        double result = Math.toDegrees(Math.atan2(y, x));
        if(result < 0) {
            result *= -1;
            result += 180;
        }
        
        return result;
    }
    
    
}
