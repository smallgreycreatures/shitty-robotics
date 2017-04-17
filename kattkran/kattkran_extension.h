

#include "Arduino.h"
#include <Servo.h>

const byte servo_pin=6 ;  //digital servo ut
const byte sensor_pin=2 ; //digital in
const byte actuator_1_pin=5 ; //simulerad analog fran digital
const byte actuator_2_pin=3 ; //simulerad analog fran digital

const int wait_time=10000;//time between the cat leaving and water tirnd off in ms
const byte servo_speed=20;//the rotating the servo will have in [ms/deg]

//the two variables below are detrenmend by identify_tap?
static byte away_angle=160   ;//angle that the arm shoud start pointig to
static byte tap_angle=10  ;//angle the arm is pointing to when its pointing towardes the tap

static byte actuator_1_position;//the analog value written to the linear actuators
static byte actuator_2_position;//0->0V  , 255->5V

static bool is_water_on=false ;

Servo servo ;


void circular_motion(bool direction,byte speed=servo_speed);
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
