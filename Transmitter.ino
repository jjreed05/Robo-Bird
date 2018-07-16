/*
 *  ECEN361
 *  Robo-Bird - Transmitter
 */

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;
int sensorValue = 0;
int sensorPinX = A0;
int sensorPinY = A1;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

typedef struct{ // This struct is to be sent to the bird for Servo and Motor control
  int stickX;
  int stickY;
} Stick;

Stick joyStick;

void setup() {
  Serial.begin(115200);
  Serial.println(F("GettingStarted"));
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_MAX);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){ 
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
}

void loop() {
    radio.stopListening();     // First, stop listening so we can talk.
    
    Serial.println(F("Now sending"));
    joyStick.stickX = analogRead(sensorPinX); // read a value from the joystick and assign it to the struct
    joyStick.stickY = analogRead(sensorPinY);  
    
     if (!radio.write( &joyStick, sizeof(unsigned long) )){  // sending the struct and checking for a failure to send.
       Serial.println(F("failed"));
     }
      
        // Describe the results
        Serial.println((joyStick.stickX));  // print joystick values to the terminal
        Serial.println((joyStick.stickY));

    delay(1000);
} // Loop

