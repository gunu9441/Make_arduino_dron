//_04_mpu6050_gyro_125

#include <Wire.h>
const int mpu6050_addr = 0x68;
//MPU6050 I2C 통신 주소 = 0x68
int16_t GyX, GyY, GyZ; //16bit
void setup() {
  Serial.begin(115200);
  Wire.begin();
  //I2C 통신 시작
  Wire.beginTransmission(mpu6050_addr);
  //I2C 슬레이브 모듈과 통신을 시작할 때 호출
  Wire.write(0x6b);//0x6b : PWR_MGMT_1 레지스터
  //Wire.write:전송하고자하는 1바이트 데이터를 내부메모리 큐에 저장하는 역할
  Wire.write(0);
  //0x6b 번지에 0을 보내면 내부 0x6b 번지를 0으로하고 MPU6050을 깨움(sleep을 깨움)
  Wire.endTransmission(true);
  //Wire.write 함수에 의해 큐에 저장된 
  //하나이상의 바이트 데이터를 슬레이브모듈로 보내고 전송 끝
  //true 를 보내면 endTransmission 함수는 데이터 전송 후 
  //정지 메시지 보내고 I2C 버스의 제어권을 내려놓음
  //false 를 보내면 endTransmission 함수는 데이터 전송 후
  // 통신 재시작 메시지를 보냄 , I2C 버스 제어권을 내려놓지않음
}

void loop() {
  Wire.beginTransmission(mpu6050_addr);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(mpu6050_addr, 6, true);
  //MPU6050 내부 0x43 ~ 48 까지 자이로 센서값 6바이트 데이터 요청

  GyX = Wire.read() << 8|Wire.read();
  //자이로센서 값 을 1바이트씩 받게 됨
  GyY = Wire.read() << 8|Wire.read();
  GyZ = Wire.read() << 8|Wire.read();

  static int cnt_loop;
  cnt_loop++;
  if ( cnt_loop % 50 != 0) return;

  Serial.print("GyX= "); Serial.print(GyX);
  Serial.print(" | GyY= "); Serial.print(GyY);
  Serial.print(" | GyZ= "); Serial.print(GyZ);
  Serial.println();
}
