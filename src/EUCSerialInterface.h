#ifndef EUC_SERIAL_INTERFACE_H
#define EUC_SERIAL_INTERFACE_H

#include <Arduino.h>

class Euc {
    public:
        // Main data
        float voltage;
        float speed;
        float tempMileage;
        float temperature;
        float mileage;
        
        bool isNew();

        Euc(Stream &ReceiverSerial, Stream &TransmitterSerial);

        void tick();
        void setCallback(void (*eucLoopCallback)(float, float, float, float, float, float, bool));

        void beep();
        void setRideMode(int mode);
        void setAlarms(int level, bool state);
        void calibrateAlignment();
        void set6kmhTiltback(bool state);

        Stream &ReceiverSerial;
        Stream &TransmitterSerial;
        void (*eucLoop)(float,float,float,float,float,float,bool);
    
        int getRideState();
    protected:

};

/*
 * Class for usable data from unicycle
 */

class GotwayMCM2: public Euc {
    public:
        struct RawData {
            unsigned char headerPrimaryPacket[8] = {0x04, 0x18, 0x5A, 0x5A, 0x5A, 0x5A, 0x55, 0xAA};
            unsigned char voltage[2] = {0x00, 0x00};
            unsigned char speed[2] = {0x00, 0x00};
            unsigned char tempMileage[4] = {0x00, 0x00, 0x00, 0x00};
            unsigned char current[2] = {0x00, 0x00};
            unsigned char temperature[2] = {0x00, 0x00};
            //unsigned char headerSecondaryPacket[8] = {0x00, 0x18, 0x5A, 0x5A, 0x5A, 0x5A, 0x55, 0xAA};
            unsigned char unknownData[4] = {0x00, 0x00, 0x00, 0x00}; // TODO: figure out what that is //NOTE: only first byte seems to change
            unsigned char headerSecondaryPacket[8] = {0x00, 0x18, 0x5A, 0x5A, 0x5A, 0x5A, 0x55, 0xAA};
            unsigned char mileage[4] = {0x00, 0x00, 0x00, 0x00};
            unsigned char end[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            
            bool dataIsNew = false;
        };
        struct UsableData {
            float voltage;
            float speed;
            float tempMileage;
            float current;
            float temperature;
            //unsigned char unknownData[4];
            float mileage;
            bool dataIsNew = false;
        };
        GotwayMCM2::RawData receiveRawData();
        GotwayMCM2::UsableData makeRawDataUsable(GotwayMCM2::RawData eucRawData);
};

class GotwayMCM4: public Euc {
    public:
        struct RawData {

        };
        struct UsableData {
            float voltage;
            float speed;
            float tempMileage;
            float temperature;
            float mileage;
            
            float calculateAcceleration();
            float calculatePower();
            bool isNew();
        };
        float calculateAcceleration();
        float calculatePower();
}

#endif // EUC_SERIAL_INTERFACE_H