

#include "Arduino.h"
#include "kattkran_extension.h"
#include <Servo.h>

// TODO every function included in kattkran.h





void circular_motion(bool direction,byte speed){
  // TODO verify circular_motion function
  /*

  */
    byte angle_goal;//the angle that the servo shoud end on
    byte start_angle;//the angle the servo has when the function is calld
    byte curren_angle;//the angle that vill change in the function

    bool increase_angle;//if true increase curren_angle

    start_angle=servo.read() ;

    if (direction) //this if/else determen the goal angle
      angle_goal=away_angle;
    else //angle=false
      angle_goal=tap_angle;

    increase_angle=(start_angle < angle_goal);

    curren_angle=start_angle;

    while (angle_goal - curren_angle){/*
      this loop will change the curren_angle until curren_angle and goal is same
      */
      servo.write(curren_angle) ;
      delay(speed) ;
      if (increase_angle)
        ++curren_angle;
      else
        --curren_angle;
    }
}
