#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

//CHANGE THE BELOW LINE TO IDENTIFY ROUTER
#define ROUTER_ID 0
#define TX_ADDRESS (((ROUTER_ID - 1) % 5) + 1)
#define BUGGY_RX_ADDRESS 0
#define ROUTER_RX_ADDRESS (ROUTER_ID % 5 + 1)

RF24 radio(7, 8); // CE, CSN

uint32_t LATEST_SEQ_NUM = 0;

void setup() 
{
  radio.begin();
  radio.openReadingPipe(BUGGY_RX_ADDRESS);
  radio.openReadingPipe(ROUTER_RX_ADDRESS);
  radio.setPALevel(RF24_PA_MIN);
  //Higher channels are less prone to interference
  //http://arduinoinfo.mywikis.net/wiki/Nrf24L01-2.4GHz-HowTo
  radio.setChannel(75);
  radio.startListening();
}

void loop() 
{
  #if ROUTER_ID == 0 
    //Base router only needs to listen and print to console
    while (radio.available())
    {
      uint32_t seq_num = 0;
      radio.read(&seq_num, sizeof(uint32_t));
      if (seq_num > LATEST_SEQ_NUM) 
      {
        //This is the latest message and we should process it
      }
      else 
      {
        //Clear the remaining bytes from the  
      }
    }
  #else
    //All other routers must listen and forward
  #endif

}
