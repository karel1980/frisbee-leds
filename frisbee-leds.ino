#include <Adafruit_NeoPixel.h>
#include <TinyWireM.h>

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

#define NEOPIXEL_PIN 1
#define NEOPIXEL_COUNT 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void mpu_init() {
  TinyWireM.beginTransmission(MPU_addr);
  TinyWireM.send(0x6B);  // PWR_MGMT_1 register
  TinyWireM.send(0);     // set to zero (wakes up the MPU-6050)
  TinyWireM.endTransmission();

  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'  
}

void mpu_read() {
  TinyWireM.beginTransmission(MPU_addr);
  TinyWireM.send(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(MPU_addr, 14);  // request a total of 14 registers
  AcX=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void setup() {
  TinyWireM.begin();
  mpu_init();
}

int cycleOffset = 0;

void loop() {
  mpu_read();

  if(AcZ > 0) {
    fill(strip.Color(255,0,0));
  } else {
    fill(strip.Color(0,255,0));
  }
  strip.show();
}

void fill(uint32_t color) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
}
