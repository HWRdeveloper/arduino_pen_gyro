#include<Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(3, 4);
const int MPU=0x68;  //MPU 6050 의 I2C 기본 주소
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
double aX, aY;
int current_color;
const double RADIAN_TO_DEGREE = 180 / 3.14159;
void setup(){
  Serial.begin(9600);                                 // 시리얼 통신 속도 설정 및 시작 합니다.
  BTSerial.begin(9600);     
  initSensor();
  Serial.begin(9600);
}
void initSensor() {
  Wire.begin();      //Wire 라이브러리 초기화
  Wire.beginTransmission(MPU); //MPU로 데이터 전송 시작
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     //MPU-6050 시작 모드로
  Wire.endTransmission(true); 
}
void loop(){
  int color;
  Wire.beginTransmission(MPU);    //데이터 전송시작
  Wire.write(0x3B);               // register 0x3B (ACCEL_XOUT_H), 큐에 데이터 기록
  Wire.endTransmission(false);    //연결유지
  Wire.requestFrom(MPU,14,true);  //MPU에 데이터 요청
  //데이터 한 바이트 씩 읽어서 반환
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  //시리얼 모니터에 출력
  aY = atan(-AcX / sqrt (pow (AcY, 2 ) + pow(AcZ,2)));
  aY *= RADIAN_TO_DEGREE;
  aX = atan(-AcY / sqrt (pow (AcY, 2 ) + pow(AcZ,2)));
  aX *= RADIAN_TO_DEGREE;
  if(abs(aY) > abs(aX)){
    if(aY > 0) {
     color = 1;

    } else{
      color = 3;

    }
  } else {
    if(aX>0) {
      color = 2;

    } else {
      color = 4;

    }
  }
  if(current_color  != color) {
          current_color = color;
          BTSerial.println(color);
  }
  delay(333);
}
