#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<string>
#define ssid "Tang 5"
#define password "55555555"
#define mqtt_server "m10.cloudmqtt.com"
#define mqtt_topic_pub "demo"
#define mqtt_topic_sub "demo"
#define mqtt_user "kzfqglrk"
#define mqtt_pwd "W0PM8GLTBDxC"
const uint16_t mqtt_port = 16963;
String inString = "";

WiFiClient espClient;// Tạo đối tượng wificlient
PubSubClient client(espClient);// Khai báo là client có thuộc tính của PubSubClient
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
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
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setup_wifi(); // Hàm tự viết ở dưới để kết nối wifi
  client.setServer(mqtt_server, mqtt_port);// Hàm kết nối vào mqtt server
  client.setCallback(callback);
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {// Nếu chưa kết nối
    Serial.print("Attempting MQTT connection..."); // thì in ra dòng này
    // Attempt to connect
    if (client.connect("ESP8266Client",mqtt_user, mqtt_pwd)) { //nếu kết nối đúng 
      Serial.println("connected");// in ra là đã kết nối
      // Once connected, publish an announcement...
      client.publish(mqtt_topic_pub, "ESP_reconnected");// rồi gửi lên broker là ESP_reconnected
      // ... and resubscribe
      client.subscribe(mqtt_topic_sub);// và lại hóng tin từ broker
    } else {
      Serial.print("failed, rc="); // còn nếu không thì in ra cái thất bại
      Serial.print(client.state());// in ra trạng thái của client
      Serial.println(" try again in 5 seconds"); 
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
 if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();// gán thời gian bây giờ là millis
  if (now - lastMsg > 2000) {// Cái này là cứ 2s 
    lastMsg = now;
    ++value;// giá trị được cộng 1
    snprintf (msg, 75, "hello world #%ld", value);// cái này là kiểu như gán giá trị của biến mảng mgs với kích thức là 75 , hello word rồi giá trị đc thay vào %d
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqtt_topic_pub, msg);// đẩy lên broker
  }
}
