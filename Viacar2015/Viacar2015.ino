#include <IntervalTimer.h>
#include <SPI.h>
#include "Globals.h"
#include "RadioTerminal.h"
#include "Commands.h"
#include "ADC.h"
#include <cmath>


IntervalTimer controlTimer;
ADC adc;

void controlLoop();
float getPosition();
float volt2dist();


void setup()
{
    v1.setCutoffFreq(50.f, dt);
    v2.setCutoffFreq(50.f, dt);
    xdot.setTimeConst(50.f, dt);

    adc.setResolution(12);
    adc.setConversionSpeed(ADC_HIGH_SPEED);
    adc.setSamplingSpeed(ADC_HIGH_SPEED);
    adc.setAveraging(16);
    
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
    x = getPosition(x);
}


float getPosition()
{
    vr = adc.analogRead(sensorRPin) / 4096.f * 3.3f;
    vl = adc.analogRead(sensorLPin) / 4096.f * 3.3f;

    xr = volt2dist(vr);
    xl = volt2dist(vl);

    float candidates[] =
    {
        (+xr + +xl) * 0.5f,
        (-xr + +xl) * 0.5f,
        (-xr + -xl) * 0.5f,
    };

    float diffScores[] =
    {
        std::fabs(xr - xl + d),
        std::fabs(-xr - xl + d),
        std::fabs(-xr + xl + d),
    };


float volt2dist(float v)
{
    return 0.f;
}

