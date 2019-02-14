#include<Servo.h>

Servo serX;
Servo serY;

String serialData;

void setup() {
  serX.attach(10);
  serY.attach(11);
  Serial.begin(9600);
  Serial.setTimeout(25);
  

}

void loop() {

}

void serialEvent(){
  serialData = Serial.readString();

  serX.write(parseDataX(serialData));
  serY.write(parseDataY(serialData));
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

