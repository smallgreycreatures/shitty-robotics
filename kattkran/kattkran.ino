

#include "kattkran_extension.h"
#include <Servo.h>

bool is_water_on=false ;

Kattkran kattkran ;

void setup () {
  pinMode(SENSOR_PIN,INPUT);
}

void loop() {
/*
see flow shart for execution order
QUESTION does the loop look good?
*/

  if (!is_water_on && (digitalRead(SENSOR_PIN)==HIGH)){//turn the water on
    kattkran.go_to_rest() ;
    //restposition probebly pointing away from tap
    kattkran.identify_tap() ;
    kattkran.circular_motion(false);
    //resting position pointing towardes tap
    kattkran.turn_water_on();
    is_water_on=true ;
    kattkran.go_to_rest();
  }
  else if (is_water_on){
    kattkran.time_limit() ;
    kattkran.turn_water_off();
    is_water_on=false;
    kattkran.go_to_rest();
    kattkran.circular_motion(true);
  }
}
