#include <Wire.h>

void setup() {
  Serial.begin(115200);
  
  Wire.begin();
  //I2C 통신 시작
  Wire.setClock(400000);
  
  Wire.beginTransmission(0x68);
  Wire.write(0x6b);
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() {
  Wire.beginTransmission(0x68);
  //I2C 슬레이브 모듈과 통신을 시작할 때 호출
  Wire.write(0x45);
  //indicate 0x45 to do something & make a kind of line
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 2, true);
  //request 2 byte memory from 0x68(ICT)

  int16_t GyYH = Wire.read();
  int16_t GyYL = Wire.read();
  int16_t GyY = GyYH << 8|GyYL;

  static int32_t GyYSum = 0;
  static double GyYOff = 0;
  static int cnt_sample = 10000;
  if(cnt_sample > 0){
    GyYSum +=GyY;
    cnt_sample--;
    if(cnt_sample ==0){
      GyYOff = GyYSum / 10000.0;
    }
    delay(1);
    return;
  }
  double GyYD = GyY - GyYOff;
//GyYD : 보정된 자이로센서 값
 double GyYR = GyYD / 131.0;
//추가 new02end

  static int cnt_loop;
  cnt_loop++;
  if(cnt_loop % 100 !=0) return;

  Serial.print("GyY = ");
  Serial.print(GyY);
  Serial.print(": GyYOff = ");
  Serial.print(GyYOff);
  Serial.print(" --> GyYD = ");
  Serial.print(GyYD);
  Serial.print(" : GyYR = ");
  Serial.print(GyYR);
  Serial.println();
}
