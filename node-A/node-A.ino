/* Code for NODE A */

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
  radio.openWritingPipe(node_B_address);
  radio.openReadingPipe(1, node_A_address);

  // Start the radio listening for data
  radio.startListening();
}

void loop() {
  /****************** Ping Out Role ***************************/

  radio.stopListening();                                    // First, stop listening so we can talk.
  
  Serial.println(F("Now sending Hello to node B"));

  char msg_to_B[20] = "Hello from node_A";                             // Take the time, and send it.  This will block until complete
  
  unsigned long start_time = micros();      
  if (!radio.write( &msg_to_B, sizeof(msg_to_B) )) {
    Serial.println(F("failed"));
  }

  radio.startListening();                                    // Now, continue listening

  unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not

  while ( ! radio.available() ) {                            // While nothing is received
    if (micros() - started_waiting_at > 200000 ) {           // If waited longer than 200ms, indicate timeout and exit while loop
      timeout = true;
      break;
    }
  }

  if ( timeout ) {                                            // Describe the results
    Serial.println(F("Failed, response timed out."));
  } else {
    char msg_from_B[20];                             // Grab the response, compare, and send to debugging spew
    radio.read( &msg_from_B, sizeof(msg_from_B) );
    unsigned long end_time = micros();

    // Spew it
    Serial.print(F("Sent '"));
    Serial.print(msg_to_B);
    Serial.print(F("', Got response '"));
    Serial.print(msg_from_B);
    Serial.print(F("', Round-trip delay "));
    Serial.print(end_time - start_time);
    Serial.println(F(" microseconds"));
  }

  // Try again 1s later
  delay(2000);
}
