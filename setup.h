
// WiFi credentials
#define WIFI_PASS "YOUR WIFI PASSWORD"
#define WIFI_SSID "YOUR WIFI SSID"

#define HTTP_GET_INTERVAL_SECS 180        //interval in secs to load data to Zivy Obraz, do not put less than 60s

// PID Golemio server token here - see https://api.golemio.cz/api-keys/auth/sign-in
String auth_token = "_insert Golemio token here_";

// Zivy obraz
String Server_Name = "http://in.zivyobraz.eu";        // server URL for "Zivy Obraz"
String Server_Key = "/?import_key=_import key pro Zivy Obraz_";  // your import token for "Zivy Obraz"

/*
 * NOTE on PID stop ids codes
 *
 * either one specific stop: Stop_Ids = "ids=U693Z2P"; // zde bus U Palaty do centra
 * nebo: Stop_Ids = "ids=U254Z1P";  // tramvaj Klamovka do centra
 * or cluster of more stops: String STOP_IDS = "ids=U612Z1P&ids=U612Z12P&ids=U612Z2P&ids=U612Z3P&ids=U612Z4P&ids=U612Z41P&ids=U612Z5P";  
 */
