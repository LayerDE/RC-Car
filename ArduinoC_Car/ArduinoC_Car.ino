#include <Servo.h>
#include <SPI.h>
#include <LoRa.h>
#define SERVO_COUNT (1<<2)
#define SERVO_MID 1500
Servo servo[SERVO_COUNT];
const uint8_t PROGMEM SERVO_PIN[SERVO_COUNT]={14,15,16,17};
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  for(uint8_t x=0;x<SERVO_COUNT;x++) servo[x].attach(SERVO_PIN[x]);
  //Serial.begin(9600);
  while (!Serial);

  //Serial.println("LoRa Receiver");

  if (!LoRa.begin(915E6)) {
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }
    // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}
/*
void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
*/
void loop(){}
union parameter {
  char bit_8[2];
  int16_t bit_16;
};

void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received packet '");
  union parameter tmp;
  // read packet
  for (int i = 0; i < packetSize; i++) {
    if(i%2){
      tmp.bit_8[1]=(char)LoRa.read();
      exec_servo(tmp.bit_16);
    }
    else{
      tmp.bit_8[0]=(char)LoRa.read();
    }
    
  }
  
  //LoRa.packetRssi(); //signalstärke
}
void exec_servo(int16_t data){
  servo[data&0xF].writeMicroseconds(SERVO_MID+data/0x10);  
}
