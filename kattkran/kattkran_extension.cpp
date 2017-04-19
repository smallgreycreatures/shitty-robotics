

#include "Arduino.h"
#include "kattkran_extension.h"
#include <Servo.h>

// TODO every function included in kattkran.h



Kattkran::Kattkran(){
//TODO a nice init
    pinMode(SENSOR_PIN,INPUT);
    _servo.attach(SERVO_PIN);
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
    if(digitalRead(SENSOR_PIN==HIGH)){
      t=0; //cat's back
    }
    else{
      t++; //if cat isn't there, ++ time unit to later exit function 
    }
    
    }
  }
  

void Kattkran::identify_tap(){


}

