/* Code for NODE B */

#include <SPI.h>
#include "RF24.h"

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 */
RF24 radio(9, 10);
/**********************************************************/

byte node_A_address[6] = "NodeA";
byte node_B_address[6] = "NodeB";

void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));

  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(node_A_address);
  radio.openReadingPipe(1, node_B_address);

  // Start the radio listening for data
  radio.startListening();
}

void loop() {
  /****************** Pong Back Role ***************************/

  char msg_from_A[20];

  if ( radio.available()) {
    // Variable for the received timestamp
    while (radio.available()) {                                   // While there is data ready
      radio.read( &msg_from_A, sizeof(msg_from_A) );             // Get the payload
    }

    radio.stopListening();                                        // First, stop listening so we can talk

    char msg_to_A[20] = "Hello from node_B";
    radio.write( &msg_to_A, sizeof(msg_to_A) );              // Send the final one back.
    radio.startListening();                                       // Now, resume listening so we catch the next packets.
    
    Serial.print(F("Got message '"));
    Serial.print(msg_from_A);
    Serial.print(F("', Sent response '"));
    Serial.print(msg_to_A);
    Serial.println(F("'"));
  }

}
