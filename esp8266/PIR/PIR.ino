
//sp8266 node mcu 红外感应器
int ledPin=13;//esp8266 GPIO D7

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, INPUT);
  Serial.begin(9600);
}

void loop() {
 // 讀取 PIR Sensor 的狀態
  int value  = digitalRead(ledPin);

  // 判斷 PIR Sensor 的狀態
  if (value == HIGH) {     
     Serial.println("true");
  }
  else {
    Serial.println("false");
  }
  delay(100);
}
