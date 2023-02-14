#include <Wire.h>
#define I2C_SLAVE_ADDR 0x04 // 4 in hexadecimal

#define LEFT1 35 
#define LEFT2 4 
#define LEFT3 25 
#define RIGHT1 13 
#define RIGHT2 26 
#define RIGHT3 27 

int16_t servoAngle,leftMotorSpeed=100, rightMotorSpeed=100;


void setup() {
  Serial.begin(115200);
  //Serial.begin(9600);
  Wire.begin();   // join i2c bus to Address 4
}

void loop() {
  
  // mapping the analouge values from the IR sensors
  int left_1 = map(constrain(analogRead(LEFT1),1719,4096), 1719, 4096, 10, 0);
  Serial.print("Left1: ");
  Serial.println(left_1);

  
  int left_2 = map(constrain(analogRead(LEFT2),1705,4096), 1705, 4096, 10, 0);
  Serial.print("Left2: ");
  Serial.println(left_2);
  int left_3 = map(constrain(analogRead(LEFT3),1167,4096), 1167, 4096, 10, 0);
  Serial.print("Left3: ");
  Serial.println(left_3);

  int right_1 = map(constrain(analogRead(RIGHT1),1244,4096), 1244, 4096, 10, 0);
  Serial.print("Right1: ");
  Serial.println(right_1);
  
  int right_2 = map(constrain(analogRead(RIGHT2),1197,4096), 1197, 4096, 10, 0);
  Serial.print("Right2: ");
  Serial.println(analogRead(RIGHT2));
  int right_3 = map(constrain(analogRead(RIGHT2),602,4096), 602, 4096, 10, 0);
  Serial.print("Right3");
  Serial.println(right_3);

  
  int sensor_val = left_1+left_2+left_3+right_1+right_2+right_3;  // sum of the sensors values
  double w_average = ((left_1*-8)+(left_2*-23)+(left_3*-37)+(right_1*8)+(right_2*23)+(right_3*37))/sensor_val;
  
  Serial.print("W average");
  Serial.println(w_average);

  if (w_average<0){
    //Turning Right
    servoAngle = 40;
  }
  else if(w_average>0){
    //Turning Left
    servoAngle = 150;
  }
  else{
    // Straight on black line
    servoAngle = 83;
  }
  
  Serial.print("Servo Angle: ");
  Serial.println(servoAngle);
  
  transmit_to_arduino();
  delay(100);

}

void transmit_to_arduino(){
  Wire.beginTransmission(I2C_SLAVE_ADDR); // transmit to device #4
    Wire.write((byte)((leftMotorSpeed & 0x0000FF00) >> 8));    // first byte of x, containing bits 16 to 9
    Wire.write((byte)(leftMotorSpeed & 0x000000FF));           // second byte of x, containing the 8 LSB - bits 8 to 1
    Wire.write((byte)((rightMotorSpeed & 0x0000FF00) >> 8));    // first byte of y, containing bits 16 to 9
    Wire.write((byte)(rightMotorSpeed & 0x000000FF));           // second byte of y, containing the 8 LSB - bits 8 to 1
    
    Wire.write((byte)((servoAngle & 0x0000FF00) >> 8));    // first byte of y, containing bits 16 to 9
    Wire.write((byte)(servoAngle & 0x000000FF));           // second byte of y, containing the 8 LSB - bits 8 to 1
  Wire.endTransmission();   // stop transmitting 
  
}
