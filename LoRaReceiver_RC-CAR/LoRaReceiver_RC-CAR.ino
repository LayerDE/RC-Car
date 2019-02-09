#include <SPI.h>
#include <LoRa.h>
#include <ESP32Servo.h>

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6

// create four servo objects 
Servo throttle;
Servo steering;
Servo lights;

const int light_pos[] = {1000, 1500, 2000};
#define LIGHTS_OFF light_pos[0]
#define LIGHTS_SLEEPY light_pos[1]
#define LIGHTS_ON light_pos[0]
// Published values for SG90 servos; adjust if needed
int minUs = 850;
int maxUs = 2150;

// These are all GPIO pins on the ESP32
// Recommended pins include 2,4,12-19,21-23,25-27,32-33 
int servo1Pin = 18;
int servo2Pin = 19;
int servo3Pin = 12;

int pos1 = 0;      // position in degrees
int pos2 = 0;

void setup() {
  throttle.setPeriodHertz(50);      // Standard 50hz servo
  steering.setPeriodHertz(50);      // Standard 50hz servo
  lights.setPeriodHertz(50);      // Standard 50hz servo

  throttle.attach(servo1Pin, minUs, maxUs);
  steering.attach(servo2Pin, minUs, maxUs);
  lights.attach(servo3Pin, minUs, maxUs);
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  throttle.writeMicroseconds(1500);
  steering.writeMicroseconds(1500);
  lights.writeMicroseconds(LIGHTS_OFF);
}

/**
 * hex2int
 * take a hex string and convert it to a 32bit number (max 8 hex digits)
 */
int hex2int(char hex) {
        // get current character then increment
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (hex >= '0' && hex <= '9') return hex - '0';
        else if (hex >= 'a' && hex <='f') return hex - 'a' + 10;
        else if (hex >= 'A' && hex <='F') return hex - 'A' + 10;    
        else return 0;
    }
int state = 0;
void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      char tmp_val = (char)LoRa.read();
      Serial.print(tmp_val);
      switch(state){
        case 0:
          if(packetSize >= 3){
            if(tmp_val == 'x' || tmp_val == 'X'){
              state = 1;
              break;
            }
            else if(tmp_val == 'y' || tmp_val == 'Y'){
              state = 3;
              break;
            }
            else{
              break;
            }
          }
          else{
            break;
          }
        case 1:
          pos1 = tmp_val<<8;
          state++;
          break;
        case 2:
          pos1 |= tmp_val;
          throttle.write(pos1);
          state = 0;
          break;
        case 3:
          pos2 = tmp_val<<8;
          state++;
          break;
        case 4:
          pos2 |= tmp_val;
          steering.write(pos2);
          state = 0;
          break;
      }
        
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
