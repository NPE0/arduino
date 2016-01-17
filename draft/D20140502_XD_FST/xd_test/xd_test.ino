#include <VirtualWire.h>
void setup()
{ 
  delay(1000);
   Serial.begin(115200);       
    vw_set_ptt_inverted(true);  // Required by the RF module

    vw_set_tx_pin(9);         // Arduino pin to connect the TRANSMITTer data pin
    vw_set_rx_pin(10);         // Arduino pin to connect the receiver data pin
   vw_set_ptt_pin(11);
    vw_setup(2000);            // bps connection speed
    vw_rx_start(); 
}

   const char *msg = "HELLO WORLD";
 
void loop()
{
   //Message to send:
   vw_send((uint8_t *)msg, strlen(msg));
   vw_wait_tx();        // We wait to finish sending the message

   
   //delay(100);  
   uint8_t buf[VW_MAX_MESSAGE_LEN];
   uint8_t buflen = VW_MAX_MESSAGE_LEN;
//   if (vw_wait_rx_max(2000)){
     if (vw_get_message(buf, &buflen))      // We check if we have received data
     {
      Serial.println("data");
      int i;
      // Message with proper check    
      for (i = 0; i < buflen; i++)
      {
        Serial.write(buf[i]);  // The received data is stored in the buffer
                              // and sent through the serial port to the computer
      }
      Serial.println();
  //  }
  }else {
    //if (vw_tx_active())  Serial.println("active");

    Serial.println("Timout ");
  }
     delay(200);         // We wait to send the message again                
}

