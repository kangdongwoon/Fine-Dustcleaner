/*
 * Arduino Uno에서 PM2008 미세먼지 센서 값을 I2C 통신으로 읽고
 * Uart 통신으로 NodeMCU에 Startbit와 Endbit를 포함해 전송
 */
#include <pm2008_i2c.h>
#define enA 9
#define in3 4
#define in4 5
PM2008_I2C pm2008_i2c;
unsigned char data[3] = {0,0,0};

void setup() {
  //pinMode(enA, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pm2008_i2c.begin();
  Serial.begin(9600);
  pm2008_i2c.command();
  delay(1000);
}
int sensor=0;
void loop() {
  uint8_t ret = pm2008_i2c.read();
  if (ret == 0) {
    //Serial.println(pm2008_i2c.pm1p0_grimm);
    //Serial.write(pm2008_i2c.pm10_grimm);
    data[0] = (unsigned char)(pm2008_i2c.pm1p0_grimm/100);
    data[1] = (unsigned char)(pm2008_i2c.pm1p0_grimm/10);
    data[2] = (unsigned char)(pm2008_i2c.pm1p0_grimm%10);
  }
  Serial.print('s');
  Serial.print(data[0]);
  delay(200);
  Serial.print(data[1]);
  delay(200);
  Serial.print(data[2]);
  Serial.print('e');
  delay(600);

  /*
   * IN1 6 
   * IN2 7
   * ENA 9
   */
  
  sensor = data[0]*100+data[1]*10+data[2];
  Serial.print(sensor);
  if(sensor>20){
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);  
  }
  else if(sensor<10){
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);  
  }  
}
