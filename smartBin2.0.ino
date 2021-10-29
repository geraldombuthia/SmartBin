#include <Servo.h>
#include <SoftwareSerial.h>

#define echoPin 2
#define trigPin 3
#define echoPin2 4
#define trigPin2 5
#define servoPin 6
#define greenLed 7
#define redLed 8

int distance;
int distance2;
long duration;
bool smsSent = false;

Servo Servos;
SoftwareSerial sim800l(7, 8);
void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  Servos.attach(servoPin);
  Serial.begin(9600);
  sim800l.begin(9600);
  Serial.println("Ultrasonic Sesnor executing");
  Serial.println("Servo on...");

}
int sonicOut(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.println(distance);
  delay(1000);
  return distance;
}
int sonicIn(){
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration = pulseIn(echoPin2, HIGH);
  distance2 = duration * 0.034 / 2;
  Serial.println(distance2);
  delay(1000);
  return distance2;
}
void servoOpen(){
  Servos.write(180);
  delay(1000);
}
void servoClose(){
  Servos.write(0);
  delay(1000);
}
void binEmptied(){
  if(sonicIn > 300 && smsSent == true){
    smsSent = false;
  }
}
void gsmSend(){
  Serial.println("Sending SMS...");               //Show this message on serial monitor
  sim800l.print("AT+CMGF=1\r");                   //Set the module to SMS mode
  delay(100);
  sim800l.print("AT+CMGS=\"+254728127853\"\r");  //Your phone number don't forget to include your country code, example +212123456789"
  delay(500);
  sim800l.print("Bin is full. Come clear the trash.");       //This is the text to send to the phone number, don't make it too long or you have to modify the SoftwareSerial buffer
  delay(500);
  sim800l.print((char)26);// (required according to the datasheet)
  delay(500);
  sim800l.println();
  Serial.println("Text Sent.");
  delay(500);
}
void ledFull(){
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, HIGH);
}
void ledEmpty(){
   digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
}
void loop() {
  // put your main code here, to run repeatedly:
  if (sim800l.available()){            //Displays on the serial monitor if there's a communication from the module
    Serial.write(sim800l.read()); 
  }
    if(sonicIn() < 30){
    Serial.println("Bin full");
    ledFull();
    servoClose();
    if(smsSent == false){
      gsmSend();
      smsSent = true;
    }
    return;
  } else {
    ledEmpty();
    if(sonicOut() < 30){
      servoOpen();
    }else {
      servoClose();
    }
  }
}
