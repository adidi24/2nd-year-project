//
//  Alarm.hpp
//  
//
//  Created by Abdelkrim Zitouni on 2021-06-01.
//

#ifndef Alarm_h
#define Alarm_h
#include "Arduino.h"
class Alarmm {
public:
    uint8_t AlarmHours, AlarmMins, AlarmDays;
    
    
    explicit Alarmm( uint8_t DOW = 2, uint8_t AlarmH = 0, uint8_t AlarmM = 0);

    bool CheckAlarm(uint8_t rtcH, uint8_t rtcM, uint8_t rtcD);
    
    void setAlarm(uint8_t DOW, uint8_t Hours, uint8_t Min);
    
//    uint8_t getAlarm();
    
    void setON(bool ONOFF);
    
    enum State { RUNNING, STOPPED };
    enum State state;
};





#endif /* Alarm_hpp */
