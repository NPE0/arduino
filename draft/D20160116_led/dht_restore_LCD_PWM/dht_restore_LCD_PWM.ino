#include <LiquidCrystal.h>
#include "dht.h"

dht DHT;

#define DHT22_PIN 6
#define BRIGHT_PIN 10
#define BRIGHT_CONTROL_PIN 8

#define SERIAL_ON 1

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

    byte cyr_ya[8] = {
    0b01111,
    0b10001,
    0b01111,
    0b00101,
    0b01001,
    0b10001,
    0b10001,
    0b00000
  };

  byte cyr_ss[8] = {
    0b10000,
    0b10000,
    0b10000,
    0b11110,
    0b10001,
    0b10001,
    0b11111,
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


#define CYR_L 1
#define CYR_Z 2
#define CYR_P 3
#define CYR_YA 8
#define CYR_SS 9

int bright_state=20;
#define BRIGHT_100 150
#define BRIGHT_0 0
#define BRIGHT_STEP 5
#define BRIGHT_PERCENT (BRIGHT_100 -BRIGHT_0)/100.0


void setup() {

  pinMode(BRIGHT_PIN, OUTPUT);
  analogWrite(BRIGHT_PIN, getBrightLevel(bright_state));
  pinMode(BRIGHT_CONTROL_PIN, INPUT);
  
 // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  
  lcd.createChar(0, degree);
 
  
 

 

  printBrackets();
    
  printHeader(true);
  
   
    #ifdef SERIAL_ON
      Serial.begin(115200);
      Serial.println("DHT TEST PROGRAM ");
      Serial.print("LIBRARY VERSION: ");
      Serial.println(DHT_LIB_VERSION);
      Serial.println();
      Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)\tTime (us)");
    #endif

}







int buttonState = 0; 
bool brightDirect=0;
uint32_t lcd_prev_time=0;
uint32_t bri_prev_time=0;
int chk;

// -1 - start 0- print temp, 1-set brighness
int state=-1;

void loop() {

  
    buttonState =digitalRead(BRIGHT_CONTROL_PIN);
    if (buttonState==HIGH){

        bright_state =bright_state+pow(-1,brightDirect)* BRIGHT_STEP;

        if (bright_state>100){
          bright_state=100;
          brightDirect=1;
        }

        if (bright_state<0){
          bright_state=0;
          brightDirect=0;
        }

        #ifdef SERIAL_ON
        Serial.print(getBrightLevel(bright_state));
        Serial.println();
        #endif
        
        
        analogWrite(BRIGHT_PIN, getBrightLevel(bright_state));  
        
        printBrighnessState();
        state =1;
        
        bri_prev_time =micros();
        delay(250);
        return;
      }
      
      if (state ==1 ){
        if (micros() -bri_prev_time  <1000000){
          return;
        }
      }
    

    if (micros() -lcd_prev_time  >2000000){ 
      printHeader(false); 
        // READ DATA
      #ifdef SERIAL_ON
        Serial.print("DHT22, \t");
      #endif
      uint32_t start = micros();
      chk = DHT.read22(DHT22_PIN);
      uint32_t stop = micros();
  
      printLCD(chk);
      
      #ifdef SERIAL_ON
        printSerial(chk,start,stop); 
      #endif
      
      state=0;  
      lcd_prev_time =micros();
    }

    delay(100);


}

/**Возврат уровня ШИМ для текущего значения яркости*/
int getBrightLevel(int percent){
  return  1.0* BRIGHT_0+ 1.0* percent*BRIGHT_PERCENT;
}



char* header= " B\1A\2H. TEM\3. ";

void printHeader(bool anim){
  if (state==0){
    return;
  }
 
  lcd.createChar(0, degree);
  lcd.createChar(1, cyr_l);
  lcd.createChar(CYR_Z, cyr_z);
  lcd.createChar(CYR_P, cyr_p);
  
  int headerLen =14;
  if (anim){
    for (int startPos=headerLen ; startPos>0; startPos--){
      lcd.setCursor(startPos, 0);
      
      for (int i=0 ; i<headerLen-startPos ;i++){
        lcd.write(header[i]);
        }
      delay(150);
     }


  }else{
      lcd.setCursor(1, 0);
     for (int i=0 ; i<headerLen ;i++){
        lcd.write(header[i]);
     }
 }
}


void printLCD(int chk){
  

   lcd.setCursor(0, 1);
   lcd.write((byte)5);
   switch (chk)
    {
    case DHTLIB_OK:
        
        lcd.print(" ");
        lcd.print(DHT.humidity,1);lcd.print("%  ");
        lcd.print(DHT.temperature,1);
        lcd.write((byte)0);
        lcd.print(" ");
        
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
    lcd.setCursor(15, 1);
    lcd.write((byte)7);
}


void printBrighnessState(){
  lcd.createChar(CYR_YA, cyr_ya);
  lcd.createChar(CYR_SS, cyr_ss);

  if (state!=1){
    printBrackets() ;
    
    lcd.setCursor(1, 0);
    lcd.print("   ");
    lcd.write(CYR_YA);
    lcd.print("PKOCT");
    lcd.write(CYR_SS);
    lcd.write(":   ");
  }

 
  lcd.setCursor(1, 1);
  lcd.print("      ");
 lcd.print(bright_state);
 if (bright_state<10){
  lcd.print("       ");
 }else if (bright_state<100){
  lcd.print("      ");
 }else{
  lcd.print("     ");
  }
  
  
}

void printBrackets(){
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
}




#ifdef SERIAL_ON
void printSerial(int chk,uint32_t start,uint32_t stop){
 lcd.createChar(0, degree);
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





