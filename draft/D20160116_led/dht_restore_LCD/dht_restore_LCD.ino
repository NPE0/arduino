#include <LiquidCrystal.h>
#include "dht.h"

dht DHT;

#define DHT22_PIN 6
//#define SERIAL_ON 1

   // initialize the library with the numbers of the interface pins
  LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
  byte degree[8] = {
    0b11000,
    0b11000,
    0b00111,
    0b01000,
    0b01000,
    0b01000,
    0b00111,
    0b00000
  };
  byte cyr_l[8] = {
    0b00111,
    0b01101,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b00000
  };
   byte cyr_z[8] = {
    0b10101,
    0b10101,
    0b01110,
    0b00100,
    0b01110,
    0b10101,
    0b10101,
    0b00000
  };
   byte cyr_p[8] = {
    0b11111,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b00000
  }; 

  byte bigs_upL[8] = {
    0b01111,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000
  }; 

 byte bigs_doL[8] = {
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b01111,
    0b00000
  }; 
  
   byte bigs_upR[8] = {
    0b11110,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001
  }; 

 byte bigs_doR[8] = {
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b11110,
    0b00000
  }; 
  
void setup() {
   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  
 // create a new character
  lcd.createChar(0, degree);
  lcd.createChar(1, cyr_l);
  lcd.createChar(2, cyr_z);
  lcd.createChar(3, cyr_p);
  
  lcd.createChar(4, bigs_upL);
  lcd.createChar(5, bigs_doL);
  lcd.createChar(6, bigs_upR);
  lcd.createChar(7, bigs_doR);


  lcd.setCursor(0, 0);
  lcd.write((byte)4);
  lcd.setCursor(0, 1);
  lcd.write((byte)5);
  lcd.setCursor(15, 0);
  lcd.write((byte)6);
  lcd.setCursor(15, 1);
  lcd.write((byte)7);
    
  
  for (int i=14;i >0; i--){
    printHeader(i,14);
  }
  
   
    #ifdef SERIAL_ON
      Serial.begin(115200);
      Serial.println("DHT TEST PROGRAM ");
      Serial.print("LIBRARY VERSION: ");
      Serial.println(DHT_LIB_VERSION);
      Serial.println();
      Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)\tTime (us)");
    #endif

}


char* header= " B\1A\2H. TEM\3. ";
int headerLen =14;
void printHeader(int startPos,int  endPos){
  lcd.setCursor(startPos, 0);
  for (int i=0 ; i<endPos-startPos ;i++){
    lcd.write(header[i]);
    }
  delay(300);
}




void loop() {
  
      // READ DATA
    #ifdef SERIAL_ON
      Serial.print("DHT22, \t");
    #endif
    uint32_t start = micros();
    int chk = DHT.read22(DHT22_PIN);
    uint32_t stop = micros();

    printLCD(chk);
    
    #ifdef SERIAL_ON
      printSerial(chk,start,stop); 
    #endif
    
    delay(2000);


}


void printLCD(int chk){
  

   lcd.setCursor(0, 1);
   switch (chk)
    {
    case DHTLIB_OK:
        lcd.write((byte)5);
        lcd.print(" ");
        lcd.print(DHT.humidity,1);lcd.print("%  ");
        lcd.print(DHT.temperature,1);
        lcd.write((byte)0);
        lcd.print(" ");
        lcd.write((byte)7);
        break;
    case DHTLIB_ERROR_CHECKSUM:
        lcd.print("Checksum error.");
        break;
    case DHTLIB_ERROR_TIMEOUT:
        lcd.print("Time out error.");
        break;
    default:
        lcd.print("Unknown error.");
        break;
    }
}



#ifdef SERIAL_ON
void printSerial(int chk,uint32_t start,uint32_t stop){
 
   switch (chk)
    {
    case DHTLIB_OK:
        Serial.print("OK,\t");
        break;
    case DHTLIB_ERROR_CHECKSUM:
        Serial.print("Checksum error,\t");
        break;
    case DHTLIB_ERROR_TIMEOUT:
        Serial.print("Time out error,\t");
        break;
    default:
        Serial.print("Unknown error,\t");
        break;
    }

      // DISPLAY DATA
      Serial.print(DHT.humidity, 1);
      Serial.print(",\t");
      Serial.print(DHT.temperature, 1);
      Serial.print(",\t");
      Serial.print(stop - start);
      Serial.println();
}
#endif


