/*
 * This accessory.cpp is configurated for light accessory + fan accessory
 */

#include "Accessory.h"

#include "PHKAccessory.h"

//Global Level of light strength
int lightStength;
int fanSpeed;

class lightPowerState: public boolCharacteristics {
public:
    lightPowerState(int index): boolCharacteristics(index, charType_on, premission_read|premission_write){}
    string value() {
        if (lightStength > 0)
            return "1";
        return "0";
    }
    void setValue(string str) {
        this->boolCharacteristics::setValue(str);
        if (_value) {
            lightStength = 255;
            setLightStrength(255);
        } else {
            lightStength = 0;
            setLightStrength(0);
        }
    }
};

class lightBrightness: public intCharacteristics {
public:
    lightBrightness(int index):intCharacteristics(index, charType_brightness, premission_read|premission_write, 0, 100, 1, unit_percentage) {}
    void setValue(string str) {
        this->intCharacteristics::setValue(str);
        lightStength = _value;
        setLightStrength(2.55*_value);
    }
};

class lightService: public Service {
    stringCharacteristics serviceName;
    lightPowerState powerState;
    lightBrightness brightness;
public:
    lightService(int index): Service(index, charType_lightBulb),
    serviceName(index+1, charType_serviceName, premission_read, 0), powerState(index+2), brightness(index+3)
    {
        serviceName.setValue(deviceName);
        powerState.setValue("false");
    }
    inline virtual short numberOfCharacteristics() { return 3; }
    inline virtual characteristics *characteristicsAtIndex(int index) {
        switch (index-1-serviceID) {
            case 0:
                return &serviceName;
            case 1:
                return &powerState;
            case 2:
                return &brightness;
        }
        return 0;
    }
};



//For bridge, create more than one subclass, and insert in main accessory
//Also change the MainAccessorySet
class MainAccessory: public Accessory {
    infoService info;
    lightService light;
public:
    MainAccessory(int aid): Accessory(aid),
    info(1), light(info.serviceID+info.numberOfCharacteristics()+1) {}
    inline virtual short numberOfService() { return 2; }
    inline virtual Service *serviceAtIndex(int index) {
        switch (index-1) {
            case 0:
                return &info;
            case 1:
                return &light;
        }
        return 0;
    }
};

class fanPowerState: public boolCharacteristics {
public:
    fanPowerState(int index): boolCharacteristics(index, charType_on, premission_read|premission_write){}
    string value() {
        if (fanSpeed > 0)
            return "1";
        return "0";
    }
    void setValue(string str) {
        this->boolCharacteristics::setValue(str);
        if (_value) {
            fanSpeed = 255;
            setFanSpeed(255);
        } else {
            fanSpeed = 0;
            setFanSpeed(0);
        }
    }
};

class fanSpeed: public intCharacteristics {
public:
    fanSpeed(int index):intCharacteristics(index, charType_brightness, premission_read|premission_write, 0, 100, 1, unit_percentage) {}
    void setValue(string str) {
        this->intCharacteristics::setValue(str);
        fanSpeed = _value;
        setFanSpeed(2.55*_value);
    }
};

class fanService: public Service {
    stringCharacteristics serviceName;
    fanPowerState powerState;
    fanSpeed speed;
public:
    fanService(int index): Service(index, charType_fan),
    serviceName(index+1, charType_serviceName, premission_read, 0), powerState(index+2), speed(index+3)
    {
        serviceName.setValue("Fan");
        powerState.setValue("false");
    }
    inline virtual short numberOfCharacteristics() { return 3; }
    inline virtual characteristics *characteristicsAtIndex(int index) {
        switch (index-1-serviceID) {
            case 0:
                return &serviceName;
            case 1:
                return &powerState;
            case 2:
                return &speed;
        }
        return 0;
    }
};


class FanAccessory: public Accessory {
    infoService info;
    fanService fan;
public:
    FanAccessory(int aid): Accessory(aid), info(1), fan(info.serviceID+info.numberOfCharacteristics()+1) {}
    inline virtual short numberOfService() { return 2; }
    inline virtual Service *serviceAtIndex(int index) {
        switch (index-1) {
            case 0:
                return &info;
            case 1:
                return &fan;
        }
        return 0;
    }
};

//For bridge, change the subject to dynamic assign
class MainAccessorySet: public AccessorySet {
    MainAccessory acc;
    FanAccessory fan;
public:
    MainAccessorySet(): acc(1), fan(2) {
    }
    short numberOfAccessory() { return 2; }
    Accessory * accessoryAtIndex(int index) {
        if (index == 0) return &acc;
        else return &fan;
    }
};

AccessorySet *accSet;

void initAccessorySet() {
    printf("Initial Accessory\n");
    accSet = new MainAccessorySet();
};