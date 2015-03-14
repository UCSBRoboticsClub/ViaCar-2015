#include "Globals.h"
#include <Arduino.h>


float vr = 0.f;
float vl = 0.f;
float xr = 0.f;
float xl = 0.f;
LowPass x;
float h = 0.01f;
float d = 0.10f;
float c1 = 0.2f;
float c2 = 4.4e3f;
float speed = 0.3f;
float controllerOut = 0.f;
bool controllerEnabled = true;

Motor motor(motorFwdPin, motorBackPin, motorSpeedPin);
Servo servo(servoPin);
Button calSwitch(switchPin, LOW, true);
PIDController servoController(dt);
