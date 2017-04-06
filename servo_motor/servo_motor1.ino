#include <Servo.h>

int servoPin = 3;
Servo servo;

int angle = 0;

void setup() {
  servo.attach(servoPin);

}

void loop() {
  //Scan from 0 to 180 degrees
  for (angle=0; angle < 360; angle++)
  {
    servo.write(angle);
    delay(15);
  }
  /*delay(5000);
  //Scan back from 180 to 0 degrees
  for (angle=180; angle > 0; angle--)
  {
    servo.write(angle);
    delay(15);
  }*/
  delay(2000);
}
