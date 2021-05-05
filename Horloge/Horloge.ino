```cpp


#include <DS1302.h>
//#include <IRremote.h>  // not now !
#include <Pushbutton.h> 
//#include <EEPROM.h> 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

/*
  last modification : 05-05-2021
  by : Zitouni Abdelkrim
  
  ps : Comment your last modifications to discuss them first
*/


DS1302 rtc(8, 7, 6);                  // initialising the DS1302 pins                  
LiquidCrystal_I2C lcd(0x3F, 16, 2);   // set the @ of the I2C LCD



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

// time variables
int CountdownTime = 1;
int hr = 0;
int min = 0;
int setHr = 0;
int setMin = 0;
// date variables
int dy = 1;
int mon = 1;
int yr = 2000;
int setDy = 1;
int setMon = 1;
int setYr = 2000;
// alarm variables
int alarmHr = 0;
int alarmMin = 0;
Time::Day DOW = Time::kSunday;
//stopwatch variables



// enum selection {    // for mode selection (still not working)
//   OnOff,
//   Hour,
//   set_DateTime,
//   add_Timer,
//   start_Stopwatch
  
// };

//enum selection mode = OnOff;


//*******************SETUP*******************\\.
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); 
  Serial.begin(9600);  

  rtc.writeProtect(false);
  rtc.halt(false);

  Time t(2020, 05, 05, 01, 27, 00, Time::kTuesday);
  rtc.time(t);
}


//////////////////////////////////////////////
//*******************LOOP*******************\\.
void loop() {
  printTime();
  //modeSelection();
  delay(1000);
  
}
//*******************LOOP*******************\\.
//////////////////////////////////////////////




//function -DayAsString- that converts days of the week into strings
String DayAsString(const Time::Day day) { 
  switch (day) {
    case Time::kSunday: return "Sunday";
    case Time::kMonday: return "Monday";
    case Time::kTuesday: return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday: return "Thursday";
    case Time::kFriday: return "Friday";
    case Time::kSaturday: return "Saturday";
  }
  return "inc";
}


//function -printTime- that prints DateTime to both serial monitor and LCD
void printTime() {
  //lcd.clear();
  Time t = rtc.time();    // Get the current time and date from the DS1302
 
  const String day = DayAsString(t.day);   // getting the day of the week

  // Print everything out to the Serial Monitor window
  Serial.print(day); Serial.print("  ");
  Serial.print(t.yr); Serial.print("-");
  Serial.print(t.mon); Serial.print("-");
  Serial.print(t.date); Serial.print("  ");
  Serial.print(t.hr); Serial.print(":");
  Serial.print(t.min); Serial.print(":");
  Serial.println(t.sec);

  // Print everything out to the LCD
  lcd.setCursor(4, 0);
  lcd.print(t.hr);
  lcd.setCursor(6, 0);
  lcd.print(":");
  lcd.setCursor(7, 0);
  lcd.print(t.min);
  lcd.setCursor(9, 0);
  lcd.print(":");
  lcd.setCursor(10, 0);
  lcd.print(t.sec);
  
  lcd.setCursor(0, 1);
  lcd.print(day.substring(0, 4));
  lcd.setCursor(6, 1);
  lcd.print(t.date);
  lcd.setCursor(8, 1);
  lcd.print("/");
  lcd.setCursor(9, 1);
  lcd.print(t.mon);
  lcd.setCursor(11, 1);
  lcd.print("/");
  lcd.setCursor(12, 1);
  lcd.print(t.yr);
}

// Function modeSelection : select mode (Date-Time, Timer, StopWhatch) from inputs 
void modeSelection() {
  
  if ((hourButton.getSingleDebouncedPress())) {       // will be change after adding remote control
    printTime();
  } else if ((stopwatchButton.getSingleDebouncedPress())) {
    stopWhatch();
  } else if ((timerButton.getSingleDebouncedPress())) {
    Timer();
  }
}


// Function stopWhatch : Displays stopWhatch (not completed !)***********
void stopWhatch() {
  lcd.clear();
}


//Function Timer : displays Timer (not completed !)***********
void Timer() { 
  lcd.clear();
}


void setDateTime() { // not completed ! ***********

  Time t = rtc.time();
  
  int cursorX = 4, cursorY = 0;

  if (mode == Hour) {
    while (setButton.isPressed()) {
      if (timerButton.getSingleDebouncedPress()) {
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
        lcd.setCursor(cursorX, cursorY);
      }
      if (cursorY == 0) {   
        switch (cursorX) {
          case 4:
            setHr = t.hr;
            IncDec(setHr);
            lcd.print(setHr);
                    
            break;
          case 7:
            setMin = t.min;
            IncDec(setMin);
            lcd.print(setMin);
            break;   
        } 
      } else {
        switch (cursorX) {
          case 0:
            break;
          case 6:
            setDy = t.date;
            IncDec(setDy);
            if (setDy <= lastDayOfTheMonth(setMon)) lcd.print(setDy); else {
              setDy = 1; 
              lcd.print(setDy);
            }
                    
            break;
          case 9:
            break;
          case 12:
            break;             
          } 
      }
    } 
  }      
}


// Function IncDec : increments/decrements any integer by pressing the left/right button respectively 
void IncDec(int any) {  
  if (hourButton.getSingleDebouncedPress()) any++; else if (timerButton.getSingleDebouncedPress()) any--;
}

// Function lastDayOfTheMonth days per month (Leap Year included)
int lastDayOfTheMonth(int setMonth) {
  switch (setMonth) {
    case 1: //January
      return 31;
      break;
    case 2: //February
      if ((setYr % 4) == 0) return 29; else return 28;
      break;
    case 3: //March
      return 31;
      break;
    case 4: //April
      return 30;
      break;
    case 5: //May
      return 31;
      break;
    case 6: //June
      return 30;
      break;
    case 7: //JulY
      return 31;
      break;
    case 8: //August
      return 31;
      break;
    case 9: //September
      return 30;
      break;
    case 10: //October
      return 31;
      break;
    case 11: //November
      return 30;
      break;
    case 12: //December
      return 31;
      break;
  }
}


















```
