

#include "Arduino.h"
#include "kattkran_extension.h"
#include <Servo.h>

// TODO every function included in kattkran.h



void Kattkran::init(){
    pinMode(SENSOR_PIN,INPUT);
    _servo.attach(SERVO_PIN);
    pinMode(LED_BUILTIN, OUTPUT);
}


bool Kattkran::sensor(){

  // NOTE currently the internal led activte when motion is detected
  // NOTE the sesor react with a LOW signal

  if (digitalRead(SENSOR_PIN) == LOW) {// check if the input is active
    digitalWrite(LED_BUILTIN, HIGH);  // turn LED ON
    if (_pir_state == HIGH) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      _pir_state = LOW;
    }
    return true;
  }
  else {//no motion detected
    digitalWrite(LED_BUILTIN, LOW); // turn LED OFF
    if (_pir_state == LOW){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      _pir_state = HIGH;
    }
    return  false ;
  }
}

void Kattkran::circular_motion(bool direction, byte speed) {
  // TODO verify circular_motion function
  /*

  */
  byte angle_goal;//the angle that the servo shoud end on
  byte start_angle;//the angle the servo has when the function is calld
  byte current_angle;//the angle that vill change in the function

  bool increase_angle;//if true increase curren_angle

  start_angle = _servo.read() ;

  if (direction) //this if/else determen the goal angle
    angle_goal = _away_angle;
  else //angle=false
    angle_goal = _tap_angle;

  increase_angle = (start_angle < angle_goal);

  current_angle = start_angle;

  while (angle_goal - current_angle) {
    /*
      this loop will change the curren_angle until curren_angle and goal is same
    */
    _servo.write(current_angle) ;
    delay(speed) ;
    if (increase_angle)
      ++current_angle;
    else
      --current_angle;
  }
}

void Kattkran::go_to_rest() {

}

void Kattkran::turn_water_on() {

}

void Kattkran::turn_water_off() {

}

void Kattkran::time_limit(){

  int t=0; //time unit
  while(t<WAIT_TIME){
    if(sensor())
      t=0; //cat's back

    else{
      delay(1);
      t++; //if cat isn't there, ++ time unit to later exit function
    }

  }
}


void Kattkran::identify_tap(){


}
