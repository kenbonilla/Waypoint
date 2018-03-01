package waypoint.client;

import org.json.*;
import javax.swing.*;
import java.io.*;
import java.math.RoundingMode;
import java.text.DecimalFormat;

import javax.swing.event.*;
import javax.swing.text.html.*;
import java.awt.event.*;
import java.awt.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;
import java.util.zip.GZIPInputStream;

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
 * Purpose: Java client UI for Waypoint management.
 * This class creates Gui components for a UI to manage waypoints.
 * This software is meant to run on OSX, and Windows Cygwin using g++.
 * WaypointGUIDriver demonstrates use of the WaypointGUI class
 * for solving ser321 assignments.
 * WaypointGUI class uses Swing components JTextField, JComboBox, and
 * JTextArea to realize a GUI for Waypoint management. 
 * <p/>
 * Ser321 Principles of Distributed Software Systems.
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering
 *                       Ira Fulton Schools of Engineering, ASU Polytechnic
 * @file    WaypointGUI.java
 * @date    January, 2018
 * @license See above
 */

/**
 * Copyright 2018 Kenneth Bonilla,
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 * 
 * 
 * @author Kenneth Bonilla kfbonill@asu.edu
 * @version January, 2018
 **/

public class WaypointGUIDriver extends WaypointGUI implements ActionListener, ItemListener {

