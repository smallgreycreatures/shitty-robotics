#ifndef kattkran_extension_h
#define kattkran_extension_h

#include "Arduino.h"
#include <Servo.h>

const byte SERVO_PIN=3;  //digital servo ut
const byte SENSOR_PIN=2; //digital in
const byte ACTUATOR_0_PIN=9; //simulerad analog fran digital
const byte ACTUATOR_1_PIN=10; //simulerad analog fran digital

//Final positions for Actuator 0 or 1 when open and close tap
const byte ACTUATOR_0_OPEN_TAP = 113;
const byte ACTUATOR_1_OPEN_TAP = 65;
const byte ACTUATOR_0_CLOSE_TAP_1_MOVE = 116;
const byte ACTUATOR_0_CLOSE_TAP_2_MOVE = 45;
const byte ACTUATOR_1_CLOSE_TAP = 65;

//Limits for the hydralic systems. Are set when actuator servos inits
const byte PUMP_0_MIN = 45;// MIN for small pump
const byte PUMP_0_MAX = 116; //MAX for small pump
const byte PUMP_1_MIN = 45;//MIN for big pump
const byte PUMP_1_MAX = 130;//MAX for big pump

//values for the positions when the actuators are in rest positions,
const byte ACTUATOR_0_REST = 45;
const byte ACTUATOR_1_REST = 65;

//time between the cat leaving and water tirnd off in ms
const int WAIT_TIME=10000;//TODO Change dummy value

//the rotating the servo will have in [ms/deg]
const byte SERVO_SPEED=50;//TODO Change dummy value

//the time the sensor is inactive after tap begin turned off
const int DELAY_AFTER_COMPLETION=3000;//TODO Change dummy value

//when going to rest you need to take a root around the tap whith help of servo
const byte GOING_TO_REST_ROTATION_ANGLE=125 ;//TODO Change dummy value

//below are the angle limits for the rotating base servo
const byte TAP_ANGLE=90;//TODO Change dummy value
const byte AWAY_ANGLE=45;//TODO Change dummy value

class Kattkran
{
public:

void init();
  /*runs tha satup/initialisation for the program
  */
bool sensor();
  /*this function return true if the sensor senses motion and
  change the state of _pir_state.
  note that the sensor output LOW when motion is detected.
  */

void circular_motion(bool direction,byte speed=SERVO_SPEED);
  /*den cirklara rorelsen mellan riktad mot kran och riktad fran kran.
  direction syftar pa om den ar riktad mot kranen och ska snurra fran kranen
  eller om den ar riktad fran kranen och ska snurra fran kranen
  rirtad fran kran -> mot kran => direction=false
  rirtad mot kran -> fran kran => direction=true
  speed syftar pa rotationshastigheten i ms/grad
  drivs enbart av servo
  */

void go_to_rest();
  /*make the arm go to a stable state with a stable center of mass.
  requires that the arm can bee in any position, meaning that it has to avoid
  potantialy turning the water on/off.
  driven by linear actuators.
  */

void turn_water_on();
/*turns on the water starting from its resting position and ends without going
  back to restposition
relys on actuators
*/

void turn_water_off();
  /*turns off the water starting from its resting position and ends without going
    back to restposition
  relys on actuators
  */

void time_limit();
  /*a loop that the program is stuck in until the cat has left and a cirtin amount
  of time has passed.

  depends on sensor,(wait_time)
  */

private:

  byte _actuator_0_position;//the analog value written to the linear actuators
  byte _actuator_1_position;//0cm=45, 10cm=141


  Servo _servo;
  int _pir_state = HIGH;//store the detected sensor input in this variable
  Servo _actuator0;
  Servo _actuator1;

  int _actuator_write_read_converter(int value,bool way=true);
  /*converts the angle from serve.wite(angle) to the expected value form
  analogWrite() or the same backwords
  if way is true get: input write angle and return analog read value
  if way=false :input is read analog and output is write angle
  */
  int _actuator_cm_to_servo_angle_converter(int value, bool way);
  /*
   * converts the lenght the actuator reaches out to the angle
   * value that is written to it.
   * bool=true -> cm to angle
   * bool=false -> angle to cm
   */
   void _circulate(byte goal_angle, byte speed = SERVO_SPEED);
   /*circulate the rotating servo(_servo) to goal_angle.
   The speed is in ms/deg.
   */

   void _move_actuator(byte position, byte actuator_read_pin, Servo *actuator_object=NULL );
   /*moves an actuator in servo mode to the argument position.
   the function run until the task is finished.
   position writes to actiator_object in servo mode.
   actuator_read_pin makes the asumption that actuator position and is related with _actuator_write_read_converter.
   actiator_object is the actuator writen to in servo mode.
   */

   void _wait_on_actuator(byte position, byte actuator_read_pin);
   /*waits until a actuator hase reached it's position.
   it breaks the funcition if the actiator stops moving.
   */

   void _actuators_go_to_min();

};



#endif
