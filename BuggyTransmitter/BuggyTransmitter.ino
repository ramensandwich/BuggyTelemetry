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
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(75);
  radio.stopListening();
  Serial.begin(115200);
}
void loop() {
  //We're multicasting, so we don't want ACKs from all of our receivers
  //TODO: Determine if our receivers are in range to interfere and if we want ACKs or not

  //We want the loop to run at 10Hz
  uint32_t loop_end = millis() + 100;

  transmit_data(TEST_HEADER, 0);
  transmit_data(43, 10);
  transmit_data(44, 99);
  transmit_data(51, 573489);

  //Limit the loop to run at 10Hz
  //while (millis() < loop_end) {}
}

void transmit_data(uint8_t header, uint32_t data)
{
  radio.write(&SEQ_NUM, sizeof(uint32_t));
  SEQ_NUM++;
  radio.write(&header, sizeof(uint8_t));
  radio.write(&data, sizeof(uint32_t));
}
