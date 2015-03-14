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


struct Parameter
{
    const char* name;
    float* var;
};

struct Variable
{
    const char* name;
    std::function<float(void)> func;
};


const Parameter paramList[] =
{
    {"h", &h},
    {"d", &d},
    {"c1", &c1},
    {"c2", &c2},
    {"speed", &speed},
};


const Variable varList[] =
{
    {"vr", [&]{ return vr; }},
    {"vl", [&]{ return vl; }},
    {"xr", [&]{ return xr; }},
    {"xl", [&]{ return xl; }},
    {"x", [&]{ return float(x); }},
    {"calsw", [&]{ return calSwitch.pressed(); }},
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
    const int varListSize = (sizeof varList) / (sizeof varList[0]);

    const char* s = std::strchr(input, ' ');
    if (s != nullptr)
    {
        ++s;
        for (int i = 0; i < varListSize; ++i)
        {
            if (std::strncmp(s, varList[i].name, 32) == 0)
                return new WatchHandler(varList[i].func);
        }
    }

    RadioTerminal::write("Usage: w <var>\nValid vars:");
    for (int i = 0; i < varListSize; ++i)
    {
        snprintf(buf, 32, "\n  %s", varList[i].name);
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
    const int paramListSize = (sizeof paramList) / (sizeof paramList[0]);
    
    const char* s = std::strchr(input, ' ');
    if (s != nullptr)
    {
        ++s;
        for (int i = 0; i < paramListSize; ++i)
        {
            if (std::strncmp(s, paramList[i].name, 32) == 0)
            {
                snprintf(buf, 32, "%s = %4.4f",
                         paramList[i].name,
                         *(paramList[i].var));
                RadioTerminal::write(buf);
                return nullptr;
            }
        }
    }
    
    RadioTerminal::write("Usage: p <param>\nValid parameters:");
    for (int i = 0; i < paramListSize; ++i)
    {
        snprintf(buf, 32, "\n  %s", paramList[i].name);
        RadioTerminal::write(buf);
    }
    return nullptr;
}


CmdHandler* set(const char* input)
{
    char buf[32];
    const int paramListSize = (sizeof paramList) / (sizeof paramList[0]);
    
    const char* s = std::strchr(input, ' ');
    if (s != nullptr)
    {
        const char* s2 = std::strchr(++s, ' ');
        if (s2 != nullptr)
        {
            int pslen = s2 - s;
            float value = strtof(++s2, nullptr);
            
            for (int i = 0; i < paramListSize; ++i)
            {
                if (std::strncmp(s, paramList[i].name, pslen) == 0)
                {
                    *(paramList[i].var) = value;
                    snprintf(buf, 32, "%s = %4.4f",
                             paramList[i].name,
                             *(paramList[i].var));
                    RadioTerminal::write(buf);
                    return nullptr;
                }
            }
        }
    }
    
    RadioTerminal::write("Usage: s <param> <value>\nValid parameters:");
    for (int i = 0; i < paramListSize; ++i)
    {
        snprintf(buf, 32, "\n  %s", paramList[i].name);
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
