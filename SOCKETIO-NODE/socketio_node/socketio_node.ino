#include <SocketIOClient.h>
#define LedPin 2
#define ButtonPin 0
#define SOFTAP_MODE
#ifdef SOFTAP_MODE
const char* password = "0907777905";
#else
const char* ssid = "Bao";
const char* password = "0907777905";
#endif
const char HexLookup[17] = "0123456789ABCDEF";
String host = "192.168.1.18";
int port = 3000;
bool clicked = false;
SocketIOClient socket;
void setupNetwork() {
 #ifdef SOFTAP_MODE
 WiFi.disconnect();
 byte mac[6];
 WiFi.macAddress(mac);
 char ssid[14] = "Minion-000000";
 ssid[7] = HexLookup[(mac[3] & 0xf0) >> 4];
 ssid[8] = HexLookup[(mac[3] & 0x0f)];
 ssid[9] = HexLookup[(mac[4] & 0xf0) >> 4];
 ssid[10] = HexLookup[(mac[4] & 0x0f)];
 ssid[11] = HexLookup[(mac[5] & 0xf0) >> 4];
 ssid[12] = HexLookup[(mac[5] & 0x0f)];
 ssid[13] = 0;
 WiFi.softAP(ssid, password);
 #else
 WiFi.begin(ssid, password);
  Serial.println(WiFi.localIP());
 
 uint8_t i = 0;
 while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
 if(i == 21){
 while(1) delay(500);
 }
 #endif
}
void click() {
 clicked = true;
}
void light(String state) {
 Serial.println("[light] " + state);
 if (state == "\"state\":true") {
 Serial.println("[light] ON");
 digitalWrite(LED_BUILTIN, HIGH);
 }
 else {
 Serial.println("[light] off");
 digitalWrite(LED_BUILTIN, LOW);
 }
}
void setup() {
  // put your setup code here, to run once:
pinMode(LED_BUILTIN, OUTPUT);
 
 digitalWrite(LED_BUILTIN, HIGH);
 Serial.begin(115200);
 setupNetwork();
 attachInterrupt(digitalPinToInterrupt(ButtonPin), click, FALLING);
if(!socket.connect(host, port)){
  Serial.println("Fail connect to socket ");
  return;
}
 socket.connect(host, port);
 socket.on("light", light);

}


void clickCheck() {
 if (clicked) {
 Serial.println("[click]");
 socket.emit("toggle", "{\"state\":true}");
 clicked = false;
 }
}

void loop() {
  // put your main code here, to run repeatedly:
 socket.monitor();
 clickCheck();
}
