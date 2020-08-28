#include <Wire.h>

void setup() {
  Serial.begin(115200);
  
  Wire.begin();
  //I2C 통신 시작99
  Wire.setClock(400000);
  //I2C 통신 속도 설정
  Wire.beginTransmission(0x68);
  Wire.write(0x6b);
  Wire.write(0);
  Wire.endTransmission(true);
}

int throttle = 0;

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

  static unsigned long t_prev = 0;
 //지역벽수인데 전역변수처럼 사용, 이전의 값을 계속 기억함
  unsigned long t_now = micros();
  double dt =(t_now - t_prev) / 1000000.0;
  t_prev = t_now;

  static double AngleY = 0.0;
  AngleY += GyYR * dt;//거리 = 속력 * 시간

  static double tAngleY = 0;
  double eAngleY = tAngleY - AngleY;   //수평의 각도 - 기울어진 각도(오른쪽 +)
  double Kp = 1.0; // Kp = a:증폭값
  //y = a * (-AngleY)
  double BalY = Kp * eAngleY;

 double Kd = 1.0;
 BalY += Kd * (-GyYR);               //여기는 왜 GyYR이지? 위에는 각도를 통해서 곱해줬는데..
 if (throttle ==0) BalY =0;
 
  if(Serial.available()>0){
    while(Serial.available()>0){
      char input = Serial.read();
      if(input >= '0' && input<='9'){
        throttle = (input - '0') * 25;
      }
    }
  }
  double speedA = throttle + BalY;
  double speedB = throttle - BalY;
  double speedC = throttle - BalY;
  double speedD = throttle + BalY;

  int iSpeedA = constrain((int)speedA, 0, 250);
  int iSpeedB = constrain((int)speedB, 0, 250);
  int iSpeedC = constrain((int)speedC, 0, 250);
  int iSpeedD = constrain((int)speedD, 0, 250);

  analogWrite(6,iSpeedA);
  analogWrite(10,iSpeedB);
  analogWrite(9,iSpeedC);
  analogWrite(5,iSpeedD);
  
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
  Serial.print(" dt = ");
  Serial.print(dt,6);
  //소수점 6번째 까지 출력
  Serial.println();
  Serial.print(" AngleY = ");
  Serial.print(AngleY);
  Serial.print(" | BalY = ");
  Serial.print(BalY);
  Serial.println();
  Serial.print("A = ");
  Serial.print(speedA);
  Serial.print("|B = ");
  Serial.print(speedB);
  Serial.print("|C = ");
  Serial.print(speedC);
  Serial.print("|D = ");
  Serial.print(speedD);
  Serial.println();
}
