#include "Commands.h"
#include "Globals.h"
#include "RadioTerminal.h"
#include <IntervalTimer.h>
#undef min
#undef max
#include <functional>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

#define GETFLOAT(name) [&]{ return name; }
#define SETFLOAT(name) [&](float f){ name = f; }


struct Setter
{
    const char* name;
    std::function<void(float)> func;
};

struct Getter
{
    const char* name;
    std::function<float(void)> func;
};


const Setter setList[] =
{
    {"h", SETFLOAT(h)},
    {"d", SETFLOAT(d)},
    {"c1", SETFLOAT(c1)},
    {"c2", SETFLOAT(c2)},
    {"speed", SETFLOAT(speed)},
    {"kp", SETFLOAT(servoController.kp)},
    {"ki", SETFLOAT(servoController.ki)},
    {"kd", SETFLOAT(servoController.kd)},
    {"srv.cen", SETFLOAT(servo.center)},
    {"srv.upd", SETFLOAT(servo.usPerDegree)},
    {"srv.ul", SETFLOAT(servo.upperLimit)},
    {"srv.ll", SETFLOAT(servo.lowerLimit)},
    {"srv.deg", [&](float f){ servo.write(f); }},
    {"en", [&](float f){ controllerEnabled = f > 0.f; }},
};


const Getter getList[] =
{
    {"h", GETFLOAT(h)},
    {"d", GETFLOAT(d)},
    {"c1", GETFLOAT(c1)},
    {"c2", GETFLOAT(c2)},
    {"speed", GETFLOAT(speed)},
    {"kp", GETFLOAT(servoController.kp)},
    {"ki", GETFLOAT(servoController.ki)},
    {"kd", GETFLOAT(servoController.kd)},
    {"srv.cen", GETFLOAT(servo.center)},
    {"srv.upd", GETFLOAT(servo.usPerDegree)},
    {"srv.ul", GETFLOAT(servo.upperLimit)},
    {"srv.ll", GETFLOAT(servo.lowerLimit)},
    {"srv.deg", [&]{ return servo.read(); }},
    {"srv.pw", GETFLOAT(servo.pulseWidth)},
    {"ctrl", GETFLOAT(controllerOut)},
    {"en", [&]{ return float(controllerEnabled); }},
    {"vr", GETFLOAT(vr)},
    {"vl", GETFLOAT(vl)},
    {"xr", GETFLOAT(xr)},
    {"xl", GETFLOAT(xl)},
    {"x", [&]{ return float(x); }},
    {"but", [&]{ return float(button.pressed()); }},
    {"sw1", [&]{ return float(switch1.pressed()); }},
    {"sw2", [&]{ return float(switch2.pressed()); }},
};


class WatchHandler : public CmdHandler
{
public:
    WatchHandler(std::function<float(void)> wf);
    virtual void sendChar(char c) { timer.end(); RadioTerminal::terminateCmd(); }
    static std::function<float(void)> watchFun;
    static IntervalTimer timer;
    static void refresh();
};

std::function<float(void)> WatchHandler::watchFun;
IntervalTimer WatchHandler::timer;


CmdHandler* watch(const char* input)
{
    char buf[32];
    const int getListSize = (sizeof getList) / (sizeof getList[0]);

    const char* s = std::strchr(input, ' ');
    if (s != nullptr)
    {
        ++s;
        for (int i = 0; i < getListSize; ++i)
        {
            if (std::strncmp(s, getList[i].name, 32) == 0)
                return new WatchHandler(getList[i].func);
        }
    }

    RadioTerminal::write("Usage: w <var>\nValid vars:");
    for (int i = 0; i < getListSize; ++i)
    {
        snprintf(buf, 32, "\n  %s", getList[i].name);
        RadioTerminal::write(buf);
    }
    return nullptr;
}


WatchHandler::WatchHandler(std::function<float(void)> wf)
{
    watchFun = wf;
    timer.begin(&WatchHandler::refresh, 200000);
}


void WatchHandler::refresh()
{
    char output[256];

    sprintf(output, "\r         \r\r\r%4.4f", watchFun());
    RadioTerminal::write(output);
}


CmdHandler* print(const char* input)
{
    char buf[32];
    const int getListSize = (sizeof getList) / (sizeof getList[0]);
    
    const char* s = std::strchr(input, ' ');
    if (s != nullptr)
    {
        ++s;
        for (int i = 0; i < getListSize; ++i)
        {
            if (std::strncmp(s, getList[i].name, 32) == 0)
            {
                snprintf(buf, 32, "%s = %4.4f",
                         getList[i].name,
                         getList[i].func());
                RadioTerminal::write(buf);
                return nullptr;
            }
        }
    }
    
    RadioTerminal::write("Usage: p <var>\nValid variables:");
    for (int i = 0; i < getListSize; ++i)
    {
        snprintf(buf, 32, "\n  %s", getList[i].name);
        RadioTerminal::write(buf);
    }
    return nullptr;
}


CmdHandler* set(const char* input)
{
    char buf[32];
    const int setListSize = (sizeof setList) / (sizeof setList[0]);
    
    const char* s = std::strchr(input, ' ');
    if (s != nullptr)
    {
        const char* s2 = std::strchr(++s, ' ');
        if (s2 != nullptr)
        {
            int pslen = s2 - s;
            float value = strtof(++s2, nullptr);
            
            for (int i = 0; i < setListSize; ++i)
            {
                if (std::strncmp(s, setList[i].name, pslen) == 0)
                {
                    setList[i].func(value);
                    snprintf(buf, 32, "%s = %4.4f",
                             setList[i].name,
                             value);
                    RadioTerminal::write(buf);
                    return nullptr;
                }
            }
        }
    }
    
    RadioTerminal::write("Usage: s <var> <value>\nValid variables:");
    for (int i = 0; i < setListSize; ++i)
    {
        snprintf(buf, 32, "\n  %s", setList[i].name);
        RadioTerminal::write(buf);
    }
    return nullptr;
}


void setupCommands()
{
    RadioTerminal::addCommand("w", &watch);
    RadioTerminal::addCommand("p", &print);
    RadioTerminal::addCommand("s", &set);
}


// Workaround for teensyduino/libstdc++ bug
void std::__throw_bad_function_call() { while(true) {} };
