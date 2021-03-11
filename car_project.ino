#include <AFMotor.h> 
#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include <SPI.h>
#include "hidjoystickrptparser.h"

AF_DCMotor URMotor(1, MOTOR12_64KHZ);
AF_DCMotor ULMotor(2, MOTOR12_64KHZ);
AF_DCMotor LLMotor(3, MOTOR12_64KHZ);
AF_DCMotor LRMotor(4, MOTOR12_64KHZ);

USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);
JoystickEvents JoyEvents;
JoystickReportParser Joy(&JoyEvents);

const int echoPin = 41;
const int trigPin = 39;
const int greenLED = 22;
const int redLED = 24;
const int buzzer = 26;

long duration;
int distance; 

void setup() {
  Serial.begin(115200);
  Serial.println("-----------Program Start-----------");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Motor Initialization
  URMotor.setSpeed(255);
  ULMotor.setSpeed(255);
  LLMotor.setSpeed(255);
  LRMotor.setSpeed(255);

  URMotor.run(RELEASE);
  ULMotor.run(RELEASE);
  LLMotor.run(RELEASE);
  LRMotor.run(RELEASE);

  // USB Controller Initialization
  if (Usb.Init() == -1)
    Serial.println("Controller did not start.");
  else 
    Serial.println("Controller is connected");

  delay(200);

  if (!Hid.SetReportParser(0, &Joy))
    ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1);
}

void loop() {
  Usb.Task();

  // Reading distance from ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  

  // Controller will only work if distance is > 5cm
  if(distance > 5) {
    // LED and Buzzer Logic
    digitalWrite(buzzer, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);

    // Controller Logic
    switch(JoyEvents.buttonPress) {
      case 1:
        Serial.println("X button is pressed!");
      
        URMotor.setSpeed(255);
        ULMotor.setSpeed(255);
        LLMotor.setSpeed(255);
        LRMotor.setSpeed(255);

        URMotor.run(FORWARD);
        ULMotor.run(RELEASE);
        LLMotor.run(RELEASE);
        LRMotor.run(FORWARD);
        break;
      case 2:
        Serial.println("A button is pressed!");
        URMotor.setSpeed(255);
        ULMotor.setSpeed(255);
        LLMotor.setSpeed(255);
        LRMotor.setSpeed(255);

        URMotor.run(BACKWARD);
        ULMotor.run(BACKWARD);
        LLMotor.run(BACKWARD);
        LRMotor.run(BACKWARD);
        break;
      case 3:
        Serial.println("B button is pressed!");

        URMotor.setSpeed(255);
        ULMotor.setSpeed(255);
        LLMotor.setSpeed(255);
        LRMotor.setSpeed(255);

        URMotor.run(RELEASE);
        ULMotor.run(FORWARD);
        LLMotor.run(FORWARD);
        LRMotor.run(RELEASE);
        break;
      case 4:
        Serial.println("Y button is pressed!");

        URMotor.setSpeed(255);
        ULMotor.setSpeed(255);
        LLMotor.setSpeed(255);
        LRMotor.setSpeed(255);

        URMotor.run(FORWARD);
        ULMotor.run(FORWARD);
        LLMotor.run(FORWARD);
        LRMotor.run(FORWARD);
        break;      
      case 5:
        Serial.println("LB button is pressed!");
        break;
      case 6:
        Serial.println("RB button is pressed!");
        break;
      case 7:
        Serial.println("LT button is pressed!");
        break;
      case 8:
        Serial.println("RT button is pressed!");
        break; 
      default:
        URMotor.run(RELEASE);
        ULMotor.run(RELEASE);
        LLMotor.run(RELEASE);
        LRMotor.run(RELEASE);
    }
  } else {
    digitalWrite(buzzer, HIGH);
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);        
  }
}
