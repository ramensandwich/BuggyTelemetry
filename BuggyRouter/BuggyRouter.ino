#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define RETRY_LIMIT 5
//CHANGE THE BELOW LINE TO IDENTIFY ROUTER
#define ROUTER_ID 1
#define TX_ADDRESS (((ROUTER_ID - 1) % 5) + 1)
#define BUGGY_RX_ADDRESS 0
#define ROUTER_RX_ADDRESS (ROUTER_ID % 5 + 1)
uint8_t addresses[][6] = {"0Node", "1Node", "2Node", "3Node", "4Node", "5Node"};

RF24 radio(7, 8); // CE, CSN

uint32_t LATEST_SEQ_NUM = 0;
uint32_t tStart = millis();
uint32_t packet_count = 0;

void setup() 
{
  radio.begin();
  //Note: Pipe 0 is also used by the writing pipe
  radio.openReadingPipe(1, addresses[BUGGY_RX_ADDRESS]);
  radio.openReadingPipe(2, addresses[ROUTER_RX_ADDRESS]);
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
      //These are default values
      uint32_t seq_num = 0;
      uint8_t msg_id = 0;
      uint32_t msg_data = 0;
      //TODO: Error checking to verify that the correct number of bytes were read

      radio.read(&seq_num, sizeof(uint32_t));
      while (!radio.available());
      radio.read(&msg_id, sizeof(uint8_t));
      while (!radio.available()); 
      radio.read(&msg_data, sizeof(uint32_t));
      
      if (seq_num > LATEST_SEQ_NUM) 
      {
        LATEST_SEQ_NUM = seq_num;
        packet_count++;
        uint32_t tDelta = millis() - tStart;
        float tDeltaSec = (float)tDelta / 1000;
        float bandwidth = packet_count * 4 * 8 / tDeltaSec;
        //This is the latest message and we should process it
        //otherwise we'll just skip it
        #if ROUTER_ID == 1 
        //Base router only needs to listen and print to console
          Serial.print("Seq num: ");
          Serial.print(seq_num);
          Serial.print(" id: ");
          Serial.print(msg_id);
          Serial.print(" data: ");
          Serial.print(msg_data);
          Serial.print(" Bandwidth: ");
          Serial.println(bandwidth);
        #else
          //Switch from receiver to transmitter
          radio.stopListening();
          radio.openWritingPipe(addresses[TX_ADDRESS]);
          
          radio.write(&seq_num, sizeof(uint32_t));
          radio.write(&msg_id, sizeof(uint8_t));
          radio.write(&msg_data, sizeof(uint32_t));

          //Switch back to receiver
          radio.startListening();
        #endif
      }

    }
}
