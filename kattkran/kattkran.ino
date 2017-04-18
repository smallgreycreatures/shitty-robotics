

#include "kattkran_extension.h"
#include <Servo.h>



void setup () {
  // QUESTION is the setup correct?

  pinMode(SENSOR_PIN,INPUT) ;
  servo.attach(SERVO_PIN);
  /*ej krav for analog signal
  pinMode(ACTUATOR_1_PIN,OUTPUT)
  pinMode(ACTUATOR_2_PIN,OUTPUT)
  */
}

void loop() {
/*
see flow shart for execution order
QUESTION does the loop look good?
*/

  if (!is_water_on && (digitalRead(SENSOR_PIN)==HIGH)){//turn the water on
    go_to_rest() ;
    //restposition probebly pointing away from tap
    identify_tap() ;
    circular_motion(false);
    //resting position pointing towardes tap
    turn_water_on();
    is_water_on=true ;
    go_to_rest();
  }
  else if (is_water_on){
    time_limit() ;
    turn_water_off();
    is_water_on=false;
    go_to_rest();
    circular_motion(true);
  }
}
