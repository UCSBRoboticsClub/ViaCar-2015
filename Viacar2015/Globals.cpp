#include "Globals.h"
#include <Arduino.h>


float vr = 0.f;
float vl = 0.f;
float xr = 0.f;
float xl = 0.f;
LowPass x;
float h = 0.06f;
float d = 0.13f;
float c1 = 0.2f;
float c2 = 4.4e3f;
float speed = 0.3f;
float controllerOut = 0.f;
bool controllerEnabled = true;
float xmax = 0.3f;
LowPass minScore;
float scoreLimit = 0.1f;

Motor motor(motorFwdPin, motorBackPin, motorSpeedPin);
Servo servo(servoPin);
Button button(buttonPin, LOW, true);
Button switch1(switch1Pin, LOW, true);
Button switch2(switch2Pin, LOW, true);
PIDController servoController(dt);
