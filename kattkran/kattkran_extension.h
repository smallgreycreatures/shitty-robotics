#ifndef kattkran_extension_h
#define kattkran_extension_h

#include "Arduino.h"
#include <Servo.h>

const byte SERVO_PIN=3;  //digital servo ut
const byte SENSOR_PIN=2 ; //digital in
const byte ACTUATOR_1_PIN=5 ; //simulerad analog fran digital
const byte ACTUATOR_2_PIN=3 ; //simulerad analog fran digital

const int WAIT_TIME=10000;//time between the cat leaving and water tirnd off in ms
const byte SERVO_SPEED=20;//the rotating the servo will have in [ms/deg]
const int DELAY_AFTER_COMPLETION=3000;//the time the sensar is inactive after tap begin tirnd off

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

  byte _actuator_1_position;//the analog value written to the linear actuators
  byte _actuator_2_position;//0->0V  , 255->5V


  Servo _servo ;
  int _pir_state = HIGH;//store the detected sensor input in this variable

};



#endif
