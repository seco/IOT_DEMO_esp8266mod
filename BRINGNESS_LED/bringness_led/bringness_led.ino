#include <ESP8266WiFi.h>
void LED_CHANGE();

// LED FET
#define ledPIN    0

int led_delay = 0;
#define time_at_colour 1200 
unsigned long TIME_LED = 0;
int LED; 
int LED_A = 0;

// WiFi Settings
const char *ssid =  "Bao";
const char *pass =  "0907777905";

// Start WiFi Server
WiFiServer server(80);
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT); 
  
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("");
  
  server.begin();
  Serial.println("Webserver started");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (client) {
    
      // Read the first line of the request
      String req = client.readStringUntil('\r');
      Serial.println(req);
      client.flush();
    
      // Prepare the response. Start with the common header:
      String s = "HTTP/1.1 200 OK\r\n";
      s += "Content-Type: text/html\r\n\r\n";
      s += "<!DOCTYPE HTML>\r\n<html>\r\n";
      
      if((req.indexOf("/led/") != -1)) { //test for servo control sring
         
        // get RGB value
        int pos = req.length();
        int ind1 = req.indexOf("/led/") + 5;
        String teststring = req.substring(ind1, pos);
        int ind2 = teststring.indexOf('/');
        String ledValue = req.substring(ind1, ind2+ind1);
        
        int ledVal = map(ledValue.toInt(),0,100,0,1023);
        LED = constrain(ledVal,0,1023);
        
        int diff_led = abs(LED-LED_A);
        if(diff_led > 0){
          led_delay = time_at_colour / abs(LED-LED_A); 
        }else{
          led_delay = time_at_colour / 100; 
        }
        
        // confirm to client
        s += "LED value changed to ";
        s += ledValue;
      
      }
      
      s += "<br>";
      s += "Request OK!";
      
      s += "</html>\n";
     
      client.flush();
    
      // Send the response to the client
      client.print(s);
      //delay(1);
      Serial.println("Client disonnected");
    
      // The client will actually be disconnected 
      // when the function returns and 'client' object is detroyed
      
  }
  
  if(millis() - TIME_LED >= led_delay){
    TIME_LED = millis();
    LED_CHANGE();
  }
}
void LED_CHANGE()
{
  if(LED != LED_A){
    if(LED_A > LED) LED_A = LED_A - 1;
    if(LED_A < LED) LED_A++;
    digitalWrite(LED_BUILTIN, LED_A);
  }
}
