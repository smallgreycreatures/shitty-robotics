

#include "kattkran_extension.h"
#include <Servo.h>

bool is_water_on=false ;

Kattkran kattkran ;

void setup () {
  kattkran.init();
  Serial.begin(9600);
  delay(DELAY_AFTER_COMPLETION);
}

void loop() {
/*
see flow shart for execution order
QUESTION does the loop look good?
*/
  kattkran.go_to_rest();
  delay(5000);
  kattkran.turn_water_on();
  delay(000);
  kattkran.go_to_rest();
  delay(5000);
  kattkran.turn_water_off();
  delay(5000);
/*
  if (!is_water_on && kattkran.sensor()){//turn the water on
    kattkran.go_to_rest() ;
    //restposition probebly pointing away from tap
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
    delay(DELAY_AFTER_COMPLETION);
  }*/
}
