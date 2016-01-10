
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

// Constants
const int sensorT1 = A0;
const int sensorT2 = A1;
const int sensorT3 = A2;

const byte request_flag = 0x7E;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

void setup() {

  Serial.begin(115200);
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_HIGH);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  // Start the radio listening for data
  radio.startListening();
}

void loop() {
  while(Serial.available()) {
    volatile byte buffer;
    buffer = Serial.read();
    if(buffer == request_flag) {
      Serial.println("Got Request. Forwarding to sensor");
      radio.stopListening();
      radio.write(&request_flag, sizeof(request_flag));
      
      boolean timeout = false;
      unsigned long started_waiting_at = micros();

      while ( ! radio.available() ) {                            // While nothing is received
        if (micros() - started_waiting_at > 200000 ) {           // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
        }
      }

      if ( timeout ) {                                            // Describe the results
        Serial.println(F("Failed, response timed out."));
      } else {
        byte request;
        radio.read( &request, sizeof(request) );
        Serial.print(request, DEC);
      }
    }
  }
}

unsigned int readData(int pin) {
  return analogRead(pin);
}
