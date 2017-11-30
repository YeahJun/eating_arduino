#include <Wire.h> //I2C Arduino Library
#define HMC 0x1E //0011110b, I2C 7bit address of HMC5883, 지자기 센서
#define ACC 0x53 // I2C 7bit address of ADXL345, 가속도 센서
#define A_TO_READ (6)
#define GYRO 0x68 // I2C 7bit address of ITG3205, 자이로 센서
#define G_SMPLRT_DIV 0x15
#define G_DLPF_FS 0x16
#define G_INT_CFG 0x17
#define G_PWR_MGM 0x3E
#define G_TO_READ 8
#define TIME_MSG_LEN  11   // time sync to PC is HEADER and unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message

int Sound_Sensor = A0;
int Sensor_val;
int hx, hy, hz, turetemp;
int g_offx = 120;
int g_offy = 20;
int g_offz = 93;
byte addr;
char time_string[100];

void writeTo(int DEVICE, byte address, byte val)
{
  Wire.beginTransmission(DEVICE);
  Wire.write(address);
  Wire.write(val);
  Wire.endTransmission();
}

void readFrom(int DEVICE, byte address, int num, byte buff[])
{
  Wire.beginTransmission(DEVICE);
  Wire.write(address);
  Wire.endTransmission();
  Wire.beginTransmission(DEVICE);
  Wire.requestFrom(DEVICE, num);
  int i = 0;
  while (Wire.available())
  {
    buff[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
}

void initAcc() {
  writeTo(ACC, 0x2D, 1 << 3);
  writeTo(ACC, 0x31, 0x0B);
  writeTo(ACC, 0x2C, 0x09);

  writeTo(GYRO, G_PWR_MGM, 0x00);  //ITG3205 set
  writeTo(GYRO, G_SMPLRT_DIV, 0x07);
  writeTo(GYRO, G_DLPF_FS, 0x1E);   // +/- 2000 dgrs/sec, 1KHz
  writeTo(GYRO, G_INT_CFG, 0x00);
}

void getAccelerometerData(int * result)
{
  int regAddress = 0x32;
  byte buff[A_TO_READ];
  readFrom(ACC, regAddress, A_TO_READ, buff);
  result[0] = (((int)buff[1]) << 8) | buff[0];
  result[1] = (((int)buff[3]) << 8) | buff[2];
  result[2] = (((int)buff[5]) << 8) | buff[4];
}

void getGyroscopeData(int * result)
{
  int regAddress = 0x1B;
  int temp, x, y, z;
  byte buff[G_TO_READ];
  readFrom(GYRO, regAddress, G_TO_READ, buff);
  result[0] = ((buff[2] << 8) | buff[3]) + g_offx;
  result[1] = ((buff[4] << 8) | buff[5]) + g_offy;
  result[2] = ((buff[6] << 8) | buff[7]) + g_offz;
  result[3] = (buff[0] << 8) | buff[1];
}

void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT_PULLUP);

  Wire.begin();

  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(HMC); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();

  initAcc();
}

void loop() {
  int acc[3];
  int gyro[4];
  //Sound Sensor
  //normal value of 'Sensor val' = 200~380
  Sensor_val = analogRead(Sound_Sensor);
  Serial.print(Sensor_val);
  Serial.print("/");
  //Geomagnetic sensor
  int x, y, z; //triple axis data

  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(HMC);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();


  //Read data from each axis, 2 registers per axis
  Wire.requestFrom(HMC, 6);
  if (6 <= Wire.available()) {
    x = Wire.read() << 8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read() << 8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read() << 8; //Y msb
    y |= Wire.read(); //Y lsb
  }

  //Print out values of each axis
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(z);
  Serial.print("/");


  //ACC sensor
  getAccelerometerData(acc);
  hx = acc[0];
  hy = acc[1];
  hz = acc[2];
  Serial.print(hx);
  Serial.print(",");
  Serial.print(hy);
  Serial.print(",");
  Serial.print(hz);
  Serial.print("/");
  //Gyro sensor
  getGyroscopeData(gyro);
  hx = gyro[0] / 14.375;
  hy = gyro[1] / 14.375;
  hz = gyro[2] / 14.375;
  Serial.print(hx);
  Serial.print(",");
  Serial.print(hy);
  Serial.print(",");
  Serial.print(hz);
  Serial.print("/");
  // button

  int value = digitalRead(8);
  if (value == LOW) {
    Serial.print("1");
  }
  else {
    Serial.print("0");
  }
  Serial.println();
}
