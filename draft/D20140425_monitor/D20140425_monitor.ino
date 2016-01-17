/*
Adafruit Arduino - Lesson 5. Serial Monitor

*/

 

int latchPin = 5;

int clockPin = 6;

int dataPin = 4;
int states[8];
 

byte leds = 0;

 

void setup() 

{

  pinMode(latchPin, OUTPUT);

  pinMode(dataPin, OUTPUT);  

  pinMode(clockPin, OUTPUT);

  updateShiftRegister();

  Serial.begin(9600);

  while (! Serial); // Wait untilSerial is ready - Leonardo

  Serial.println("Enter LED Number 0 to 7 or 'x' to clear");

}

 

void loop() 

{

  if (Serial.available())

  {

    char ch = Serial.read();

    if (ch >= '0' && ch <= '7')  {

      int led = ch - '0';
      if (!states[led]){
        bitSet(leds, led);
        states[led]=1;
      }else{
          bitClear(leds, led);
          states[led]=0;
      }

      updateShiftRegister();

      Serial.print("Turned on LED ");

      Serial.println(led);

    }

    if (ch == 'x')

    {

      leds = 0;

      updateShiftRegister();

      Serial.println("Cleared");

    }

  }

}

 

void updateShiftRegister()

{

   digitalWrite(latchPin, LOW);

   shiftOut(dataPin, clockPin, LSBFIRST, leds);

   digitalWrite(latchPin, HIGH);

}
