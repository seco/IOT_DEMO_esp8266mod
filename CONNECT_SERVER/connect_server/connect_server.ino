#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
 
SocketIOClient client;
const char* ssid = "Tang 5";          
const char* password = "55555555"; 
int port = 3484;
char host[]  = "192.168.1.165";
extern String RID;
extern String Rfull;
 
 
//Một số biến dùng cho việc tạo một task
unsigned long previousMillis = 0;
long interval = 2000;
 
void setup()
{
    //Bật baudrate ở mức 115200 để giao tiếp với máy tính qua Serial
    Serial.begin(115200);
    delay(10);
 
    //Việc đầu tiên cần làm là kết nối vào mạng Wifi
    Serial.print("Ket noi vao mang ");
    Serial.println(ssid);
 
    //Kết nối vào mạng Wifi
    WiFi.begin(ssid, password);
 
    //Chờ đến khi đã được kết nối
    while (WiFi.status() != WL_CONNECTED) { //Thoát ra khỏi vòng 
        delay(500);
        Serial.print('.');
    }
 
    Serial.println();
    Serial.println(F("Da ket noi WiFi"));
    Serial.println(F("Dia chi IP cua ESP8266 (Socket Client ESP8266): "));
    Serial.println(WiFi.localIP());
 
    if (!client.connect(host, port)) {
        Serial.println(F("Ket noi den socket server that bai!"));
        return;
    }
 
    //Khi đã kết nối thành công
    if (client.connected()) {
        //Thì gửi sự kiện ("connection") đến Socket server ahihi.
        client.send("connection", "message", "Connected !!!!");
    }
}
 
void loop()
{
    //tạo một task cứ sau "interval" giây thì chạy lệnh:
    if (millis() - previousMillis > interval) {
        //lệnh:
        previousMillis = millis();
 
        //gửi sự kiện "atime" là một JSON chứa tham số message có nội dung là Time please?
        client.send("atime", "message", "Time please?");
    }
 
    //Khi bắt được bất kỳ sự kiện nào thì chúng ta có hai tham số:
    //  +RID: Tên sự kiện
    //  +RFull: Danh sách tham số được nén thành chuỗi JSON!
    if (client.monitor()) {
        Serial.println(RID);
        Serial.println(Rfull);
    }
 
    //Kết nối lại!
    if (!client.connected()) {
      client.reconnect(host, port);
    }
}
