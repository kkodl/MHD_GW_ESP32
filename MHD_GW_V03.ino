
/*
Prague PID (Public Transport System) departure informations extract and import to "Zivy Obraz" https://zivyobraz.eu/
Proof of concept. No error handling. Use at your own risk.
Use ArduinoJson.h library V6 and higher.
>> Caution - See the calculation of JSON buffer size. If you overload it, it crashes <<
It can be overloaded by asking data about too many trains into the future.
For more details, see https://arduinojson.org/v6/assistant/#/step1
Have fun!
Karel Kotrba 
*/

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "setup.h"

char json[6000];                  // buffer response from golemio into char array
String payload;                   //store for the golemio direct response

  // Inside the brackets, 6000 is the capacity of the memory pool in bytes.
  // Don't forget to change this value to match your JSON document.
  // Use arduinojson.org/v6/assistant to compute the capacity.

StaticJsonDocument<8192> jsonBuffer;    // place for deserialization and data extraction

WiFiClient wifi;
HTTPClient http;

int train_count;        // how many vehicles into the future we are asking i.e. 4 = next four vehicles leaving
int Stop_Number;        // our own index of stop i.e. if we ask for data from 3 stops, we must somehow label the values in Zivy Obraz
                        // so this is label index for stops

String Number_Of_Trips;   // in http querry it has to be as a string
String Stop_Ids;          // PID stop code storage, see http://data.pid.cz/stops/xml/StopsByName.xml
String Name;              // string to store name of value for Zivy Obraz

long HttpGetStartMillis;  // timer



void setup() {
  Serial.begin(115200);
  delay (10);
  connectWifi();
  HttpGetStartMillis = millis() + 1000 * HTTP_GET_INTERVAL_SECS;    // fake the timer, we ask immediately after boot
}


void loop() {
  if (millis() - HttpGetStartMillis > 1000 * HTTP_GET_INTERVAL_SECS) {    // time to update?
    Serial.println ("Time to get data!");
    HttpGetStartMillis = millis();
    connectWifi();

    Number_Of_Trips = "3";        // pocet budoucich spoju, na ktere se u zastavky ptame
    Stop_Ids = "ids=_ZDE KOD PRO ZASTAVKU #1_";     // code of stop, see readme
    Stop_Number = 1;              // index of Stop in Zivy Obraz
    Request_Train();              // ask for data and push it


    Number_Of_Trips = "3";        // pocet budoucich spoju, na ktere se u zastavky ptame
    Stop_Ids = "ids=_ZDE KOD PRO ZASTAVKU #2_";     // code of stop, see readme
    Stop_Number = 2;              // index of Stop in Zivy Obraz
    Request_Train();              // ask for data a push it


    Number_Of_Trips = "3";        // pocet budoucich spoju, na ktere se u zastavky ptame
    Stop_Ids = "ids=_ZDE KOD PRO ZASTAVKU #3_";     // code of stop, see readme
    Stop_Number = 3;              // index of Stop in Zivy Obraz
    Request_Train();              // ask for data a push it

    Serial.println("--------- Done for the moment ------------------");
 }
}


void Request_Train(){
  train_count = Number_Of_Trips.toInt();
  Serial.println("GET request to PID Golemio server");
  String serverName = "https://api.golemio.cz/v2/pid/departureboards?" + Stop_Ids + "&total=" + Number_Of_Trips + "&preferredTimezone=Europe%2FPrague";  // server address, must include querry
  Serial.println(serverName);
  http.begin(serverName.c_str());
  http.addHeader("Content-Type", "application/json; charset=utf-8");
  http.addHeader("X-Access-Token", auth_token);
  // Send HTTP GET request
  int httpResponseCode = http.GET();
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    payload.toCharArray(json, payload.length()+1);
    //Serial.println(json);
   }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
   }
  // Free resources
  http.end();


  // Deserialize the JSON document
DeserializationError error = deserializeJson(jsonBuffer, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

//Extract name of stop from Golemio JSON response and print it to console
JsonObject forecast = jsonBuffer["stops"][0];     // it's first member of array, extract that one into smaller JSON
const char* sensor = forecast["stop_name"];       // let's extract the name of stop (there's a lot more about the stop in this JSON)
Serial.print("Zastávka: ");
Serial.println(sensor);

//Prepare value name/index for Ž.O. and push it together with value in to Ž.O. server
Name = "Zastavka_" + String(Stop_Number) +"_Name";
Push_Data_To_LiPi(Name, sensor);

Serial.print ("Počet příštích spojů: ");  // How many next depatures we aim for - it's time determined, even for cluster of stops 
Serial.println(train_count);              // These are all already inside the JSON from Golemio
Serial.println(); // newline to console

for (int i=0; i<train_count; i++) {       // extract necessary data for one departure after another
forecast = jsonBuffer["departures"][i];   // get the JSON field member for specific departure 

  sensor = forecast["arrival_timestamp"]["predicted"];    // extract predicted time of arrival
  String temp_time_string = String(sensor);
  int delimiter = temp_time_string.indexOf("T");
  String arr_time = temp_time_string.substring(delimiter + 1, delimiter + 6);   // parse just the hour and minute from complete date&time record
  Serial.println(arr_time);
  Name = "Z" + String(Stop_Number) + "_Poradi_" + String(i) + "_Odjezd";        // compose the value label/name for Ž.O.
  Push_Data_To_LiPi(Name, arr_time);                                            // and push it to Ž.O.

  Serial.print("Linka: ");
  sensor = forecast["route"]["short_name"];                                     // parse the line name i.e. line numbering
  Serial.println(sensor);
  Name = "Z" + String(Stop_Number) + "_Poradi_" + String(i) + "_Linka";         // compose the value label/name for Ž.O.
  Push_Data_To_LiPi(Name, sensor);                                              // and push it to Ž.O.
  
  Serial.print("Směr  ");
  sensor = forecast["trip"]["headsign"];        // parse the direction - actually this is a vehicle headsign, in case it goes to depot or is derouted
  Serial.println(sensor);
  Name = "Z" + String(Stop_Number) + "_Poradi_" + String(i) + "_Smer";          // compose the value label/name for Ž.O.
  Push_Data_To_LiPi(Name, sensor);                                              // and push it to Ž.O.
  
  Serial.println();
  Serial.println();
 }
}


// ----------------------------- Connects to Wifi ---------------------------
// if connected, it does nothing
//
void connectWifi() {
  if (WiFi.status() == WL_CONNECTED) return;
  delay(500);
  Serial.print("\nConnecting to WiFi ");
  //Serial.print(WIFI_SSID);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.hostname("MHD_GW");
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
    }
  Serial.println("Connected");
}


// ---------- Pushes the data passed to it towards Živý Obraz server
// 
void Push_Data_To_LiPi(String DataName, String DataValue){
  DataValue.replace(" ","%20");           // replace spaces in names, to have clean http request
  HTTPClient http;
  String serverPath = Server_Name + Server_Key + "&" + DataName + "=" + DataValue;    // compose URL for Ž.O. server
  Serial.print ("Pushing to Ž.O.: ");
  Serial.print(serverPath);
  http.begin(wifi, serverPath.c_str());
  int httpResponseCode = http.GET();      // Send HTTP GET request
  if (httpResponseCode>0) {
    Serial.print(" >> Response: ");
    Serial.print(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
    } else {
      Serial.print("Err code: ");
      Serial.println(httpResponseCode);
     }
  http.end();         // Free resources
  Serial.println();   // newline to console
  delay (1000);       //most probbably not needed, just not to piss off the server on LiPi side
}
