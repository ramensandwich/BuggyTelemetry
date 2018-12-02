#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define TX_ADDRESS 0

RF24 radio(7, 8); // CE, CSN
uint32_t SEQ_NUM = 0;
uint8_t TEST_HEADER = 42;
uint8_t addresses[][6] = {"0Node", "1Node", "2Node", "3Node", "4Node", "5Node"};


void setup() {
  radio.begin();
  radio.openWritingPipe(addresses[TX_ADDRESS]);
  radio.setPALevel(RF24_PA_MIN);
  //Higher channels are less prone to interference
  //http://arduinoinfo.mywikis.net/wiki/Nrf24L01-2.4GHz-HowTo
  radio.setChannel(75);
  radio.stopListening();
  Serial.begin(9600);
}
void loop() {
  //We're multicasting, so we don't want ACKs from all of our receivers
  //TODO: Determine if our receivers are in range to interfere and if we want ACKs or not
  radio.write(&SEQ_NUM, sizeof(uint32_t), true);
  radio.write(&TEST_HEADER, sizeof(uint8_t), true);
  uint32_t payload = 0;
  radio.write(&payload, sizeof(uint32_t), true);
  Serial.print("Send: ");
  Serial.println(SEQ_NUM);
  SEQ_NUM++;
  delay(1000);
}
