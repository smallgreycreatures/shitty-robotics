

#include "Arduino.h"
#include "kattkran_extension.h"
#include <Servo.h>

// TODO every function included in kattkran.h



void Kattkran::init(){
    pinMode(SENSOR_PIN,INPUT);
    _servo.attach(SERVO_PIN);
    _actuator0.attach(ACTUATOR_0_PIN, PUMP_0_MIN, PUMP_0_MAX);
    _actuator1.attach(ACTUATOR_1_PIN, PUMP_1_MIN, PUMP_1_MAX);
    pinMode(LED_BUILTIN, OUTPUT);
}


bool Kattkran::sensor(){

  // NOTE currently the internal led active when motion is detected
  // NOTE the sensor react with a LOW signal

  if (digitalRead(SENSOR_PIN) == LOW) {// check if the input is active
    //digitalWrite(LED_BUILTIN, HIGH);  // turn LED ON
    if (_pir_state == HIGH) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      _pir_state = LOW;
    }
    return true;
  }
  else {//no motion detected
    //digitalWrite(LED_BUILTIN, LOW); // turn LED OFF
    if (_pir_state == LOW){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      _pir_state = HIGH;
    }
    return false;
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
  // TODO skriv om, denna ar gjord pa en halvtimma...
  bool below;

  analogRead(A0);
  delay(120);
  _actuator_0_position=_actuator_write_read_converter(analogRead(A0),false);

  analogRead(A1);
  delay(120);
  _actuator_1_position=_actuator_write_read_converter(analogRead(A1),false);
  if (_actuator_0_position < ACTUATOR_0_CLOSE_TAP && _actuator_1_position < ACTUATOR_1_CLOSE_TAP )
    below=true;
  else
    below=false;
  if (below){
    _actuator0.write(PUMP_0_MIN);
    _actuator1.write(PUMP_1_MIN);
    while (_actuator_0_position > (PUMP_0_MIN+10) || _actuator_1_position > (PUMP_1_MIN+10) ) {
      analogRead(A0);
      delay(120);
      _actuator_0_position=_actuator_write_read_converter(analogRead(A0),false);

      analogRead(A1);
      delay(120);
      _actuator_1_position=_actuator_write_read_converter(analogRead(A1),false);
    }
  }
  else{
    _actuator0.write(PUMP_0_MAX);
    _actuator1.write(PUMP_1_MAX);
    while (_actuator_0_position < (PUMP_0_MAX-10) || _actuator_1_position < (PUMP_1_MAX-10) ) {
      analogRead(A0);
      delay(120);
      _actuator_0_position=_actuator_write_read_converter(analogRead(A0),false);

      analogRead(A1);
      delay(120);
      _actuator_1_position=_actuator_write_read_converter(analogRead(A1),false);
    }
  }
  _actuator0.write(ACTUATOR_0_REST);
  _actuator1.write(ACTUATOR_1_REST);
  while (_actuator_0_position != ACTUATOR_0_REST || _actuator_1_position != ACTUATOR_0_REST) {
    analogRead(A0);
    delay(120);
    _actuator_0_position=_actuator_write_read_converter(analogRead(A0),false);

    analogRead(A1);
    delay(120);
    _actuator_1_position=_actuator_write_read_converter(analogRead(A1),false);
  }
}

void Kattkran::turn_water_on() {
  _actuator0.write(ACTUATOR_0_OPEN_TAP); //Write desired position to actuator 0
  delay(15);

  int previous_analog_read = 0; //Saved analogRead from 100ms before
  while (_actuator_write_read_converter(analogRead(A0),false) < ACTUATOR_0_OPEN_TAP) {

    if (previous_analog_read == analogRead(A0)) //Then we're stuck
      break;
    previous_analog_read = analogRead(A0);
    //Wait until actuator is in final position
    delay(100);
    //It takes 100ms for the AD converter to convert signal.
    //Better just wait that time before reading signal again
  }

  _actuator1.write(ACTUATOR_1_OPEN_TAP); //Write desired position to actuator 1
  delay(15);
  previous_analog_read = 0;
  while (_actuator_write_read_converter(analogRead(A1),false) < ACTUATOR_1_OPEN_TAP) {
    if (previous_analog_read == analogRead(A0)) //Then we're stuck
      break;
    previous_analog_read = analogRead(A0);

    delay(100);
  }

}

void Kattkran::turn_water_off() {
  _actuator0.write(ACTUATOR_0_CLOSE_TAP); //Write desired position to actuator 0
  delay(15);

  int previous_analog_read = 0;//Saved analogRead from 100ms before
  while (_actuator_write_read_converter(analogRead(A0),false) < ACTUATOR_0_CLOSE_TAP) {
    if (previous_analog_read == analogRead(A0)) //Then we're stuck
      break;
    previous_analog_read = analogRead(A0);

    //Wait until actuator is in final position
    delay(100);
    //It takes 100ms for the AD converter to convert signal.
    //Better just wait that time before reading signal again

  }

  _actuator1.write(ACTUATOR_1_CLOSE_TAP); //Write desired position to actuator 1
  delay(15);
  previous_analog_read = 0;
  while (_actuator_write_read_converter(analogRead(A1),false) < ACTUATOR_1_CLOSE_TAP) {
    if (previous_analog_read == analogRead(A0)) //Then we're stuck
      break;
    previous_analog_read = analogRead(A0);
    delay(100);
  }
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

int Kattkran::_actuator_write_read_converter(int value,bool way){
  /* eqation is expected to be linear and described as f(x)=ax+b when way=true
    Y=-6.82x+974
    X=(y-974)/-6.82
  */

  const float a=-6.82, b=974;
  if (way)
    return ((int) ((a*value+b) +0.5));//the +0.5 makes c++ round properly
  else
    return ((int) (((value+b)/a) +0.5));
}
