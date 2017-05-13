

#include "kattkran_extension.h"
#include <Servo.h>

bool is_water_on=false ;

Kattkran kattkran ;

void setup () {
  Serial.begin(9600);
  kattkran.init();
  
  delay(DELAY_AFTER_COMPLETION);
}

void loop() {
/*
see flow shart for execution order
QUESTION does the loop look good?
*/

  if (!is_water_on && kattkran.sensor()){//turn the water on
    kattkran.go_to_rest() ;
    delay(500);
    //restposition probebly pointing away from tap
    kattkran.circular_motion(false);
    delay(500);
    //resting position pointing towardes tap
    kattkran.turn_water_on();
    delay(500);
    is_water_on=true ;
    kattkran.go_to_rest();
    delay(500);
  }
  else if (is_water_on){
    kattkran.time_limit() ;
    delay(500);
    kattkran.turn_water_off();
    delay(500);
    is_water_on=false;
    kattkran.go_to_rest();

    delay(4000);
    kattkran.circular_motion(true);
    delay(4000);
    delay(DELAY_AFTER_COMPLETION);
  }
}
