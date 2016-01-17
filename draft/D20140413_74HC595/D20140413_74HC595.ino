int latchPin = 5;

int clockPin = 6;

int dataPin = 4;

 

byte leds = 0;

 

void setup() 

{

  pinMode(latchPin, OUTPUT);

  pinMode(dataPin, OUTPUT);  

  pinMode(clockPin, OUTPUT);

}

 

void loop() 

{


//leds =0; 
//  updateShiftRegister();

//  delay(500);

  for (int i = 0; i < 8; i++)

  {
    leds = 0;
    bitSet(leds, i);
   // bitSet(leds, 7-i);

    updateShiftRegister();

    delay(200);

  }

 


}

 

void updateShiftRegister()

{

   digitalWrite(latchPin, LOW);

   shiftOut(dataPin, clockPin, LSBFIRST, leds);

   digitalWrite(latchPin, HIGH);

}

