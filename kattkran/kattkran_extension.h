#ifndef kattkran_extension_h
#define kattkran_extension_h

#include "Arduino.h"
#include <Servo.h>

const byte SERVO_PIN=3;  //digital servo ut
const byte SENSOR_PIN=2; //digital in
const byte ACTUATOR_0_PIN=9; //simulerad analog fran digital
const byte ACTUATOR_1_PIN=10; //simulerad analog fran digital

//Final positions for Actuator 0 or 1 when open and close tap
const byte ACTUATOR_0_OPEN_TAP = 80; //TODO Change dummy value
const byte ACTUATOR_1_OPEN_TAP = 90; //TODO Change dummy value
const byte ACTUATOR_0_CLOSE_TAP_1_MOVE = 90;//TODO Change dummy value
const byte ACTUATOR_0_CLOSE_TAP_2_MOVE = 45;
const byte ACTUATOR_1_CLOSE_TAP = 66;//TODO Change dummy value

//Limits for the hydralic systems. Are set when actuator servos inits
const byte PUMP_0_MIN = 45; //TODO Change dummy value
const byte PUMP_0_MAX = 141;//TODO Change dummy value
const byte PUMP_1_MIN = 45;//TODO Change dummy value
const byte PUMP_1_MAX = 141;//TODO Change dummy value

//values for the positions when the actuators are in rest positions,
const byte ACTUATOR_0_REST = 50;//TODO Change dummy value
const byte ACTUATOR_1_REST = 50;//TODO Change dummy value

const int WAIT_TIME=10000;//time between the cat leaving and water tirnd off in ms
const byte SERVO_SPEED=20;//the rotating the servo will have in [ms/deg]
const int DELAY_AFTER_COMPLETION=3000;//the time the sensor is inactive after tap begin turned off

const byte GOING_TO_REST_ROTATION_ANGLE=30 ;//when going to rest you need to take a root around the tap whith help of servo

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

void identify_tap();
  /*
  QUESTION :what does this do
  i don't know what this does but Frans told me it shoud be here
  */
private:
  //the two variables below are detrenmend by identify_tap?
  byte _away_angle=160   ;//angle that the arm shoud start pointig to
  byte _tap_angle=10  ;//angle the arm is pointing to when its pointing towardes the tap

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
   void _circulate(byte goal_angle, byte speed=SERVO_SPEED);
   /*circulate the rotating servo to goal_angle.
   The speed is in ms/deg.
   */

   void _move_actuator(byte position, byte actuator_number);
   /*moves an actuator in servo mode to the argument position.
   the function run until the task is finished.
   */

   void _wait_on_actuator(byte position, byte actuator_number);
   /*waits until a actuator hase reached it's position.
   it breaks the funcition if the actiator stops moving.
   */

};



#endif
