#include "Globals.h"
#include <Arduino.h>


LowPass x;
float xmax = 0.5f;
float xdot = 0.f;
float speed = 0.4f;
float vel = 0.f;
float velfch = 0.995f;
float velfcl = 0.999f;
float theta = 0.f;
float thetaint = 0.f;
LowPass thetalp;
float thetamax = 1.f;
float curvature = 0.f;
float kp = 100.f;
float kd = 30.f;
bool controllerEnabled = true;
float vr = 0.f;
float vl = 0.f;
float xr = 0.f;
float xl = 0.f;
float h = 0.06f;
float d = 0.13f;
float c1 = 0.2f;
float c2 = 4.4e3f;
LowPass minScore;
float scoreLimit = 0.1f;

Motor motor(motorFwdPin, motorBackPin, motorSpeedPin);
Servo servo(servoPin);
Button button(buttonPin, LOW, true);
Button switch1(switch1Pin, LOW, true);
Button switch2(switch2Pin, LOW, true);
