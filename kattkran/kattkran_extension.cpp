

#include "Arduino.h"
#include "kattkran_extension.h"
#include <Servo.h>

// TODO every function included in kattkran.h



void Kattkran::init(){
    pinMode(SENSOR_PIN,INPUT);
    
    _actuator0.attach(ACTUATOR_0_PIN);
    _actuator1.attach(ACTUATOR_1_PIN);
    pinMode(LED_BUILTIN, OUTPUT);
    _actuators_go_to_min();
    delay(2000);
    _servo.attach(SERVO_PIN);
    go_to_rest();
    delay(2000);
    _circulate(AWAY_ANGLE);
    delay(500);
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
    _circulate(AWAY_ANGLE,speed);
  }
  else{ //angle=false
  Serial.print("towardes tap\n");
    _circulate(TAP_ANGLE,speed);
  }


}

void Kattkran::go_to_rest() {
  Serial.print("Going to rest");

  byte const ROTATION_DEVIATION=10;//the amount of degres we can be from tap in ordet to requst rotatiron
  bool rotate;//is true if we are close to the tap and need ta rotate

  //if statments below tells us if we shoud rotate
  if (TAP_ANGLE-ROTATION_DEVIATION <_servo.read() &&  _servo.read()<ROTATION_DEVIATION+TAP_ANGLE){
    rotate=true;
    Serial.print(", with rotation");
    }
  else
    rotate=false ;

  if (rotate)// rotate if we shoud rotate
    _circulate(GOING_TO_REST_ROTATION_ANGLE);
  delay(500);
  //move the actuators simotainiously
  _actuator0.write(ACTUATOR_0_REST);
  _actuator1.write(ACTUATOR_1_REST);
  _wait_on_actuator(ACTUATOR_0_REST,A0);
  _wait_on_actuator(ACTUATOR_1_REST,A1);
  delay(1000);
 if (rotate)//rotate back again
  _circulate(TAP_ANGLE);

  Serial.print(".\n") ;
}

void Kattkran::turn_water_on() {
  //_circulate(TAP_ANGLE);
  //_actuators_go_to_min();
  Serial.println("Turning water on.");
  _move_actuator(ACTUATOR_1_OPEN_TAP,A1, &_actuator1);
  Serial.println("Actuator0 open tap");
  _move_actuator(ACTUATOR_0_OPEN_TAP,A0, &_actuator0);
  delay(2000);
  _move_actuator(PUMP_0_MIN, A0, &_actuator0);
}

void Kattkran::turn_water_off() {
  //_circulate(TAP_ANGLE);
  //_actuators_go_to_min();
  _circulate(GOING_TO_REST_ROTATION_ANGLE);
  delay(2000);
  Serial.println("Turning water off.");
  _move_actuator(ACTUATOR_0_CLOSE_TAP_1_MOVE,A0, NULL);
  delay(1000);
  _move_actuator(ACTUATOR_1_CLOSE_TAP,A1, NULL);
  delay(2000);
  _circulate(TAP_ANGLE);
  delay(1000);
  _move_actuator(PUMP_0_MIN,A0, NULL);
  delay(4000);
  _move_actuator(PUMP_0_MAX, A0, NULL);
  _circulate(GOING_TO_REST_ROTATION_ANGLE);
  _actuators_go_to_min();
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


void Kattkran::_circulate(byte goal_angle,byte speed){
  
  Serial.print("Circulating.");
  Serial.print(goal_angle, DEC);
  Serial.println("");
  byte current_angle = _servo.read();//the angle that vill change in the function, it start with the ltest writen value.
  Serial.print(current_angle, DEC);
  bool increase_angle;//if true increase curren_angle
  increase_angle=(current_angle<goal_angle);
  while (goal_angle != current_angle) {
    //this loop will change the curren_angle until curren_angle and goal is same

    _servo.write(current_angle) ;
    delay(speed) ;
    if (increase_angle)
      ++current_angle;
    else
      --current_angle;
  }
  Serial.println("Current angle");
  Serial.print(current_angle, DEC);
  Serial.print("Circulation ended.\n");
}

void Kattkran::_move_actuator(byte position, byte actuator_read_pin, Servo *actuator_object) {

  //the if statnts below ar kind of specifik to our use
  if (actuator_object == NULL){//if no object is given, go with actiator 0 or 1.
  if (actuator_read_pin==A0)
    actuator_object = & _actuator0;
  else
    actuator_object = & _actuator1;
  }
  (*actuator_object).write(position);
  delay(15);
  _wait_on_actuator(position, actuator_read_pin);

}

void Kattkran::_wait_on_actuator(byte position, byte actuator_read_pin) {

  int previous_analog_read = 0;//Saved analogRead from 100ms before
  int analog_read = analogRead(actuator_read_pin);
  while (_actuator_write_read_converter(analog_read ,false) < position) {
    if (previous_analog_read == analog_read) //Then we're stuck
      break;
    previous_analog_read = analog_read;

    //Wait until actuator is in final position
    delay(100);
    //It takes 100ms for the AD converter to convert signal.
    //Better just wait that time before reading signal again
    analog_read = analogRead(actuator_read_pin);
  }
}

void Kattkran::_actuators_go_to_min() {
  Serial.println("go to min");
  _move_actuator(PUMP_0_MIN, A0, &_actuator0);
  _wait_on_actuator(PUMP_0_MIN, A0);
  _move_actuator(PUMP_1_MIN, A1, &_actuator1);
  _wait_on_actuator(PUMP_0_MIN, A1);
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
