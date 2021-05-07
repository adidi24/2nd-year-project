```cpp
#include "virtuabotixRTC.h"    
//#include <IRremote.h>  // not now !
#include <Pushbutton.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

/*
  last modification : 07-05-2021
  by : Zitouni Abdelkrim
  
  ps : Comment your last modifications to discuss them first
*/


virtuabotixRTC myRTC(6, 7, 8);       // initialising the DS1302 pins
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // set the @ of the I2C LCD



const int setButton_pin = 2;
const int hourButton_pin = 5;
const int stopwatchButton_pin = 4;
const int timerButton_pin = 3;
const int alarmButton_pin = A1;


Pushbutton setButton(setButton_pin);
Pushbutton hourButton(hourButton_pin);
Pushbutton stopwatchButton(stopwatchButton_pin);
Pushbutton timerButton(timerButton_pin);
Pushbutton alarmButton(alarmButton_pin);



unsigned long clockDisplay = millis();

// time variables
int CountdownTime = 1;
int hr = 0;
int min = 0;
int setHr = 0;
int setMin = 0;
// date variables
int dy;
int mon;
int yr;
int setDy;
int setMon;
int setYr;
int setDOW = 1;
// alarm variables
int alarmHr = 0;
int alarmMin = 0;

//stopwatch variables



enum selection {  // for mode selection (still not working)
  Hour,
  Timer_Count,
  Alarm,
  Stopwatch

};

enum selection mode = Hour;


//*******************SETUP*******************\\.
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  Serial.begin(9600);


  //myRTC.setDS1302Time(00, 00, 1, 6, 7, 5, 2020);
  setHr = myRTC.hours;
  setMin = myRTC.minutes;
  setDOW = myRTC.dayofweek;
  setDy = myRTC.dayofmonth;
  setMon = myRTC.month;
  setYr = myRTC.year;
}


//////////////////////////////////////////////
//*******************LOOP*******************\\.
void loop() {
  mode = Hour;
  if (millis() - clockDisplay > 500) {
    myRTC.updateTime();
    clockDisplay = millis();
    printTime();
  }

  switch (mode) {
    case Hour:
      setDateTime();
      //updateTime();
      //lcd.clear();
      printTime();
      break;
    case Timer_Count:
      break;
    case Stopwatch:
      break;
  }
  delay(1000);
}
//*******************LOOP*******************\\.
//////////////////////////////////////////////




//function -DayAsString- that converts days of the week into strings
String DayAsString(uint8_t DOW) {
  switch (DOW) {
    case 1: return "Sunday";
    case 2: return "Monday";
    case 3: return "Tuesday";
    case 4: return "Wednesday";
    case 5: return "Thursday";
    case 6: return "Friday";
    case 7: return "Saturday";
  }
  return "incorrect";
}


//function -printTime- that prints DateTime to both serial monitor and LCD
void printTime() {
  lcd.clear();

  const String DOW = DayAsString(myRTC.dayofweek);  // getting the day of the week

  // Print everything out to the Serial Monitor window
  Serial.print(DOW);
  Serial.print("  ");
  Serial.print(myRTC.year);
  Serial.print("-");
  Serial.print(myRTC.month);
  Serial.print("-");
  Serial.print(myRTC.dayofmonth);
  Serial.print("  ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);

  // Print everything out to the LCD
  lcd.setCursor(4, 0);
  lcd.print(myRTC.hours);
  lcd.setCursor(6, 0);
  lcd.print(":");
  lcd.setCursor(7, 0);
  lcd.print(myRTC.minutes);
  lcd.setCursor(9, 0);
  lcd.print(":");
  lcd.setCursor(10, 0);
  lcd.print(myRTC.seconds);

  lcd.setCursor(0, 1);
  lcd.print(DOW.substring(0, 4));
  lcd.setCursor(6, 1);
  lcd.print(myRTC.dayofmonth);
  lcd.setCursor(8, 1);
  lcd.print("/");
  lcd.setCursor(9, 1);
  lcd.print(myRTC.month);
  lcd.setCursor(11, 1);
  lcd.print("/");
  lcd.setCursor(12, 1);
  lcd.print(myRTC.year);
}

// Function modeSelection : select mode (Date-Time, Timer, StopWhatch) from inputs
void modeSelection() {

  // if ((hourButton.getSingleDebouncedPress())) {       // will be changed after adding remote control
  //   printTime();
  // } else if ((stopwatchButton.getSingleDebouncedPress())) {
  //   stopWhatch();
  // } else if ((timerButton.getSingleDebouncedPress())) {
  //   Timer();
  // }
}


// Function stopWhatch : Displays stopWhatch (not completed !)***********
void stopWhatch() {
  lcd.clear();
}


//Function Timer : displays Timer (not completed !)***********
void Timer() {
  lcd.clear();
}


// Function setDateTime : setting year, month, day, according to the input
void setDateTime() {  
  int cursorX = 4, cursorY = 0;
  while (setButton.isPressed()) {
    if (timerButton.getSingleDebouncedPress()) {  // from here ..
      cursorX++;
      if (cursorX > 7 && cursorY == 0) {
        cursorY = 1;
        cursorX = 0;
      };
      if (cursorX > 12 && cursorY == 1) {
        cursorY = 0;
        cursorX = 4;
      };
      lcd.cursor_on();
      lcd.setCursor(cursorX, cursorY);  // .. to here : we're setting the cursor according to user input
    }
    // Here begins the Time setting
    if (cursorY == 0) {
      switch (cursorX) {
        case 4:
          setHr = myRTC.hours;
          IncDec(setHr);
          if (setHr >= 24) setHr = 0;
          lcd.print(setHr);

          break;
        case 7:
          setMin = myRTC.minutes;
          IncDec(setMin);
          if (setMin >= 60) setMin = 0;
          lcd.print(setMin);
          break;
      }
    } else {  // Here begins the Date setting
      switch (cursorX) {
        case 0:
          setDOW = myRTC.dayofweek;
          IncDec(setDOW);
          if (setDOW > 7) setDOW = 1;
          lcd.print(DayAsString(setDOW).substring(0, 4));
          break;
        case 6:
          setDy = myRTC.dayofmonth;
          IncDec(setDy);
          if (setDy > lastDayOfTheMonth(setMon)) setDy = 1;
          lcd.print(setDy);
          break;
        case 9:
          setMon = myRTC.month;
          IncDec(setMon);
          if (setMon > 12) setMon = 1;
          lcd.print(setMon);
          break;
        case 12:
          setMon = myRTC.year;
          IncDec(setYr);
          if (setYr > 2099) setYr = 2000;
          lcd.print(setYr);
          break;
      }
    }
  }
  myRTC.hours = setHr;
  myRTC.minutes = setMin;
  myRTC.dayofweek = setDOW; 
  myRTC.dayofmonth = setDy;
  myRTC.month = setMon;
  myRTC.year = setYr;
  delay(15);
}
///


// Function IncDec : increments/decrements any integer by pressing the left/Middle button respectively (used to set the clock)
void IncDec(int any) {
  if (hourButton.getSingleDebouncedPress()) any++;
  else if (timerButton.getSingleDebouncedPress())
    any--;
}

// Function lastDayOfTheMonth days per month (Leap Year included)
int lastDayOfTheMonth(int setMonth) {
  switch (setMonth) {
    case 1:  //January
      return 31;
      break;
    case 2:  //February
      if ((setYr % 4) == 0) return 29;
      else
        return 28;
      break;
    case 3:  //March
      return 31;
      break;
    case 4:  //April
      return 30;
      break;
    case 5:  //May
      return 31;
      break;
    case 6:  //June
      return 30;
      break;
    case 7:  //JulY
      return 31;
      break;
    case 8:  //August
      return 31;
      break;
    case 9:  //September
      return 30;
      break;
    case 10:  //October
      return 31;
      break;
    case 11:  //November
      return 30;
      break;
    case 12:  //December
      return 31;
      break;
  }
}

```
