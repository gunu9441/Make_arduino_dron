#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Wire.begin();
  Wire.setClock(400000);//400KHz
  //I2C 통신속도 설정
  Wire.beginTransmission(0x68);
  Wire.write(0x6b);
  Wire.write(0x0);
  Wire.endTransmission(true);
}

int throttle  = 0;
//throttle 전역변수

void loop() {
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,6,true); //new 12 rpy
//new12 rpy start
  int16_t GyXH = Wire.read();
  int16_t GyXL = Wire.read();
  int16_t GyYH = Wire.read();
  int16_t GyYL = Wire.read();
  int16_t GyZH = Wire.read();
  int16_t GyZL = Wire.read();
  int16_t GyX = GyXH << 8 | GyXL;
  int16_t GyY = GyYH << 8 | GyYL;
  int16_t GyZ = GyZH << 8 | GyZL;
  //new12 rpy
 static int32_t GyXSum = 0,GyYSum = 0,GyZSum = 0;//new12 rpy
 static double GyXOff=0.0,GyYOff=0.0,GyZOff=0.0;//new12 rpy
 static int cnt_sample = 1000;//1000 개

 if( cnt_sample > 0){ //new12 rpy
    GyXSum +=GyX; //자이로센서값 1000 개 누적
    GyYSum +=GyY;
    GyZSum +=GyZ;
    cnt_sample--;
    if(cnt_sample == 0){
      GyXOff = GyXSum /1000.0;
      GyYOff = GyYSum /1000.0;
      GyZOff = GyZSum /1000.0;
    }
    delay(1);
    return;
  } //new12 rpy
  double GyXD = GyX - GyXOff;
  double GyYD = GyY - GyYOff;
  double GyZD = GyZ - GyZOff;
  //GyYD : 보정된 자이로센서 값
  double GyXR = GyXD /131.0; // new03 추가  ****왜 나누어준것?
  double GyYR = GyYD /131.0;
  double GyZR = GyZD /131.0;//new rpy

//new04 start
  static unsigned long t_prev = 0;
  unsigned long t_now = micros();
  double dt = (t_now - t_prev)/1000000.0;
  t_prev = t_now;
//new04 end
//new05 start
static double AngleX = 0.0, //new12rpy
              AngleY = 0.0,
              AngleZ = 0.0; //각도 **** 각속도인지 각도인지
AngleX += GyXR * dt; 
AngleY += GyYR * dt; 
AngleZ += GyZR * dt;  //new12
 
if (throttle ==0) AngleX = AngleY=AngleZ = 0;//new 10 추가 new12
//new05 end
//new06 start 좌우균형값 찾기
static double tAngleX = 0.0,
              tAngleY = 0.0,
              tAngleZ = 0.0;
double eAngleX = tAngleX - AngleX; 
// tAngleX: 목표 각도, AngleX: 현재 각도, eAngleX: 틀어진 각도
double eAngleY = tAngleY - AngleY;
double eAngleZ = tAngleZ - AngleZ; 

double Kp = 1.0; // Kp =a:증폭값
//y = a*(-AngleY) 
double BalX = Kp * eAngleX;
double BalY = Kp * eAngleY;
double BalZ = Kp * eAngleZ;

//회전속도로 좌우균형값 찾기
double Kd = 1.0; //상쇄 상수
BalX += Kd *(-GyXR); //y = b *(-w);
BalY += Kd *(-GyYR);
BalZ += Kd *(-GyZR);
if (throttle ==0) BalX=BalY=BalZ = 0;


if(Serial.available()>0){
    while(Serial.available()>0){
        char input = Serial.read();
        if(input >= '0' && input<='9'){
            throttle =(input - '0')*25;//'0' == 48
        }
        else if(input == 'a'){
          tAngleY = -10.0;
        }
        else if(input == 'd'){
          tAngleY = +10.0;
        }
        else if(input =='s'){
          tAngleY = 0.0;
        }
    }
}
//new08 end
//new07 start
double speedA = throttle + BalY + BalX + BalZ;
double speedB = throttle - BalY + BalX - BalZ;
double speedC = throttle - BalY - BalX + BalZ;
double speedD = throttle + BalY - BalX - BalZ;
if(throttle ==0){
  AngleX = AngleY = AngleZ = 0.0;
  speedA = speedB = speedC = speedD = 0;
}
//new07 end
//new09 start
int iSpeedA = constrain((int)speedA,0,250);
int iSpeedB = constrain((int)speedB,0,250);
int iSpeedC = constrain((int)speedC,0,250);
int iSpeedD = constrain((int)speedD,0,250);

analogWrite(6,iSpeedA);
analogWrite(10,iSpeedB);
analogWrite(9,iSpeedC);
analogWrite(5,iSpeedD);
//new09 end

  static int cnt_loop;
  cnt_loop++;
  if(cnt_loop %100 !=0) return;
/*
  Serial.print("GyY = ");
  Serial.print(GyY);
  Serial.print(": GyYOff = ");
  Serial.print(GyYOff);
  Serial.print(" --> GyYD = ");
  Serial.print(GyYD);
  Serial.print(" : GyYR = ");
  Serial.print(GyYR);
  Serial.print("dt = ");
  Serial.print(dt,6);
  Serial.print("AngleY = ");
  Serial.print(AngleY);
  Serial.print(" | BalY = ");
  Serial.print(BalY);*/
  Serial.print("  A = ");Serial.print(speedA);
  Serial.print(" | B = ");Serial.print(speedB);
  Serial.print(" | C = ");Serial.print(speedC);
  Serial.print(" | D = ");Serial.print(speedD);
  Serial.println();
}
//new12_angle_velo_rpy
