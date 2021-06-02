//
//  Alarm.cpp
//  
//
//  Created by Abdelkrim Zitouni on 2021-06-01.
//

#include "Alarm.h"



Alarmm::Alarmm(uint8_t DOW, uint8_t AlarmH, uint8_t AlarmM) {
    AlarmHours = AlarmH;
    AlarmMins = AlarmM;
    AlarmDays = DOW;
}

bool Alarmm::CheckAlarm(uint8_t rtcH, uint8_t rtcM, uint8_t rtcD) {
    if (state == Alarmm::RUNNING) {
        if (this->AlarmHours == rtcH && this->AlarmMins == rtcM && this->AlarmDays == rtcD) {
            return true;
        } else return false;
    }
}

void Alarmm::setAlarm(uint8_t DOW, uint8_t Hours, uint8_t Min) {
    AlarmDays = DOW;
    AlarmHours = Hours;
    AlarmMins = Min;
}

//uint8_t Alarmm::getAlarm() {
//    return AlarmDays * 10000UL + AlarmHours * 100UL + AlarmMins;
//}

void Alarmm::setON(bool ONOFF) {
    if (ONOFF == true) state = Alarmm::RUNNING;
    else if (ONOFF == false) state = Alarmm::STOPPED;
}



