/**
Схема - 2 кнопки и диод 
*/

int bottom1Pin = 9;
int bottom2Pin = 8;

int ledPin = 5;

void setup(){
  pinMode(bottom1Pin, INPUT_PULLUP );

  pinMode(bottom2Pin, INPUT_PULLUP);

  pinMode(ledPin, OUTPUT); 

  Serial.begin(9600);
}



void loop(){
  
  int sensorValue1 = digitalRead(bottom1Pin);
  Serial.println(sensorValue1);
  int sensorValue2 = digitalRead(bottom2Pin);
    Serial.println(sensorValue2);

  if (sensorValue1==LOW){
     digitalWrite(ledPin, HIGH);  
  }

  if (sensorValue2==LOW){
       digitalWrite(ledPin, LOW);    
  }else{

  }
 
  
}

