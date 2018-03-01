package waypoint.client;

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

import org.json.*;
import java.io.*;
import java.util.*;
import java.net.URI;
import java.net.URL;

public class Jwp extends Object implements JSONString, Serializable{

    private String name;
    private Vector<Waypoint> wps = new Vector<Waypoint>();
    private String [] names;
    //private Vector<String> names = new Vector<String>;
    
    @Override
    public String toJSONString() {
        // TODO Auto-generated method stub
        System.out.println("\n\n " + wps.size()+"\n\n " );
        String ret;
        JSONObject obj = new JSONObject();
        obj.put("name",name);
        for (Enumeration<Waypoint> e = wps.elements(); e.hasMoreElements();){
           Waypoint way = (Waypoint)e.nextElement();
           obj.put(way.getName(),way.toJSONObject());
        }
        ret = obj.toString();
        //System.out.println("group tojsonstring returning string: "+ret);
        return ret;
    }
    

    
    public Jwp() {
        this.name = "unkown";
    }
    
    public Jwp(URL url) {
        
    }
    
    public Jwp(File filename) {
        try {
            FileInputStream in = new FileInputStream(filename);
            JSONObject obj = new JSONObject(new JSONTokener(in));
            names = JSONObject.getNames(obj);
            System.out.println("Names are: ");
            for(int j=0; j< names.length; j++){
                System.out.println(names[j]+", ");
             }
            
            wps = new Vector<Waypoint>();
            int k = 0;
            for (int i = 0; i < names.length; i++) {
                
                Waypoint awp = new Waypoint((JSONObject) obj.getJSONObject(names[i]));
                wps.add(awp);
                //System.out.println("point added");
                k++;
            }
            System.out.println("loaded " + k +" points");
            in.close();
        }
        catch(Exception ex) {
            System.out.println("JWP CREATE CATCH" + ex.getMessage());
        }
    }
    
    public String [] getNames() {
        return names; 
    }
    
    public Waypoint getWaypoint(int i) {
        return (wps.get(i));
    }
    
    public int getLen() {
        
        return wps.size() ;
    }
    
    public void setWP(Waypoint wp, int i) {
        wps.set(i, wp);
    }
    
    public void removeWP(int i) {
        wps.remove(i);
    }
    
    public void addWP(Waypoint twp) {
        wps.add(twp);
    }
    
    public void clearWPs() {
        wps.clear();
    }
}
