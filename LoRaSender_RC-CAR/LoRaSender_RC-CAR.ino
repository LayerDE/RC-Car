#include <SPI.h>
#include <LoRa.h>

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6

#define ADC_SERVO0 36
#define SERVO0_MID (1<<11)
#define SERVO0_MIN 0
#define SERVO0_MAX ((1<<12)-1)
#define DEADZONE0 (1<<5)

#define ADC_SERVO1 37
#define SERVO1_MID (1<<11)
#define SERVO1_MIN 0
#define SERVO1_MAX ((1<<12)-1)
#define DEADZONE1 (1<<5)

char buffer[4];
char event_buffer[256];
void setup() {
  Serial.begin(921600);
  while (!Serial);

  Serial.println("LoRa Sender");
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  buffer[3] = event_buffer[0] = '\0';
}

void set_buff(uint16_t inval){
    buffer[1] = inval >> 8;
    buffer[2] = inval & 0xFF;
}
void getServo0(){
  int tmp = analogRead(ADC_SERVO0);
  buffer[0] = 'x';
  if(tmp < SERVO0_MID){
    if(tmp > SERVO0_MID - DEADZONE0)
      set_buff(1500);
    else if(tmp < SERVO0_MIN + DEADZONE0)
      set_buff(850);
    else
      set_buff(map(tmp,SERVO0_MIN + DEADZONE0,SERVO0_MID - DEADZONE0,850,1500));
  }
  else{
    if(tmp > SERVO0_MAX - DEADZONE0)
      set_buff(2150);
    else if(tmp < SERVO0_MID + DEADZONE0)
      set_buff(1500);
    else
      set_buff(map(tmp,SERVO0_MID + DEADZONE0,SERVO0_MAX - DEADZONE0,1500,2150));
  }
}

void getServo1(){
  int tmp = analogRead(ADC_SERVO1);
  buffer[0] = 'y';
  if(tmp < SERVO1_MID){
    if(tmp > SERVO1_MID - DEADZONE1)
      set_buff(1500);
    else if(tmp < SERVO1_MIN + DEADZONE1)
      set_buff(850);
    else
      set_buff(map(tmp,SERVO1_MIN + DEADZONE1,SERVO1_MID - DEADZONE1,850,1500));
  }
  else{
    if(tmp > SERVO1_MAX - DEADZONE1)
      set_buff(2150);
    else if(tmp < SERVO1_MID + DEADZONE1)
      set_buff(1500);
    else
      set_buff(map(tmp,SERVO1_MID + DEADZONE1,SERVO1_MAX - DEADZONE1,1500,2150));
  }
}

void loop() {
  Serial.println("Sending packet");

  // send packet
  LoRa.beginPacket();
  getServo0();
  LoRa.print(buffer);
  getServo1();
  LoRa.print(buffer);
  LoRa.print(event_buffer);
  event_buffer[0] = '\0';
  LoRa.endPacket();
  delay(5);
}
