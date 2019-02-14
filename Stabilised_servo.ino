#include <Wire.h>
#include <Servo.h>

Servo serX;
Servo serY;

String serialData;

int gX, gY, gZ;       //declares Gyro variables
long aX, aY, aZ;      //declares Accelerometer variables
long cGX, cGY, cGZ;   //declares Callibration variables
int gList[] = {gX, gY, gZ};
long aList[] = {aX, aY, aZ};
long cList[] = {cGX, cGY, cGZ};
int temperature;
float anglePitch, angleRoll;
float k = 1/(250 * 65.5);
void setup() {
  serX.attach(10);
  serY.attach(11);
  Wire.begin();
  Serial.begin(9600);
  Serial.setTimeout(25);

  setupMPU6050Registers();
  
  //Serial.println("Callibrating gyros");
  for (int cInt = 0; cInt < 2000 ; cInt ++){
    //if(cInt % 125 == 0)Serial.print(".");
    readMPU6050Data();
    
    /*for (int i = 0; i < 3 ; i ++){
      cList[i] += gList[i];
    }*/
    
    cGX += gX;
    cGY += gY;
    cGZ += gZ;
    delay(3);
  }
  cGX /= 2000;
  cGY /= 2000;
  cGZ /= 2000;
  /*for (int i = 0; i < 3 ; i ++){
    cList[i] /= 2000;
    }*/
  //Serial.println(cList[0]);
  //Serial.println(cList[1]);
  //Serial.println(cList[2]);
  Serial.println(cGX);
  Serial.println(cGY);
  Serial.println(cGZ);
}


void loop() {

  readMPU6050Data();

  gX -= cGX;
  gY -= cGY;
  gZ -= cGZ;

  
  anglePitch += (gX * k);
  angleRoll += (gY * k);

  anglePitch += angleRoll * sin(gZ * k * (PI/180));
  angleRoll -= anglePitch * sin(gZ * k * (PI/180));

  
  //Serial.println(gX);
  //Serial.println(gY);
  //Serial.println(gZ);

  Serial.println(anglePitch);
  Serial.println(angleRoll);

}

void serialEvent(){
  serialData = Serial.readString();

  serX.write(parseDataX(serialData)+3.6 * anglePitch);
  serY.write(parseDataY(serialData)+3.6 * angleRoll);
}

int parseDataX(String data){
  data.remove(data.indexOf("Y"));
  data.remove(data.indexOf("b"),1);
  data.remove(data.indexOf("X"),1);

  return data.toInt();
}
int parseDataY(String data){
  data.remove(0, data.indexOf("Y")+1);

  return data.toInt();
}


void setupMPU6050Registers(){
  //Activate the MPU-6050
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x6B);                                                    //Send the requested starting register
  Wire.write(0x00);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  //Configure the accelerometer (+/-8g)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1C);                                                    //Send the requested starting register
  Wire.write(0x10);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  //Configure the gyro (500dps full scale)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1B);                                                    //Send the requested starting register
  Wire.write(0x08);                                                    //Set the requested starting register
  Wire.endTransmission();
}

void readMPU6050Data(){                                             //Subroutine for reading the raw gyro and accelerometer data
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x3B);                                                    //Send the requested starting register
  Wire.endTransmission();                                              //End the transmission
  Wire.requestFrom(0x68,14);                                           //Request 14 bytes from the MPU-6050
  while(Wire.available() < 14);                                        //Wait until all the bytes are received
  aX = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_x variable
  aY = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_y variable
  aZ = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_z variable
  temperature = Wire.read()<<8|Wire.read();                            //Add the low and high byte to the temperature variable
  gX = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_x variable
  gY = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_y variable
  gZ = Wire.read()<<8|Wire.read(); 
}
