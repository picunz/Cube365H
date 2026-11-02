#pragma once

#include <Arduino.h>

class PomodoroTimer
{
public:

    enum State
    {
        Idle,
        Work,
        ShortBreak,
        LongBreak,
        Paused
    };

    enum Event
    {
        None,
        WorkStarted,
        ShortBreakStarted,
        LongBreakStarted,
        Finished
    };

    PomodoroTimer();

    void begin(uint32_t workMin = 25,
               uint32_t shortBreakMin = 5,
               uint32_t longBreakMin = 15,
               uint8_t cycles = 4);

    void start();
    void stop();

    void pause();
    void resume();

    Event update();

    bool isRunning() const;
    bool isPaused() const;

    State state() const;

    uint32_t remainingSeconds() const;

    uint8_t progressDots(uint8_t totalDots) const;

    uint8_t cycle() const;

private:

    void reset();

    Event startWork();
    Event startShortBreak();
    Event startLongBreak();

    Event nextState();

    State _state;
    State _savedState;

    uint32_t _workTime;
    uint32_t _shortTime;
    uint32_t _longTime;

    uint32_t _duration;

    uint32_t _startMillis;
    uint32_t _pauseStart;

    uint8_t _cycle;
    uint8_t _maxCycles;
};