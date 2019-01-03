
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "MAX17043.h"
#include "Wire.h"
#include "SSD1306Wire.h"

SSD1306Wire  display(0x3c,4,5);
MAX17043 batteryMonitor;
char tmp[8];
String tempIP = "";
float cellVoltage;
float stateOfCharge;


// web

String myhtmlPage =
    String("") +
    "<html>" +
    "<head>" +
    "    <title>DSTIKE Read Battery Test</title>" +
    "    <script defer=\"defer\">" +
    "        function ReadSwitch() {" +
    "            var xmlhttp;" +
    "            if (window.XMLHttpRequest) {" +
    "                xmlhttp = new XMLHttpRequest();" +
    "            } else {" +
    "                xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");" +
    "            }" +
    "            xmlhttp.onreadystatechange = function () {" +
    "                if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {" +
    "                    document.getElementById(\"txtState\").innerHTML = xmlhttp.responseText;" +
    "                }" +
    "            }," +
    "            xmlhttp.open(\"GET\", \"Switch\", true);" +
    "            xmlhttp.send(); " +
    "        }" +
    "    </script>" +
    "</head>" +
    "<body>" +
    "<center>"+
    "<h1>DSTIKE Battery Gague</h1>"
    "<h2>www.dstike.com</h2>"
    "    <div id=\"txtState\">Read Battery</div>" +
    "    <input type=\"button\" value=\"Read\" onclick=\"ReadSwitch()\">" +
    "</body>" +
    "</html>";
bool readbattery = false; 




const char* ssid = "xxxxxx";
const char* password = "xxxxxx";





ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", myhtmlPage); 
  Serial.println("User acessed");
}

void handleSwitch() {

  //dtostrf(stateOfCharge,4,1,tmp), dtostrf(cellVoltage,5,2,tmp)
  
dtostrf(stateOfCharge,4,1,tmp);
String batteryone= tmp;
dtostrf(cellVoltage,5,2,tmp);
String batterytwo = tmp;
String batterylife=batteryone+"%"+batterytwo+"V";


  if (readbattery == false) {
    server.send(200, "text/plain", batterylife );
    Serial.println("User read requested");
    readbattery = true;
  }
  else {
    server.send(200, "text/plain", batterylife);
    Serial.println("User read requested");
    readbattery = false;
  }                      
}
void handleNotFound() {
  server.send(404, "text/plain", "404");
  Serial.println("User acess failed");
}
void setup() {
  
display.init();
display.flipScreenVertically();
display.setFont(ArialMT_Plain_16);
display.setTextAlignment(TEXT_ALIGN_LEFT);

Wire.begin();

batteryMonitor.reset();
batteryMonitor.quickStart();
delay(1000);
 
Serial.begin(115200);
Serial.println();
Serial.println();


WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
Serial.println("");
Serial.print("IP Address: ");
Serial.println(WiFi.localIP());
tempIP =WiFi.localIP().toString();


server.on("/", handleRoot);
server.on("/Switch", handleSwitch);
server.onNotFound(handleNotFound);
server.begin();
Serial.println("HTTP server started");




}

void loop() {

display.clear();
  
  cellVoltage = batteryMonitor.getVCell();
  stateOfCharge = batteryMonitor.getSoC();
  
  display.drawString(0, 0, "DSTIKE ");
  display.drawString(0, 16, "LiPo: ");
  display.drawString(40, 16, dtostrf(stateOfCharge,4,1,tmp));
  display.drawString(90, 16, "%");

  display.drawString(0, 32, dtostrf(cellVoltage,5,2,tmp));
  display.drawString(40, 32, "V");
  display.drawString(0, 48, tempIP);
 
  display.display();
  delay(500);
  server.handleClient();
  
}
