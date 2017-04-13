
#include <Servo.h>



//notera nedanstaende konstanterkan andras
const byte servo_pin=6 ;  //digital servo ut
const byte sensor_pin=2 ; //digital in
const byte actuator_1_pin=5 ; //simulerad analog fran digital
const byte actuator_2_pin=3 ; //simulerad analog fran digital
const int start_angle=   ;//angle that the arm shoud start pointig to
const int tap_angle=  ;//angle the arm is pointing to when its pointing towardes the tap

static bool is_water_on=true ;

Servo servo ;

/*var sensor fnk
digitalRead(sensor_pin) ;
*/


void circular_motion(bool direction,int angle){
  /*den cirklara rorelsen mellan riktad mot kran och riktad fran kran.
  direction syftar pa om den ar riktad mot kranen och ska snurra fran kranen
  eller om den ar riktad fran kranen och ska snurra fran kranen
  rirtad fran kran -> mot kran => direction=false
  rirtad mot kran -> fran kran => direction=true
  angle
  drivs enbart av servo
  */


}

void go_to_rest(){
  /*make the arm go to a stable state with a stable center of mass.
  requires that the arm can bee in any position, meaning that it has to avoid
  potantialy turning the water on/off.
  driven by linear actuators.
  */
}


void turn_water_on(){
/*turns on the water starting from its resting position and ends without going
  back to restposition
relys on actuators
*/
}

void turn_water_off(){
  /*turns off the water starting from its resting position and ends without going
    back to restposition
  relys on actuators
  */
}

void time_limit(){
  /*a loop that the program is stuck in until the cat has left and a cirtin amount
  of time has passed.

  depends on sensor
  */
}
void identify_tap(){
  /*
  i don't know what this does but Frans told me it shoud be here
  */
}



void setup () {
  pinMode(sensor_pin,INPUT) ;
  servo.attach(servo_pin);
  /*ej krav for analog signal
  pinMode(actuator_1_pin,OUTPUT)
  pinMode(actuator_2_pin,OUTPUT)
  */
}

void loop() {
/*
see flow shart for execution order

*/



}
