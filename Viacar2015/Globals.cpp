#include "Globals.h"
#include <Arduino.h>


float vr = 0.f;
float vl = 0.f;
float xr = 0.f;
float xl = 0.f;
LowPass x;
float h = 0.01f;
float d = 0.10f;
float c1 = 253.f;
float c2 = 2.6f;
float speed = 0.3f;

Motor motor(motorFwdPin, motorBackPin, motorSpeedPin);
Servo steering(servoPin);
Button calSwitch(switchPin, LOW, true);
PIDController servoController(dt);
