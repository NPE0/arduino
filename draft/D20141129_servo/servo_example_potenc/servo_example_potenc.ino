#include <SoftwareServo.h> 

SoftwareServo myservo;  // create servo object to control a servo 

int potpin = 0;  // analog pin used to connect the potentiometer
int val=0;    // variable to read the value from the analog pin 

void setup() 
{ 
  Serial.begin(9600);
  myservo.attach(2);  // attaches the servo on pin 2 to the servo object 
} 

void loop() 
{ 
  int valNew = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  if ((valNew> val+2) || (valNew<val-2) ){ 
    Serial.print("analog: ");
    Serial.print(valNew);
    Serial.print(":");
    Serial.print(val);
    val = valNew;
    int writeVal = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180) 
    Serial.print(":");
    Serial.println(writeVal);
    myservo.write(writeVal);                  // sets the servo position according to the scaled value 
    SoftwareServo::refresh();
  }
  delay(15);                           // waits for the servo to get there 


} 

