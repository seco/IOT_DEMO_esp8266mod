#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


MDNSResponder mdns;
String webPage; 
ESP8266WebServer server(80);
const char* ssid     = "Bao";      
const char* password = "0907777905"; 
void setup() {
 pinMode(LED_BUILTIN, OUTPUT);
  webPage += "<h1>ESP8266</h1><p>Socket <a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a></p>";
 
  Serial.begin(115200);
  delay(100);
 
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) 
    Serial.println("MDNS responder started");
 
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on("/on", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  });
  server.on("/off", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000); 
  });
 
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop() {
  server.handleClient();
}
