

#include <DS1302.h>
//#include <IRremote.h>
#include <Pushbutton.h> 
//#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
/*
  last modification : 03-05-2021
  by : Zitouni Abdelkrim
  
  ps : Comment your last modifications to discuss them first
*/


DS1302 rtc(8, 7, 6);                  //initialising the DS1302 pins                  
LiquidCrystal_I2C lcd(0x3F, 16, 2);   //geting the @ of the LCD


//const int buttonPin = 2; //set the button to pin 2  




enum selection {.    // for mode selection (still not )
  OnOff,
  display,
  set_DateTime,
  add_Timer,
  start_Stopwatch
  
};

enum selection mode = OnOff;


//----------------SETUP----------------\\.
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); 
  Serial.begin(9600);  

  rtc.writeProtect(false);
  rtc.halt(false);

  Time t(2020, 05, 03, 02, 46, 00, Time::kMonday);
  rtc.time(t);
}


/////////////////////////////////////////
//----------------LOOP----------------\\.
void loop() {
  
  printTime();
  delay(1000);
  
}
//----------------LOOP----------------\\.
/////////////////////////////////////////




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

























