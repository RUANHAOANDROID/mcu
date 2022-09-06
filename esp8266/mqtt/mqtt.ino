#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// esp8266 mqtt + pir count
// 网络配置
const char* ssid = "OpenWrt-2.4";
const char* password = "password";
const char* mqtt_server = "hao88.cloud";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (5120)
char msg[MSG_BUFFER_SIZE];
char count = 0;
int pirGPIO = 13; //esp8266 GPIO D7
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

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
  //if ((char)payload[0] == '1') {
  
  //重置计数
  count = 0;
  digitalWrite(BUILTIN_LED, LOW);//点亮LED
  delay(500);
  digitalWrite(BUILTIN_LED, HIGH);//熄灭LED
}
//重连
void reconnect() {
  // 循环直到连接
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // pub client id
    String clientId = "ESP8266ClientCount-";
    clientId += String(random(0xffff), HEX);
    // 尝试连接
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("iot/esp8266/count", "MQTT Server is Connected");
      // 订阅重置topic
      client.subscribe("iot/esp8266/reset");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // 等待5秒重试
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(pirGPIO, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  int pirValue  = digitalRead(pirGPIO);

  //监听到计数
  if (pirValue == HIGH) {
    unsigned long now = millis();
    ++count;
    snprintf (msg, MSG_BUFFER_SIZE, "%ld", count);
    Serial.print("publish msg : ");
    Serial.println(msg);
    client.publish("iot/esp8266/count", msg);//推消息
    delay(800);
  }

}
