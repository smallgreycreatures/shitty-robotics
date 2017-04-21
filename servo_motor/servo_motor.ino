#include <Servo.h>

const byte servoPin = 9;
Servo servo;

int angle = 0;

void setup() {
  servo.attach(servoPin);

}

void loop() {
  //Scan from 0 to 180 degrees
  for (angle=40; angle <= 140; angle++)
  {
    servo.write(angle);
    delay(2);
  }
  while (servo.read()!=140)
    delay(1);
  //delay(5000);
  //Scan back from 180 to 0 degrees
  for (angle=140; angle >= 40; angle--)
  {
    servo.write(angle);
    delay(2);
  }
  while (servo.read()!=40)
    delay(1);
  //delay(2000)
}
