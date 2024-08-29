/************************* WiFi Connection Parameters ***********************************/
#define WIFI_SSID       "XXXXXXXXXXXXX"            // wifi ssid
#define WIFI_PASS       "XXXXXXXXXXXXXXXXXXXXX"    // wifi password
#define WIFI_HOSTNAME   "MHD_GW"                   // name of module
#define WIFI_ATTEMPTS   15                         // how many times x 3s we should try to connect to wifi

// Fixed IP addresses in case no DHCP is used
IPAddress local_IP(192, 168, 1, 68);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192,168,1,1);

bool Use_DNS = false;                               // IP dynamic (true) or fixed (false)


/************************* Update Interval ************************************************/
#define HTTP_GET_INTERVAL_SECS 180        //interval in secs to load data to Zivy Obraz, do not put less than 60s otherwise "Zivy Obraz" would throttle (!)


/************************* Server URL and tokens ******************************************/
// PID Golemio server token here - see https://api.golemio.cz/api-keys/auth/sign-in to get yours
String auth_token = "_enter Golemio auth token_";

// Zivy obraz parameters
String Server_Name = "http://in.zivyobraz.eu";        // server URL for "Zivy Obraz"
String Server_Key = "/?import_key=ENTER ZIVY OBRAZ IMPORT TOKEN";  // your import token for "Zivy Obraz" - register at Zivy Obraz to get one


/************************* Stops setup ****************************************************/
/*
 * STOP_COUNT_TOTAL = how many stops we want to querry in total = here it is 3 different stops
 * Stop_Id = array of stop ids codes, must be same count (!) = in this example 3 different stops are querried
 * Departure_Count = number of depatures for corresponding stop we want to querry
 * Walk_Distance = array of walk time to specific stop in minutes 
 * 
 */

#define STOP_COUNT_TOTAL 3                // 3 stops in total
         
char Stop_ID [STOP_COUNT_TOTAL] [8] = { 
 { "U693Z2P" },                           // U Palaty -> centrum ---> is stop #1
 { "U699Z3P" },                           // Strahov -> Dejvicka ---> is stop #2                      
 { "U254Z1P" },                           // Klamovka -> centrum ---> is stop #3               
 };

int Departure_Count[STOP_COUNT_TOTAL] = {
  3,                                        // we want 3 future departures from U Palaty
  3,                                        // we want 3 future departures from Strahov
  5                                         // we want 5 next departures from Klamovka
};

int Walk_Distance[STOP_COUNT_TOTAL] = {
  8,                                        // 7 minutes walk to U Palaty stop
  9,                                        // 9 minutes walk to Strahov stop
  12                                        // 12 minutes walk to Klamovka stop
}; 
