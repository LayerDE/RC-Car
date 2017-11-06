#include <Servo.h>
#include <SPI.h>
#include <LoRa.h>
#define SERVO_COUNT (1<<2)
Servo servo[SERVO_COUNT];
const uint8_t PROGMEM SERVO_PIN[SERVO_COUNT]={0,1,2,3};
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
void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received packet '");

  // read packet
  for (int i = 0; i < packetSize; i++) {
    Serial.print((char)LoRa.read());
  }

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
}
