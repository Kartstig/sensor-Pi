
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
  byte request;
  
  if( radio.available()){
    while (radio.available()) {
      radio.read( &request, sizeof(byte) );
    }
   
    if( request == request_flag ) {
      radio.stopListening();
      unsigned int payload = readData(sensorT1);
      radio.write( &payload, sizeof(payload) );
      radio.startListening();
    }
  }
}

unsigned int readData(int pin) {
  return analogRead(pin);
}
