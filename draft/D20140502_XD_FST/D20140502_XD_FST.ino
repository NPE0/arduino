
#include <LiquidCrystal.h>
#include <dht.h>
#include <VirtualWire.h>
 
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
  
  
  /////////////////////////////////
    Serial.begin(9600);	// Debugging only
    Serial.println("setup");
  
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);	 // Bits per sec
    vw_set_tx_pin(9);  //transmitter pin

    vw_set_rx_pin(10); //receiver pin 
    vw_rx_start();       // Start the receiver PLL running
 
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
 
///////////////////////////
   const char *msg = "HELLO WORLD";
   vw_send((uint8_t *)msg, strlen(msg));
   vw_wait_tx();        // We wait to finish sending the message
   delay(200);         // We wait to send the message again       
   
   uint8_t buf[VW_MAX_MESSAGE_LEN];
   uint8_t buflen = VW_MAX_MESSAGE_LEN;
   
    // Wait at most 200ms for a reply
    if (vw_wait_rx_max(200))
    {
	if (vw_get_message(buf, &buflen)) // Non-blocking
	{
	    int i;
	    
	    // Message with a good checksum received, dump it.
	    Serial.print("Got: ");
	    
	    for (i = 0; i < buflen; i++)
	    {
		Serial.print(buf[i], HEX);
		Serial.print(" ");
	    }
	    Serial.println("");
	}
    }
    else
	Serial.println("Timout");

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


void sendHello(){
      const char *msg = "hello";
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    digitalWrite(13, true); // Flash a light to show transmitting
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    Serial.println("Sent");
    digitalWrite(13, false);

}



