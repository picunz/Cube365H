#include "PomodoroTimer.h"

PomodoroTimer::PomodoroTimer()
{
    reset();
}

void PomodoroTimer::begin(uint32_t workMin,
                          uint32_t shortBreakMin,
                          uint32_t longBreakMin,
                          uint8_t cycles)
{
    _workTime  = workMin * 60000UL;
    _shortTime = shortBreakMin * 60000UL;
    _longTime  = longBreakMin * 60000UL;
    _maxCycles = cycles;
}

void PomodoroTimer::start()
{
    _cycle = 1;
    startWork();
}

void PomodoroTimer::stop()
{
    reset();
}

void PomodoroTimer::pause()
{
    if (_state == Work ||
        _state == ShortBreak ||
        _state == LongBreak)
    {
        _pauseStart = millis();
        _savedState = _state;
        _state = Paused;
    }
}

void PomodoroTimer::resume()
{
    if (_state != Paused)
        return;

    _startMillis += millis() - _pauseStart;
    _state = _savedState;
}

PomodoroTimer::Event PomodoroTimer::update()
{
    if (_state == Idle || _state == Paused)
        return None;

    if (millis() - _startMillis >= _duration) {
        return nextState();
    }

    return None;
}

bool PomodoroTimer::isRunning() const
{
    return _state != Idle;
}

bool PomodoroTimer::isPaused() const
{
    return _state == Paused;
}

PomodoroTimer::State PomodoroTimer::state() const
{
    return _state;
}

uint8_t PomodoroTimer::cycle() const
{
    return _cycle;
}

uint32_t PomodoroTimer::remainingSeconds() const
{
    if (_state == Idle)
        return 0;

    uint32_t elapsed;

    if (_state == Paused) {
        elapsed = _pauseStart - _startMillis;
    }
    else {
        elapsed = millis() - _startMillis;
    }

    if (elapsed >= _duration) {
        return 0;
    }

    return (_duration - elapsed) / 1000;
}

uint8_t PomodoroTimer::progressDots(uint8_t totalDots) const
{
    if (_state == Idle)
        return 0;

    uint32_t elapsed;

    if (_state == Paused)
        elapsed = _pauseStart - _startMillis;
    else
        elapsed = millis() - _startMillis;

    if (elapsed > _duration)
        elapsed = _duration;

    return (elapsed * totalDots) / _duration;
}

void PomodoroTimer::reset()
{
    _state = Idle;
    _savedState = Idle;

    _cycle = 0;

    _duration = 0;

    _startMillis = 0;
    _pauseStart = 0;
}

PomodoroTimer::Event PomodoroTimer::startWork()
{
    _state = Work;
    _duration = _workTime;
    _startMillis = millis();

    return WorkStarted;
}

PomodoroTimer::Event PomodoroTimer::startShortBreak()
{
    _state = ShortBreak;
    _duration = _shortTime;
    _startMillis = millis();

    return ShortBreakStarted;
}

PomodoroTimer::Event PomodoroTimer::startLongBreak()
{
    _state = LongBreak;
    _duration = _longTime;
    _startMillis = millis();

    return LongBreakStarted;
}

PomodoroTimer::Event PomodoroTimer::nextState()
{
    switch (_state)
    {
        case Work:

            if (_cycle >= _maxCycles)
                return startLongBreak();

            return startShortBreak();

        case ShortBreak:

            _cycle++;
            return startWork();

        case LongBreak:

            reset();
            return Finished;

        default:

            return None;
    }
}