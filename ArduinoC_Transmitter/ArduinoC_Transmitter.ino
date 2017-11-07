#include <SPI.h>
#include <LoRa.h>

int counter = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  if (!LoRa.begin(915E6)) {
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }
}

uint16_t make_servo(uint8_t num,int16_t val){
  return (num&0xF)|(val*0x10);
}
union parameter {
  char bit_8[];
  int16_t bit_16;
};
union parameter tmp;
void loop() {
  for(char x=0;;x=-x){
    LoRa.beginPacket();
    for(char i=0;i<4;i++){
      tmp.bit_16=make_servo(i,500*x),
      // send packet
      LoRa.print(tmp.bit_8[0]);
      LoRa.print(tmp.bit_8[1]);
    }
    LoRa.endPacket();
    delay(2000);
  }
}
