

#include "Arduino.h"
#include "kattkran_extension.h"
#include <Servo.h>

// TODO every function included in kattkran.h





void circular_motion(bool direction, byte speed) {
  // TODO verify circular_motion function
  /*

  */
  byte angle_goal;//the angle that the servo shoud end on
  byte start_angle;//the angle the servo has when the function is calld
  byte current_angle;//the angle that vill change in the function

  bool increase_angle;//if true increase curren_angle

  start_angle = servo.read() ;

  if (direction) //this if/else determen the goal angle
    angle_goal = away_angle;
  else //angle=false
    angle_goal = tap_angle;

  increase_angle = (start_angle < angle_goal);

  current_angle = start_angle;

  while (angle_goal - current_angle) {
    /*
      this loop will change the curren_angle until curren_angle and goal is same
    */
    servo.write(current_angle) ;
    delay(speed) ;
    if (increase_angle)
      ++current_angle;
    else
      --current_angle;
  }
}

void go_to_rest() {

}

void turn_water_on() {

}

void turn_water_off() {

}

void time_limit(){
   
//turns off depending on wait_time
if(digitalRead(SENSOR_PIN==HIGH)){ //if cat is still there
  delay(wait_time);
  time_limit(); //go back to function to test statement again
  
  
  
}
else{ //if cat dissappear 
 
 int t=0, waiting_time=10000; //time unit
  while(t<waiting_time){
    if(digitalRead(SENSOR_PIN==LOW)){
      t++; //if cat isn't there, ++ time unit to later exit function 
    }
    else{
      time_limit(); //if cat appear again go back to function 
    }
  }
  
}




}

void identify_tap(){

}
