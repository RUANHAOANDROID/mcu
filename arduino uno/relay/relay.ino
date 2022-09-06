int pin=3;
void setup() {
  // put your setup code here, to run once:
pinMode(pin,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(pin,HIGH );
delay(5000);
digitalWrite(pin,LOW);
delay(5000);
digitalWrite(pin,HIGH);
}
