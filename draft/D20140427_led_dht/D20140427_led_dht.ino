#include <LiquidCrystal.h>
#include <dht.h>
 
#define DHT22_PIN 6
//#define DEBUG

dht DHT;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7,8, 2,3,4,5);
boolean err =false;

void setup() {
  // set up the LCD's number of columns and rows: 
  
  
  lcd.begin(16, 2);
  lcd.print("Init sensor");  
  for (int i =0; i<5;i++){
    lcd.print(".");
    delay(800);
  }
  
  

   lcd.setCursor(0, 0);
   printHeader();
   lcd.setCursor(0, 1);
   lcd.print("                ");


  #ifdef DEBUG
    Serial.begin(115200);
    Serial.println("DHT TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT_LIB_VERSION);
    Serial.println();
    Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)\tTime (us)");
  #endif 
   
}

void loop() {
   uint32_t start = micros();
   int chk = DHT.read22(DHT22_PIN);
   uint32_t stop = micros();

#ifdef DEBUG
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
#endif    

 switch (chk)
    {
    case DHTLIB_OK:
        if (err){   printHeader(); err=false; }
        lcd.setCursor(0, 1);
        lcd.print(DHT.humidity, 1);
        lcd.setCursor(6, 1);
        lcd.print(DHT.temperature, 1);
        break;
    case DHTLIB_ERROR_CHECKSUM:
        err=true;
        printErr("Checksum error!");
        break;
    case DHTLIB_ERROR_TIMEOUT:
        err=true;
        printErr("Time out error!");
        break;
    default:
        err=true;
        printErr("Unknown error");
        break;
    }
  
  // print the number of seconds since reset:
//  lcd.print(millis()/1000);
 //Serial.println(millis()/1000);
    delay(2000);
}

void printHeader(){
  lcd.clear();  
  lcd.print(" H%   TC");
  lcd.print((char)223);
}

void printErr(char* msg){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(msg);
}
