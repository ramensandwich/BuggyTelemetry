#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

//CHANGE THE BELOW LINE TO IDENTIFY ROUTER
#define ROUTER_ID 1
#define TX_ADDRESS (((ROUTER_ID - 1) % 5) + 1)
#define BUGGY_RX_ADDRESS 0
#define ROUTER_RX_ADDRESS (ROUTER_ID % 5 + 1)

const uint8_t buggy_rx_address BUGGY_RX_ADDRESS;
const uint8_t tx_address TX_ADDRESS;
const uint8_t router_rx_address ROUTER_RX_ADDRESS;

RF24 radio(7, 8); // CE, CSN

uint32_t LATEST_SEQ_NUM = 0;

void setup() 
{
  radio.begin();
  radio.openReadingPipe(1, &buggy_rx_address);
  radio.openReadingPipe(2, &router_rx_address);
  radio.setPALevel(RF24_PA_MIN);
  //Higher channels are less prone to interference
  //http://arduinoinfo.mywikis.net/wiki/Nrf24L01-2.4GHz-HowTo
  radio.setChannel(75);
  radio.startListening();

  #if ROUTER_ID == 1
    Serial.begin(115200);
  #endif
}

void loop() 
{
    while (radio.available())
    {
      uint32_t seq_num = 0;
      uint8_t msg_id = 0;
      uint32_t msg_data = 0;
      //TODO: Error checking to verify that the correct number of bytes were read
      radio.read(&seq_num, sizeof(uint32_t));\
      radio.read(&msg_id, sizeof(uint8_t));
      radio.read(&msg_data, sizeof(uint32_t));
      if (seq_num > LATEST_SEQ_NUM) 
      {
        //This is the latest message and we should process it
        //otherwise we'll just skip it
        #if ROUTER_ID == 1 
        //Base router only needs to listen and print to console
          Serial.print("Seq num: ");
          Serial.print(seq_num);
          Serial.print(" data: ");
          Serial.print(msg_id);
          Serial.print(" msg_id: ");
          Serial.print(msg_data);
        #else
          //Switch from receiver to transmitter
          radio.stopListening();
          radio.openWritingPipe(&tx_address);
          
          radio.write(&seq_num, sizeof(uint32_t));
          radio.write(&msg_id, sizeof(uint8_t));
          radio.write(&msg_data, sizeof(uint32_t));

          //Switch back to receiver
          radio.startListening();
        #endif
      }

    }
}
