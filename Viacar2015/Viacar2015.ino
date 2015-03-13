#include <IntervalTimer.h>
#include <SPI.h>
#include "Globals.h"
#include "RadioTerminal.h"
#include "Commands.h"
#include "./Servo.h"
#include "VNH5019.h"
#include "Button.h"


IntervalTimer controlTimer;

void controlLoop();


void setup()
{
    controlTimer.begin(controlLoop, controlPeriodUs);
    controlTimer.priority(144);

    RadioTerminal::initialize();
    setupCommands();
}


void loop()
{
    delay(10);
}


void controlLoop()
{
}


