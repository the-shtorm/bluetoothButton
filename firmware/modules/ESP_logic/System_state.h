#include "Arduino.h"
#ifndef SYSTEM_H
#define SYSTEM_H

#include <Encoder_range.h>

class SystemState {
private:
    Range<int> stateRange;
    boolean bleConnected;
    boolean isClick;
    int batteryLevel;

public:
    SystemState(int min, int max, int start) : stateRange(min, max, start) { }

    ProjectState getState() {
        return (ProjectState)stateRange.get();
    }

    boolean getBluetooth() {
        return bleConnected;
    }

    boolean getClick() {
        return isClick;
    }

    int getBatteryLevel() {
        return batteryLevel;
    }

    void setBatteryLevel(int level) {
        batteryLevel = level;
    }

    void setClick(boolean click) {
        isClick = click;
    }

    void changeState(int step) {
        stateRange.change(step);
    }

    void setBluetooth(boolean bleState) {
        bleConnected = bleState;
    }
};

#endif