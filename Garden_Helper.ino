#include <CapacitiveSensor.h>


CapacitiveSensor blueSensor = CapacitiveSensor(4,2);


const int ledPinBlue =10;


int prevTouchStateBlue = LOW;
bool powerOnBlue = false;


int soilVal;
int soilPin = A0;
int soilPower = 13;


int threshold = 0; //capacitive touch threshold
int debounceTime = 50;
long startTime;




void setup() {
 Serial.begin(9600);
 pinMode(ledPinBlue, OUTPUT);
 pinMode(soilPower, OUTPUT);
 digitalWrite(soilPower, LOW);


 //calibrate threshold because the soil moisture sensor affects the sensitivity of the capacitive touch sensor
 Serial.println("calibrating, remove and insert soil moisture sensor for FIVE seconds");
 delay(1000);
  while(millis() < 5000) {
     long sensorVal = blueSensor.capacitiveSensor(30);
     if (sensorVal > threshold) {
         threshold = sensorVal;
     }
  }
  threshold = threshold + 100;


  Serial.println("done calibrating, threshold = ");
  Serial.println(threshold);
 
  startTime = millis(); // Set time for debounce check


}




void loop() {
  long blueSensorValue = blueSensor.capacitiveSensor(30);
  checkForTouch(blueSensorValue, ledPinBlue, prevTouchStateBlue, powerOnBlue);
}




//reads the soil moisture level
int readSoil()
{


   digitalWrite(soilPower, HIGH);//turn D13 "On"
   delay(10);//wait 10 milliseconds
   soilVal = analogRead(soilPin);//Read the SIG value form sensor
   digitalWrite(soilPower, LOW);//turn D13 "Off"
   return soilVal;//send current moisture value
}






//checks whether there has been a high or low change to the capacitive touch sensor
void checkForTouch(int sensorValue, int ledPin, int &prevTouchState, bool &powerOn)
{
 long currentTime = millis();  // Get current time to be used for debounce check
  int touchState = LOW;
  if (sensorValue > threshold) {
   touchState = HIGH;


  }


 bool debounceCheck = currentTime - startTime > debounceTime; // Ensure that there isn't a flicker
 bool stateChange = touchState == HIGH && prevTouchState != HIGH; // Check if the button has been pressed down


 if ( stateChange && debounceCheck ) {
   int state = powerOn ? LOW : HIGH; // Get new state of LED
   powerOn = !powerOn; // Track the state of the LED
   digitalWrite(ledPin, state); // Change the state of the LED
   startTime = millis(); // Reset time for debounce check
  
   if (state == HIGH) {
     Serial.println("LED on, take soil moisture level");
     int val = readSoil();
     Serial.println(val); //for python code
   } else{
     Serial.println("LED turned off");
     Serial.println("-2"); //for python code
    
   }
 }


 prevTouchState = touchState;


}
