

#include "Arduino.h"
#include "kattkran_extension.h"
#include <Servo.h>

// TODO every function included in kattkran.h



void Kattkran::init(){
    pinMode(SENSOR_PIN,INPUT);
    _servo.attach(SERVO_PIN);
    _actuator0.attach(ACTUATOR_0_PIN);
    _actuator1.attach(ACTUATOR_1_PIN);
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

  Serial.print("Circulating ");
  if (direction) {//this if/else determen the goal angle
    Serial.print("away from tap\n");
    _circulate(_away_angle,speed);
  }
  else{ //angle=false
  Serial.print("towardes tap\n");
    _circulate(_tap_angle,speed);
  }


}

void Kattkran::go_to_rest() {
  Serial.print("Going to rest");

  byte const ROTATION_DEVIATION=10;
  bool rotate;

  if (_tap_angle-ROTATION_DEVIATION <_servo.read() &&  _servo.read()<ROTATION_DEVIATION+_tap_angle){
    rotate=true;
    Serial.print(", with rotation");
    }
  else
    rotate=false ;

  if (rotate)
    _circulate(GOING_TO_REST_ROTATION_ANGLE);

  _actuator0.write( ACTUATOR_0_REST);
  _actuator1.write(ACTUATOR_1_REST);
  _wait_on_actuator(ACTUATOR_0_REST,0);
  _wait_on_actuator(ACTUATOR_1_REST,1);

 if (rotate)
  _circulate(_tap_angle);

  Serial.print(".\n") ;
}

void Kattkran::turn_water_on() {

  Serial.println("Turning water on.");
  _move_actuator(ACTUATOR_1_OPEN_TAP,1);


  _move_actuator(ACTUATOR_0_OPEN_TAP,0);

}

void Kattkran::turn_water_off() {

  Serial.println("Turning water off.");
  _move_actuator(ACTUATOR_0_CLOSE_TAP_1_MOVE,0);

  _move_actuator(ACTUATOR_1_CLOSE_TAP,1);
  _move_actuator(ACTUATOR_0_CLOSE_TAP_2_MOVE,0);

}

void Kattkran::time_limit(){
  Serial.println("Waiting on cat to leave.");
  int t=0; //time unit
  while(t<WAIT_TIME){
    if(sensor())
      t=0; //cat's back

    else{
      delay(100);
      t+=100; //if cat isn't there, ++ time unit to later exit function
    }

  }
}


void Kattkran::identify_tap(){


}
void Kattkran::_circulate(byte goal_angle,byte speed){

  Serial.println("Circulating");

  byte start_angle=_servo.read();//the angle the servo has when the function is calld
  byte current_angle= start_angle;//the angle that vill change in the function

  bool increase_angle;//if true increase curren_angle
  increase_angle=(start_angle<goal_angle);
  while (goal_angle != current_angle) {
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

void Kattkran::_move_actuator(byte position,byte actuator_number) {
  Servo *actuator_object;//kind of unnecisarry to do this way...

  if (actuator_number==1)
    actuator_object = & _actuator1;
  else
    actuator_object = & _actuator0;

  (*actuator_object).write(position);
  delay(15);
  _wait_on_actuator(position,actuator_number);
}

void Kattkran::_wait_on_actuator(byte position,byte actuator_number) {

  byte actuator_pin=A0+actuator_number;
  int previous_analog_read = 0;//Saved analogRead from 100ms before


  while (_actuator_write_read_converter(analogRead(actuator_pin),false) < position) {
    if (previous_analog_read == analogRead(actuator_pin)) //Then we're stuck
      break;
    previous_analog_read = analogRead(actuator_pin);

    //Wait until actuator is in final position
    delay(100);
    //It takes 100ms for the AD converter to convert signal.
    //Better just wait that time before reading signal again
  }
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
    return ((int) (((value-b)/a) +0.5));
}

int Kattkran::_actuator_cm_to_servo_angle_converter(int value, bool way) {
  int start_val = 45;
  float factor = 9.6;

  if (way == true) { //cm to servo val
    return ((int) (start_val+(value*factor)));
  } else {
    return ((int) ((value-start_val)/factor));
  }

}
