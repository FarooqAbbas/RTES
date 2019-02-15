#include "MPU9250.h"
#include <Wire.h>

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;



void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000L);  
  setupMPU();
}


void loop() {
  recordAccelRegisters();
//  recordGyroRegisters();
  printData();
  delay(600);
}

void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  //###################################################################################//
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1D); // Assessing the register 29
  Wire.write(0b1000); // setting the ACCEL_FCHOICE_B
  Wire.endTransmission();

  

  Wire.beginTransmission(0b1101000);
  Wire.write(0x19); // Assessing the register 25
  Wire.write(0b00000000); // setting the Sample Rate Divider
  Wire.endTransmission();

  //##############################################################################//
 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}

void processAccelData(){
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0; 
  gForceZ = accelZ / 16384.0;
}
void printData() {

  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
}
