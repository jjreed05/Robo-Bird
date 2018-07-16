/*
 *  ECEN361
 *  Robo-Bird - Reciever
 */

#include <SPI.h>
#include "RF24.h"
#include <Servo.h>

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;
int motor = 6;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
Servo servo1;
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

typedef struct{ // values recieved will be stored here
  int stickX;
  int stickY;
}
Stick;

int X;
int Y;

Stick joyStick;

void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  
  pinMode(motor, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  servo1.attach(3);
  servo1.write(180);

  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  
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
  joyStick.stickY = 255;
  servo1.write(100);
  // Start the radio listening for data
  radio.startListening();
}

void loop() {
    if( radio.available()){
                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &joyStick, sizeof(unsigned long) );             // Get the payload
      }
     
      radio.stopListening(); // First, stop listening so we can talk  

      Y = map(joyStick.stickY, 0, 1023, -255, 255); //motor        // Mapping the values recieved and mapping them to be able to send them to the devices
      X = map(joyStick.stickX, 0, 1023, 55, 145); //servo

      delay(200);
      analogWrite(motor, Y); // assigning speed to motor
      servo1.write(X);   // assigning direction to servo
      
      Serial.println(joyStick.stickX);  // print recieved values to terminal
      Serial.println(joyStick.stickY);
      radio.startListening();
 }
} // Loop