    private static final boolean debugOn = true;
    private Jwp jwp = new Jwp();
    private int fort = -1;
    private int mmod = -1;
    //private JFileChooser jfc = new JFileChooser(); //not needed for remote files
    private boolean points = false; //false when no wps, true after wp added or imported
    public JsonRpcRequestViaHttp server;
    public String serviceURL;
    public URL jsonURL;
    public static int id = 0;
    //public HttpURLConnection server;
    //TODO refactor wp vector to separate class library
    public WaypointGUIDriver(String base, String serviceURL) throws MalformedURLException {
        super(base);
        try {
            this.server = new JsonRpcRequestViaHttp(new URL(serviceURL));
            jsonURL = new URL(serviceURL);
            
        } catch (Exception ex) {
            System.out.println("Malformed URL " + ex.getMessage());
        }
        
        removeWPButt.addActionListener(this);
        addWPButt.addActionListener(this);
        modWPButt.addActionListener(this);
        importButt.addActionListener(this);
        exportButt.addActionListener(this);
        distBearButt.addActionListener(this);
        frWps.addItemListener(this);
        toWps.addItemListener(this);

        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                debug("you clicked X");
                System.exit(0);
            }
        });

        setVisible(true);
    }
    private String callServer(String method, String args) {
        JSONObject obj = new JSONObject();
        obj.put("jsonrpc", "2.0");
        obj.put("method",method);
        obj.put("id", ++id);
        String almost = obj.toString();
        String toInsert = ",\"params\":[" + args + "]";
        String begin = almost.substring(0,almost.length()-1);
        String end = almost.substring(almost.length()-1);
        String ret = begin + toInsert + end;
        return ret;
    }
    
    public String getWP(String name) {
        System.out.println("Getting waypoint for " + name);
        String result = "";
        try {
            String jsonStr = this.callServer("get", name);
            //System.out.println("sending: " + jsonStr);
            String resString = server.call(jsonStr);
            //System.out.println("got back: "+resString);
            
            JSONObject res = new JSONObject(resString);
            result = res.optString("result");
            
        }catch(Exception ex){
            System.out.println("exception in rpc call to plus: "+ex.getMessage());
        }
        return result;
    }
    public String getNames() {
        System.out.println("Getting names...");
        String result = null;
        try {
            String jsonStr = this.callServer("getNames", "");
            //System.out.println("sending: " + jsonStr);
            String resString = server.call(jsonStr);
            //System.out.println("got back: "+resString);
            JSONObject res = new JSONObject(resString);
            result = res.optString("result");
        }catch(Exception ex){
            System.out.println("exception in rpc call to plus: "+ex.getMessage());
        }
        return result;
        
    }
    
    public void cleanSlate() {
        System.out.println("Clearing GUI");
        jwp.clearWPs();
        frWps.removeAllItems();
        toWps.removeAllItems();
        latIn.setText("");
        lonIn.setText("");
        eleIn.setText("");
        nameIn.setText("");
        addrIn.setText("");
    }
    
    public String removeWP(String name) {
        System.out.println("Remove: " + name);
        String result = "";
        String nname = "\"" + name + "\"";
        try {
            String jsonStr = this.callServer("remove", nname);
            //System.out.println("sending: " + jsonStr);
            String resString = server.call(jsonStr);
            //System.out.println("got back: "+resString);
            result = resString;
        }catch(Exception ex){
            System.out.println("exception in rpc call to plus: "+ex.getMessage());
        }
        return result;
    }
    
    public String addWP(String jsonWP) {
        System.out.println("adding wp: " + jsonWP);
        String result = "";
        try {
            String jsonStr = this.callServer("add", jsonWP);
            //System.out.println("sending: " + jsonStr);
            String resString = server.call(jsonStr);
            //System.out.println("got back: "+resString);
            result = resString;
        }catch(Exception ex){
            System.out.println("exception in rpc call to plus: "+ex.getMessage());
        }
        return result;   
    }
    
    public void saveLibrary() {
        try {
            String jsonStr = this.callServer("saveToJsonFile", "");
            System.out.println("sending: " + jsonStr);
            String resString = server.call(jsonStr);
            System.out.println("got back: "+resString);
        }catch(Exception ex){
            System.out.println("exception in rpc call to plus: "+ex.getMessage());
        }
    }

    //////////////////////////////////////////////////
    /////////////////////////////////////////////////

    public void itemStateChanged(ItemEvent event) {
        if (event.getStateChange() == ItemEvent.SELECTED) {
            Object comp = event.getSource();
            debug("Selection event generated by " + ((comp == frWps) ? "from " : "to ") + "combobox. "
                    + "Selected waypoint is: " + (String) event.getItem());
            // update textfields when selection new
            int x = 0;
            if (comp == frWps) {
                x = frWps.getSelectedIndex();
                fort = 0;
                mmod = 0;
                // System.out.println("update from");
            } else if (comp == toWps) {
                x = toWps.getSelectedIndex();
                fort = 1;
                mmod = 1;
                // System.out.println("update to");
            }
            // else if (comp == units) {
            // uc = frWps.getSelectedIndex();
            // }

            Waypoint tempWP = new Waypoint(jwp.getWaypoint(x));
            latIn.setText(Double.toString(tempWP.getLat()));
            lonIn.setText(Double.toString(tempWP.getLon()));
            eleIn.setText(Double.toString(tempWP.getEle()));
            nameIn.setText(tempWP.getName());
            addrIn.setText(tempWP.getAddress());

        }
    }

    //////////////////////////////////////////////////
    /////////////////////////////////////////////////

    @SuppressWarnings("unchecked")
    public void actionPerformed(ActionEvent e) {
        if (e.getActionCommand().equals("Remove")) {
            debug("you clicked Remove Waypoint");
            String name = nameIn.getText();
            removeWP(name);
            int i = 0;
            int a = frWps.getSelectedIndex();
            int b = toWps.getSelectedIndex();

            if (mmod == 0) {
                i = a;
                frWps.removeItemAt(a);
                toWps.removeItemAt(a);
            }
            if (mmod == 1) {
                i = b;
                frWps.removeItemAt(b);
                toWps.removeItemAt(b);
            }
            jwp.removeWP(i);

        } else if (e.getActionCommand().equals("Add")) {
            debug("you clicked Add Waypoint");

            if (points == false) {
                frWps.removeItemAt(0);
                toWps.removeItemAt(0);
                points = true;
            }
            // distBearIn.setText("Added: "+nameIn.getText());

            double lat = Double.parseDouble(latIn.getText());
            double lon = Double.parseDouble(lonIn.getText());
            double ele = Double.parseDouble(eleIn.getText());
            String name = nameIn.getText();
            String adr = addrIn.getText();
            Waypoint twp = new Waypoint();
            String makePretty = 
                    "{\"address\": \"" + adr +"\",\"name\": \"" + name + "\",\"lon\": " + lon + 
                    ",\"lat\": " + lat + ",\"ele\": " + ele +"}";
            addWP(makePretty);
            twp.setLat(lat);
            twp.setLon(lon);
            twp.setEle(ele);
            twp.setName(name);
            twp.setAddress(adr);

            jwp.addWP(twp);

            

            frWps.addItem(name);
            toWps.addItem(name);

            mmod = -1;

        } else if (e.getActionCommand().equals("Modify")) {
            debug("you clicked Modify Waypoint");
            int x = mmod;
            if (mmod > -1) {
                double lat = Double.parseDouble(latIn.getText());
                double lon = Double.parseDouble(lonIn.getText());
                double ele = Double.parseDouble(eleIn.getText());
                String name = nameIn.getText();
                String adr = addrIn.getText();
                Waypoint twp = new Waypoint();
                twp.setLat(lat);
                twp.setLon(lon);
                twp.setEle(ele);
                twp.setName(name);
                twp.setAddress(adr);

                // replace the obj at vector index
                jwp.setWP(twp, x);
                removeWP(name);
                String makePretty = 
                        "{\"address\": \"" + adr +"\",\"name\": \"" + name + "\",\"lon\": " + lon + 
                        ",\"lat\": " + lat + ",\"ele\": " + ele +"}";
                addWP(makePretty);
            }

        } else if (e.getActionCommand().equals("Import")) {
            debug("you clicked Import Json Library");
            //out with the old
            cleanSlate();
            points = true;
            //in with the new
            String result = getNames();
            //System.out.println(result);
            Pattern p = Pattern.compile("\"([^\"]*)\""); //fetches words within parentheses 
            Matcher m = p.matcher(result);
            Vector<String> theNames=new Vector<String>();
            while(m.find()) {
                theNames.add(m.group(1)); 
            }
            for(int i=0; i< theNames.size(); i++) {
                //System.out.println(theNames.get(i));
                String newName = theNames.get(i);
                String makePretty = "\"" + newName +"\""; //makes string compatible with getWP()
                //System.out.println(getWP(makePretty));
                String jsonStr = getWP(makePretty);
                JSONObject jsonObj = new JSONObject(jsonStr);
                //System.out.println(jsonObj.toString());
                //System.out.println("");
                Waypoint twp = new Waypoint(jsonObj);
                jwp.addWP(twp);
                toWps.addItem(newName);
                frWps.addItem(newName);
                
            }

        } else if (e.getActionCommand().equals("Export")) {
            debug("you clicked Export Json Library");
            String result = jwp.toJSONString();
            //System.out.println(result);
            JSONObject toSend = new JSONObject(result);
            System.out.println(toSend.toString());

        } else if (e.getActionCommand().equals("Distance")) {
            debug("you clicked Distance and Bearing");

            if (jwp.getLen() > 1) {
                // debug("passed getLen");
                int x;
                int y;

                x = frWps.getSelectedIndex();
                Waypoint w1 = new Waypoint(jwp.getWaypoint(x));
                x = toWps.getSelectedIndex();
                Waypoint w2 = new Waypoint(jwp.getWaypoint(x));
                // debug("passed w1 w2");

                Vector<Waypoint> wp = new Vector<Waypoint>();
                wp.add(w1);
                wp.add(w2);

                y = units.getSelectedIndex();
                double distance = Waypoint.distanceGCTo(wp, y);
                double bearing = Waypoint.bearingGCInintTo(wp);

                // Prettify the decimals
                DecimalFormat df = new DecimalFormat("#.####");
                df.setRoundingMode(RoundingMode.CEILING);
                String dstring = df.format(distance);
                String bstring = df.format(bearing);

                String dAndB = (dstring + " / " + bstring + " \u00b0");

                distBearIn.setText(dAndB);

            }

        }
    }

    //////////////////////////////////////////////////
    /////////////////////////////////////////////////

    private void debug(String message) {
        if (debugOn)
            System.out.println("debug: " + message);
    }

    public static void main(String args[]) {
            String name = "Ser321 kfbonill";
            String host = "localhost"; 
            String url = "";       
            try {

            String port = "8080";
            if (args.length >= 2) {
                host = args[0];
                
                port = args[1];
            }
            url = "http://" + host + ":" + port;
            System.out.println(host);
            System.out.println("Opening connection to: " + url);
            WaypointGUIDriver sa2 = new WaypointGUIDriver(name, url);

            // File file = new File("waypoints.json");

        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}
