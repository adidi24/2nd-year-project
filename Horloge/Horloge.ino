```cpp

#include "CountDown.h"
#include "Alarm.h"
#include "virtuabotixRTC.h"  
#include <IRremote.h> 
#include <Pushbutton.h>
#include <LiquidCrystal_I2C.h>
#include <StopWatch.h>
#include <EEPROM.h>

// #include <Wire.h>

/*
  last modification : 31-05-2021
  by : Zitouni Abdelkrim
  
  ps : Comment your last modifications to discuss them first
*/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


// change this to make the song slower or faster
int tempo = 140;

// change this to whichever pin you want to use
int buzzer = 11;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Take on me, by A-ha
  // Score available at https://musescore.com/user/190926/scores/181370
  // Arranged by Edward Truong

  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;





virtuabotixRTC myRTC(6, 7, 8);       // initialising the DS1302 pins
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // set the @ of the I2C LCD
StopWatch sw_secs(StopWatch::MILLIS);
CountDown CD[3];

Alarmm AL[3];
uint8_t Ah[3], Am[3], Ad[3];
const int ARRAY_SIZE = 3;
const int STARTING_EEPROM_ADDRESS_1 = 10;
const int STARTING_EEPROM_ADDRESS_2 = 20;
const int STARTING_EEPROM_ADDRESS_3 = 30;

#define led_pin 13

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

int RECV_PIN = 9;

IRrecv irrecv(RECV_PIN);
unsigned long key_value = 0;

unsigned long clockDisplay = millis();
unsigned long AlarmDisplay = millis();


int any = 0;

int cursorX = 0, cursorY = 0;  // To set lcd cursor

bool setByRemote;

// time variables
uint8_t tDays, tHours, tMins, tSecs;
bool plus;
int accumulVal = 0;
const int MaxChars = 2;
char strValue[MaxChars + 1];   // String for 2 digits + null char
int index = 0;
int CdNumber = 0;
bool pause = false;
int CountdownMin = 1;
int setHr = 0;
int setMin = 0;
int setSec = 0;
// date variables
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

  for (int i = 0; i <= 2; i++) {
    AL[i].setON(false);
  }

  readAlarmSetFromEEPROM(STARTING_EEPROM_ADDRESS_1, Ad, ARRAY_SIZE);
  readAlarmSetFromEEPROM(STARTING_EEPROM_ADDRESS_2, Ah, ARRAY_SIZE);
  readAlarmSetFromEEPROM(STARTING_EEPROM_ADDRESS_3, Am, ARRAY_SIZE);  
  
  pinMode(buzzer, OUTPUT);
  irrecv.enableIRIn();
  // myRTC.setDS1302Time(00, 14, 21, 3, 25, 5, 2020);
}


//////////////////////////////////////////////
//*******************LOOP*******************\\.
void loop() {
  if (millis() - clockDisplay > 500) {
    clockDisplay = millis();
    myRTC.updateTime();
    buttonSlectMode();
    lcd.blink();   
    if (irrecv.decode()) {
      
      if (irrecv.decodedIRData.decodedRawData == 0xBA45FF00) {
        decMode(); 
      }
      if (irrecv.decodedIRData.decodedRawData == 0xB847FF00) {
        incMode(); 
      }
    }
       
    irrecv.resume();   

    //digitalWrite(led_pin, HIGH);
  }  
  int thisNote = 0;
  if (millis() - AlarmDisplay > 800) {
    AlarmDisplay = millis();
    for (int i = 0; i <= 2; i++) {
      if (AL[i].state == Alarmm::RUNNING) {
        if (AL[i].CheckAlarm(myRTC.hours, myRTC.minutes, myRTC.dayofweek)) {
          lcd.clear();
          while (!alarmButton.getSingleDebouncedPress() && thisNote < notes * 2) {
            AL[i].setON(false);
            divider = melody[thisNote + 1];
            if (divider > 0) {
              // regular note, just proceed
              noteDuration = (wholenote) / divider;
            } else if (divider < 0) {
              // dotted notes are represented with negative durations!!
              noteDuration = (wholenote) / abs(divider);
              noteDuration *= 1.5; // increases the duration in half for dotted notes
            }

            // we only play the note for 90% of the duration, leaving 10% as a pause
            tone(buzzer, melody[thisNote], noteDuration * 0.9);

            // Wait for the specief duration before playing the next note.
            delay(noteDuration);

            // stop the waveform generation before the next note.
            noTone(buzzer);
            lcd.setCursor(0, 0);
            lcd.print("ALARM " + i);
            lcd.print(" is ON!");
            thisNote = thisNote + 2;
          }
              lcd.clear();
        }        
      }
          
    }
  }
  
  switch (mode) {
    case Hour:
      irrecv.resume();    
      

      setDateTime();   
        // irrecv.resume(); 
      printTime();
      // setDateTime2();
      break;
    case Timer_Count:
      Timer();
      irrecv.resume(); 
      break;
    case Alarm:
      alarm();      
      break;    
    case Stopwatch:
      stopWhatch();
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
  lcd.clear();
  lcd.setCursor(4, 0);
  paddingZero(myRTC.hours);
  lcd.setCursor(6, 0);
  lcd.print(":");
  lcd.setCursor(7, 0);
  paddingZero(myRTC.minutes);
  lcd.setCursor(9, 0);
  lcd.print(":");
  lcd.setCursor(10, 0);
  paddingZero(myRTC.seconds);
  lcd.setCursor(0, 1);
  lcd.print(DOW.substring(0, 3));
  lcd.setCursor(6, 1);
  paddingZero(myRTC.dayofmonth);
  lcd.setCursor(8, 1);
  lcd.print("-");
  lcd.setCursor(9, 1);
  paddingZero(myRTC.month);
  lcd.setCursor(11, 1);
  lcd.print("-");
  lcd.setCursor(12, 1);
  lcd.print(myRTC.year);
}

void buttonSlectMode() {
  if (hourButton.getSingleDebouncedPress()) incMode();
  if (timerButton.getSingleDebouncedPress()) decMode();   
}

// Function incMode : select mode (Date-Time, Timer, StopWhatch)
void incMode() {
  switch (mode) {
  case Hour:
    mode = Timer_Count;
    break;
  case Timer_Count:
    mode = Alarm;
    break;
  case Alarm:
    mode = Stopwatch;
    break;
  case Stopwatch:
    mode = Hour;
    break;        
  }
}
// Function decMode : select mode (Date-Time, Timer, StopWhatch)
void decMode() {
  switch (mode) {
  case Hour:
    mode = Stopwatch;
    break;
  case Stopwatch:
    mode = Alarm;
    break;
  case Alarm:
    mode = Timer_Count;
    break;
  case Timer_Count:
    mode = Hour;
    break;        
  }  
}

// Function stopWhatch : Displays stopWhatch (not completed !)***********
void stopWhatch() {
  lcd.clear();
  lcd.setCursor(0, 0);
  bool sw_State = false;
  lcd.print("StopWhatch  Mode");
  lcd.setCursor(0, 1);
  lcd.print("<<Press  Start>>");
  String buff = String(sw_secs.elapsed()/1000); 
  String buff2 = String(sw_secs.elapsed() % 1000); 
  while (true) {  

    lcd.setCursor(0, 1);
    if (hourButton.getSingleDebouncedPress() && !sw_secs.isRunning()) {
      lcd.clear();
      sw_secs.start();
    }
    
    while (sw_secs.isRunning()) {
      buff = String(sw_secs.elapsed()/1000); 
      buff2 = String(sw_secs.elapsed() % 1000); 
      
      lcd.setCursor(0, 0);     
      lcd.print(buff + '.' + buff2);
      
      if (hourButton.getSingleDebouncedPress() && sw_secs.isRunning()) {
        sw_secs.stop();
              
        buff = String(sw_secs.elapsed()/1000); 
        buff2 = String(sw_secs.elapsed() % 1000);
        lcd.setCursor(0, 0); 
        lcd.print(buff + '.' + buff2);
      }

      if (stopwatchButton.getSingleDebouncedPress() && sw_secs.isRunning()) {
        lcd.setCursor(0, 1);
        
        buff = String(sw_secs.elapsed()/1000); 
        buff2 = String(sw_secs.elapsed() % 1000);
        lcd.print(buff + '.' + buff2);
      }

    }
    
    if (stopwatchButton.getSingleDebouncedPress() && !sw_secs.isRunning()) {
      lcd.clear();
      lcd.print("<<Press  Start>>");
      sw_secs.reset();
      buff = String(sw_secs.elapsed()/1000); 
      buff2 = String(sw_secs.elapsed() % 1000);
      lcd.setCursor(0, 1); 
      lcd.print(buff + '.' + buff2);
      };    

    if (timerButton.getSingleDebouncedPress()) break;  

///////////////////REMOTE//////////////////
    if (irrecv.decode()) {
      if (irrecv.decodedIRData.decodedRawData == 0xBC43FF00 && !sw_secs.isRunning()) {
        lcd.clear();
        sw_secs.start();
        };
      if (irrecv.decodedIRData.decodedRawData == 0xF609FF00 && !sw_secs.isRunning()) {
        lcd.clear();
        lcd.print("<<Press  Start>>");
        sw_secs.reset();
        buff = String(sw_secs.elapsed()/1000); 
        buff2 = String(sw_secs.elapsed() % 1000);
        lcd.setCursor(0, 1); 
        lcd.print(buff + '.' + buff2);        
      };    
      if (irrecv.decodedIRData.decodedRawData == 0xB847FF00) break;
    }    
    
    irrecv.resume();
    
    while (sw_secs.isRunning()) {
      buff = String(sw_secs.elapsed()/1000); 
      buff2 = String(sw_secs.elapsed() % 1000); 
      
      lcd.setCursor(0, 0);     
      lcd.print(buff + '.' + buff2);
      if (irrecv.decode()) {
        if (irrecv.decodedIRData.decodedRawData == 0xBC43FF00 && sw_secs.isRunning()) {
          sw_secs.stop();
              
          buff = String(sw_secs.elapsed()/1000); 
          buff2 = String(sw_secs.elapsed() % 1000);
          lcd.setCursor(0, 0); 
          lcd.print(buff + '.' + buff2);
          
          
        }
        
        if (irrecv.decodedIRData.decodedRawData == 0xF609FF00 && sw_secs.isRunning()) {
          lcd.setCursor(0, 1);
        
          buff = String(sw_secs.elapsed()/1000); 
          buff2 = String(sw_secs.elapsed() % 1000);
          lcd.print(buff + '.' + buff2);
          
        } 
        irrecv.resume();       
      }    

    }
  }          
}


//Function Timer : displays Timer (not completed !)***********
void Timer() {
  uint32_t start = millis(), remain = 1;
  int d[3], h[3], m[3], s[3];
  lcd.clear();
  bool enter;
  int i = 0, thisT = 0;
  
  lcd.setCursor(2, 0);
  lcd.print("TIMER 1");
  for (int j = 0; j <= 2; j++) {
    d[j] = 0;
    h[j] = 0;
    m[j] = 0;
    s[j] = 0;    
  }
  cursorX = 0;
  if (alarmButton.getSingleDebouncedPress()) enter = true;
  while (CD[i].remaining() >= 0 && i <= 2 && enter == true) {
    if (alarmButton.getSingleDebouncedPress()) enter = false;
    if (timerButton.getSingleDebouncedPress()) {
      i++;                
      i = i % 3;
      lcd.setCursor(2, 0);
      lcd.print("TIMER ");
      lcd.print(i + 1);
    }
    if (hourButton.getSingleDebouncedPress() && (CD[i].remaining() == 0 || CD[i].isRunning() == false)) {
      CD[i].start(d[i], h[i], m[i], s[i]);
    }

    if (CD[i].isRunning() == true) {
      if (stopwatchButton.getSingleDebouncedPress()) {  
        CD[i].stop();
      } 
      lcd.setCursor(11, 0);
      lcd.print("[ON ]");     
    } else if (CD[i].isRunning() == false){
      if (stopwatchButton.getSingleDebouncedPress()) {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("TIMER ");
        lcd.print(i + 1);
        CD[i].cont();
      }
      lcd.setCursor(11, 0);
      lcd.print("[OFF]"); 
         
    } 
            
        
       

    remain = CD[i].remaining();
    tDays = remain / 86400UL;            // Time conversion
    tHours = (remain % 86400UL) / 3600UL;
    tMins = ((remain % 86400UL) % 3600UL) / 60;
    tSecs = remain % 60;
        
    
    lcd.setCursor(0, 1);
    paddingZero(tDays);
    lcd.setCursor(2, 1);
    lcd.print("d");
    lcd.setCursor(4, 1);
    paddingZero(tHours);
    lcd.setCursor(6, 1);
    lcd.print("h");
    lcd.setCursor(8, 1);
    paddingZero(tMins);
    lcd.setCursor(10, 1);
    lcd.print("m");
    lcd.setCursor(12, 1);
    paddingZero(tSecs);
    lcd.setCursor(14, 1);
    lcd.print("s");    
      
      
    while (setButton.isPressed()) {
      
      if (timerButton.getSingleDebouncedPress()) {  // from here ..
        cursorX++;
        cursorX = cursorX % 5;
        lcd.blink();
          // .. to here : we're setting the cursor according to user input
      }    
      switch (cursorX) {
            case 1:
              
              while (true) {
                lcd.setCursor(0, 1);       
                if ((hourButton.getSingleDebouncedPress())){
                  d[i]++;
                  if (d[i] > 48 ) d[i] = 0;
                  paddingZero(d[i]);           
                } 
                if ((stopwatchButton.getSingleDebouncedPress())) {
                  d[i]--;
                  if (d[i] < 0) d[i] = 48;
                  paddingZero(d[i]);
                }
                if (setButton.isPressed()) break;
              };   
              break;

            case 2:
              
              while (true) {
                
                lcd.setCursor(4, 1);
                if ((hourButton.getSingleDebouncedPress())){
                  h[i]++;
                  if (h[i] > 23 ) h[i] = 0;   
                  paddingZero(h[i]);                    
                } 
                if ((stopwatchButton.getSingleDebouncedPress())) {
                  h[i]--;
                  if (h[i] < 0 ) h[i] = 23;       
                  paddingZero(h[i]);
                }
                if (setButton.isPressed()) break;
              };
              break;
            case 3:
              
              while (true) {
                lcd.setCursor(8, 1);
                if ((hourButton.getSingleDebouncedPress())){
                  m[i]++;
                  if (m[i] > 59 ) m[i] = 0;    
                  paddingZero(m[i]);        
                } 
                if ((stopwatchButton.getSingleDebouncedPress())) {
                  m[i]--;
                  if (m[i] < 0) m[i] = 59;    
                  paddingZero(m[i]);
                }
                if (setButton.isPressed()) break;
              };
              break;
              
            case 4:
              
              while (true) {
                lcd.setCursor(12, 1);
                if ((hourButton.getSingleDebouncedPress())){
                  s[i]++;
                  if (s[i] > 59 ) s[i] = 0;    
                  paddingZero(s[i]);     
                } 
                if ((stopwatchButton.getSingleDebouncedPress())) {
                  s[i]--;
                  if (s[i] < 0) s[i] = 59;     
                  paddingZero(s[i]);
                }
                if (setButton.isPressed()) break;
              };
              break;
          };
      if (setButton.getSingleDebouncedRelease()) break;
      thisT = i;
    };
      
      
  }
  
     
};

void alarm() {
  
  lcd.clear();
  bool enterA = true;
  int i = 0;
   
  lcd.setCursor(2, 0);
  lcd.print("ALARM 1");
  cursorX = 0;
  while (i <= 2 && enterA == true) {
    if (alarmButton.getSingleDebouncedPress()) enterA = false;
    if (timerButton.getSingleDebouncedPress()) {
      i++;                
      i = i % 3;
      lcd.setCursor(2, 0);
      lcd.print("ALARM ");
      lcd.print(i + 1);
    }

    if (AL[i].state == Alarmm::STOPPED) {
      if (stopwatchButton.getSingleDebouncedPress()) {  
        AL[i].setON(true);
        lcd.setCursor(11, 0);
        lcd.print("[ON ]"); 
      } 
    } else {
      if (stopwatchButton.getSingleDebouncedPress()) {
      
        lcd.setCursor(11, 0);
        lcd.print("[OFF]");
        AL[i].setON(false);
      }        
    }
      
        
    

  
    readAlarmSetFromEEPROM(STARTING_EEPROM_ADDRESS_1, Ad, ARRAY_SIZE);
    readAlarmSetFromEEPROM(STARTING_EEPROM_ADDRESS_2, Ah, ARRAY_SIZE);
    readAlarmSetFromEEPROM(STARTING_EEPROM_ADDRESS_3, Am, ARRAY_SIZE);  
    lcd.setCursor(0, 1);
    lcd.print(DayAsString(Ad[i]).substring(0, 3));   
    lcd.setCursor(8, 1);
    paddingZero(Ah[i]);
    lcd.setCursor(10, 1);
    lcd.print("h");
    lcd.setCursor(12, 1);
    paddingZero(Am[i]);
    lcd.setCursor(14, 1);
    lcd.print("m");   
      
      
    while (setButton.isPressed()) {
      
      if (timerButton.getSingleDebouncedPress()) {  // from here ..
        cursorX++;
        cursorX = cursorX % 5;
        lcd.blink();
          // .. to here : we're setting the cursor according to user input
      }    
      switch (cursorX) {
            case 1:
              
              while (true) {
                lcd.setCursor(0, 1);       
                if ((hourButton.getSingleDebouncedPress())){
                  Ad[i]++;
                  if (Ad[i] > 7) Ad[i] = 1;  
                  lcd.print(DayAsString(Ad[i]).substring(0, 3));         
                } 
                if ((stopwatchButton.getSingleDebouncedPress())) {
                  Ad[i]--;
                  if (Ad[i] < 1) Ad[i] = 7;
                  lcd.print(DayAsString(Ad[i]).substring(0, 3));
                }
                if (setButton.isPressed()) break;
              };   
              break;

            case 2:
              
              while (true) {
                
                lcd.setCursor(8, 1);
                if ((hourButton.getSingleDebouncedPress())){
                  Ah[i]++;
                  if (Ah[i] > 23 ) Ah[i] = 0;   
                  paddingZero(Ah[i]);                    
                } 
                if ((stopwatchButton.getSingleDebouncedPress())) {
                  Ah[i]--;
                  if (Ah[i] < 0 ) Ah[i] = 23;       
                  paddingZero(Ah[i]);
                }
                if (setButton.isPressed()) break;
              };
              break;
            case 3:
              
              while (true) {
                lcd.setCursor(12, 1);
                if ((hourButton.getSingleDebouncedPress())){
                  Am[i]++;
                  if (Am[i] > 59 ) Am[i] = 0;    
                  paddingZero(Am[i]);        
                } 
                if ((stopwatchButton.getSingleDebouncedPress())) {
                  Am[i]--;
                  if (Am[i] < 0) Am[i] = 59;    
                  paddingZero(Am[i]);
                }
                if (setButton.isPressed()) break;
              };
              break;
              
          };

      AL[i].setAlarm(Ad[i], Ah[i], Am[i]);
      // Ad[i] = AL[i].AlarmDays;
      // Ah[i] = AL[i].AlarmHours;
      // Am[i] = AL[i].AlarmMins;
      writeAlarmSetIntoEEPROM(STARTING_EEPROM_ADDRESS_1, Ad, ARRAY_SIZE);
      writeAlarmSetIntoEEPROM(STARTING_EEPROM_ADDRESS_2, Ah, ARRAY_SIZE);
      writeAlarmSetIntoEEPROM(STARTING_EEPROM_ADDRESS_3, Am, ARRAY_SIZE); 
      if (setButton.getSingleDebouncedRelease()) break;
    };
     
      
  }
  
  
};

void recvDigits(int nn) {  
  while (index < 2) {
    irrecv.resume();
    if (irrecv.decode()) {
      switch(irrecv.decodedIRData.decodedRawData){
        case 0xF30CFF00:
        strValue[index++] = '1';
        break ;
        case 0xE718FF00:
        strValue[index++] = '2';
        break ;
        case 0xA15EFF00:
        strValue[index++] = '3';
        break ;
        case 0xF708FF00:
        strValue[index++] = '4';
        break ;
        case 0xE31CFF00:
        strValue[index++] = '5';
        break ;
        case 0xA55AFF00:
        strValue[index++] = '6';
        break ;
        case 0xBD42FF00:
        strValue[index++] = '7';
        break ;
        case 0xAD52FF00:
        strValue[index++] = '8';
        break ;
        case 0xB54AFF00:
        strValue[index++] = '9';
        break ;
        case 0xE916FF00:
        strValue[index++] = '0';
        break ;       
    };      
    };
    index++;
  };
  // here when the buffer is full or on the first non digit
  strValue[index] = 0;          // terminate the string with a 0
  accumulVal = atoi(strValue);  // use atoi to convert string to an int
  index = 0;                    // reset index to receive other data
  nn = accumulVal;  
}



// Function setDateTime/2 : setting year, month, day, by button or Remote respectively
void setDateTime() { 
  bool check;
  setHr = myRTC.hours;
  setMin = myRTC.minutes;
  setDOW = myRTC.dayofweek;
  setDy = myRTC.dayofmonth;
  setMon = myRTC.month;
  setYr = myRTC.year;  
  while (setButton.isPressed()) {

    if (timerButton.getSingleDebouncedPress()) {  // from here ..
      cursorX++;
      if (cursorX > 6) cursorX = 1;
      lcd.blink();
      // lcd.setCursor(cursorX, cursorY);
        // .. to here : we're setting the cursor according to user input
    };
    switch (cursorX) {
        case 1:
          // decodeIR(setHr);
          while (true) {
            lcd.setCursor(4, 0);       
            if ((hourButton.getSingleDebouncedPress())){
              setHr++; 
              if (setHr >= 24UL) setHr = 0UL; 
              paddingZero(setHr);           
            } 
            if ((stopwatchButton.getSingleDebouncedPress())) {
              setHr--;
              if (setHr < 0UL) setHr = 23UL;
              paddingZero(setHr);
            }
            if (setButton.isPressed()) break;
          };   
          break;

        case 2:
          // decodeIR(setMin);
          while (true) {
            
            lcd.setCursor(7, 0);
            if ((hourButton.getSingleDebouncedPress())){
              setMin++;
              if (setMin >= 60UL) setMin = 0UL;     
              paddingZero(setMin);                    
            } 
            if ((stopwatchButton.getSingleDebouncedPress())) {
              setMin--;
              if (setMin < 0UL) setMin = 59UL;
              paddingZero(setMin);
            }
            if (setButton.isPressed()) break;
          };
          break;
      
      // Here begins the Date setting
      
        case 3:
          // decodeIR(setDOW);
          while (true) {
            
            lcd.setCursor(0, 1);
            if ((hourButton.getSingleDebouncedPress())){
              setDOW++;
              if (setDOW > 7) setDOW = 1;  
              lcd.print(DayAsString(setDOW).substring(0, 3));         
            } 
            if ((stopwatchButton.getSingleDebouncedPress())) {
              setDOW--;
              if (setDOW < 1) setDOW = 7;
              lcd.print(DayAsString(setDOW).substring(0, 3));
            }
            if (setButton.isPressed()) break;
          };
          break;
          
        case 4:
          // decodeIR(setDy);
          while (true) {
            irrecv.resume();
            lcd.setCursor(6, 1);
            if ((hourButton.getSingleDebouncedPress())){
              setDy++;   
              if (setDy > lastDayOfTheMonth(setMon)) setDy = 1;  
              paddingZero(setDy);     
            } 
            if ((stopwatchButton.getSingleDebouncedPress())) {
              setDy--;
              if (setDy < 1) setDy = lastDayOfTheMonth(setMon);
              paddingZero(setDy);
            }
            if (setButton.isPressed()) break;
          };
          break;
          
        case 5:
          //decodeIR(setMon);
          while (true) {
            irrecv.resume();
            lcd.setCursor(9, 1);
            if ((hourButton.getSingleDebouncedPress())){
              setMon++;
              if (setMon > 12) setMon = 1;    
              paddingZero(setMon);     
            } 
            if ((stopwatchButton.getSingleDebouncedPress())) {
              setMon--;
              if (setMon < 1) setMon = 12;
              paddingZero(setMon);
            }
            if (setButton.isPressed()) break;
          };  
          break;

        case 6:
          //decodeIR(setYr);
          while (true) {
            
            lcd.setCursor(12, 1);
            if ((hourButton.getSingleDebouncedPress())){
              setYr++;
              if (setYr > 2099) setYr = 2000;   
              lcd.print(setYr);     
            } 
            if ((stopwatchButton.getSingleDebouncedPress())) {
              setYr--;
              if (setYr < 2000) setYr = 2099;
              lcd.print(setYr);
            }
            if (setButton.isPressed()) break;
          };          
          break;
      };

    
    
    
    
    // Here begins the Time setting
    myRTC.setDS1302Time(00, setMin, setHr, setDOW, setDy, setMon, setYr);
  
  };

  lcd.setCursor(0, 0);
  delay(20);  
///
}


void setDateTime2() {
  bool check;
  setHr = myRTC.hours;
  setMin = myRTC.minutes;
  setDOW = myRTC.dayofweek;
  setDy = myRTC.dayofmonth;
  setMon = myRTC.month;
  setYr = myRTC.year;
  
  irrecv.resume();
  do {  // irRemote
    irrecv.resume();
    if (irrecv.decode()) {
      if (irrecv.decodedIRData.decodedRawData == 0xF609FF00) {
        setByRemote = false;
      };  

    if (irrecv.decodedIRData.decodedRawData == 0xBC43FF00) {  // from here ..  
      cursorX++;
      if (cursorX > 6) cursorX = 1;
      lcd.blink();
      irrecv.resume();
      // .. to here : we're setting the cursor according to user input
      }
    } else  setByRemote = true;
    irrecv.resume();
    
    // Here begins the Time setting
    switch (cursorX) {
      case 1:
        // decodeIR(setHr);
        do {
          lcd.setCursor(4, 0);
          if (irrecv.decode()) {
            if ((irrecv.decodedIRData.decodedRawData == 0xEA15FF00)) {
              setHr++;
              if (setHr >= 24) setHr = 0;
              paddingZero(setHr);
            }
            if ((irrecv.decodedIRData.decodedRawData == 0xF807FF00)) {
              setHr--;
              if (setHr < 0) setHr = 23;
              paddingZero(setHr);
            }
            irrecv.resume();
            if (irrecv.decodedIRData.decodedRawData == 0xBC43FF00) check = true;
            else check = false;
          }                
        } while (check == false);
        break;

      case 2:
        // decodeIR(setMin);
        do {

          lcd.setCursor(7, 0);
           if (irrecv.decode()) {
            if ((irrecv.decodedIRData.decodedRawData == 0xEA15FF00)) {
              setMin++;
              if (setMin >= 60) setMin = 0;
              paddingZero(setMin);
            }
            if ((irrecv.decodedIRData.decodedRawData == 0xF807FF00)) {
              setMin--;
              if (setMin < 0) setMin = 59;
              paddingZero(setMin);
            }
             irrecv.resume();
            if (irrecv.decodedIRData.decodedRawData == 0xBC43FF00) check = true;
            else check = false;
          }
                 
        } while (check == false);
        break;

        // Here begins the Date setting

      case 3:
        // decodeIR(setDOW);
        do {

          lcd.setCursor(0, 1);
          if (irrecv.decode()) {
            if ((irrecv.decodedIRData.decodedRawData == 0xEA15FF00)) {
              setDOW++;
              if (setDOW > 7) setDOW = 1;
              lcd.print(DayAsString(setDOW).substring(0, 3));
            }
            if ((irrecv.decodedIRData.decodedRawData == 0xF807FF00)) {
              setDOW--;
              if (setDOW < 1) setDOW = 7;
              lcd.print(DayAsString(setDOW).substring(0, 3));
            }
             irrecv.resume();
            if (irrecv.decodedIRData.decodedRawData == 0xBC43FF00) check = true;
            else check = false;
          }
               
        } while (check == false);
        break;

      case 4:
        // decodeIR(setDy);
        do {
          irrecv.resume();
          lcd.setCursor(6, 1);
          if (irrecv.decode()) {
            if ((irrecv.decodedIRData.decodedRawData == 0xEA15FF00)) {
              setDy++;
              if (setDy > lastDayOfTheMonth(setMon)) setDy = 1;
              paddingZero(setDy);
            }
            if ((irrecv.decodedIRData.decodedRawData == 0xF807FF00)) {
              setDy--;
              if (setDy < 1) setDy = lastDayOfTheMonth(setMon);
              paddingZero(setDy);
            }
             irrecv.resume();
            if (irrecv.decodedIRData.decodedRawData == 0xBC43FF00) check = true;
            else check = false;
          }            
                  
        } while (check == false);
        break;

      case 5:
        //decodeIR(setMon);
        do {
          irrecv.resume();
          lcd.setCursor(9, 1);
          if (irrecv.decode()) {
            if ((irrecv.decodedIRData.decodedRawData == 0xEA15FF00)) {
              setMon++;
              if (setMon > 12) setMon = 1;
              paddingZero(setMon);
            }
            if ((irrecv.decodedIRData.decodedRawData == 0xF807FF00)) {
              setMon--;
              if (setMon < 1) setMon = 12;
              paddingZero(setMon);
            }
            irrecv.resume();
            if (irrecv.decodedIRData.decodedRawData == 0xBC43FF00) check = true;
            else check = false;
          }
               
        } while (check == false);
        break;

      case 6:
        //decodeIR(setYr);
        do {

          lcd.setCursor(12, 1);
          if (irrecv.decode()) {
            if ((irrecv.decodedIRData.decodedRawData == 0xEA15FF00)) {
              setYr++;
              if (setYr > 2099) setYr = 2000;
              lcd.print(setYr);
            }
            if ((irrecv.decodedIRData.decodedRawData == 0xF807FF00)) {
              setYr--;
              if (setYr < 2000) setYr = 2099;
              lcd.print(setYr);
            }
            irrecv.resume();
            if (irrecv.decodedIRData.decodedRawData == 0xBC43FF00) check = true;
            else check = false;            
          }
                   
        } while (check == false);
        break;
      
        
    };
    myRTC.setDS1302Time(00, setMin, setHr, setDOW, setDy, setMon, setYr);        
    
  } while (setByRemote == true);
  lcd.setCursor(0, 0);
  delay(20);
  ///
}



// Function IncDec : increments/decrements any integer by pressing the left/Middle button respectively (used to set the clock)
void IncDec(int any) {
  if ((hourButton.getSingleDebouncedPress())) any++;
  else if ((stopwatchButton.getSingleDebouncedPress()))
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

// Function paddingZero : padds a 0 to any int under 10
void paddingZero(uint8_t num) {
  if (num < 10) {
    lcd.print('0');
    lcd.print(num);
  } else {
  lcd.print(num);
  }
}


// Function translate returns pressed cahrs of the irRemote
String translate() {      
         
    switch(irrecv.decodedIRData.decodedRawData){
      case 0xBA45FF00:
      return "CH-";
      break;
      case 0xB946FF00:
      
      return "CH";
      break;
      case 0xB847FF00:
      return "CH+";
      break;
      case 0xBB44FF00:
      return "|<<";
      break;
      case 0xBF40FF00:
      return ">>|";
      break ;  
      case 0xBC43FF00:
      return ">|";
      break ;               
      case 0xF807FF00:
      return "-";
      break ;  
      case 0xEA15FF00:
      return "+";
      break ;  
      case 0xF609FF00:
      return "EQ";
      break ;  
      case 0xE916FF00:
      return "0";
      break ;  
      case 0xE619FF00:
      return "100+";
      break ;
      case 0xF20DFF00:
      return "200+";
      break ;
      case 0xF30CFF00:
      return "1";
      break ;
      case 0xE718FF00:
      return "2";
      break ;
      case 0xA15EFF00:
      return "3";
      break ;
      case 0xF708FF00:
      return "4";
      break ;
      case 0xE31CFF00:
      return "5";
      break ;
      case 0xA55AFF00:
      return "6";
      break ;
      case 0xBD42FF00:
      return "7";
      break ;
      case 0xAD52FF00:
      return "8";
      break ;
      case 0xB54AFF00:
      return "9";
      break ;      
    } 
    irrecv.resume();   
}


void decodeIR() {      
         
  switch(irrecv.decodedIRData.decodedRawData){
    case 0xBA45FF00:
    Serial.print("CH-");
    decMode();      
    break;
    case 0xB946FF00:
    if (mode == Hour) setDateTime2();
    Serial.print("CH");
    break;
    case 0xB847FF00:
    Serial.print("CH+");
    incMode();      
    break;
    case 0xBB44FF00:
    Serial.print("|<<");
    break;
    case 0xBF40FF00:
    Serial.print(">>|");
    break ;  
    case 0xBC43FF00:
    Serial.print(">|");
    // pause = !pause;  
    break ;               
    case 0xF807FF00:
    Serial.print("-");
    break ;  
    case 0xEA15FF00:
    Serial.print("+");
    break ;  
    case 0xF609FF00:
    Serial.print("EQ");
    break ;  
    case 0xE916FF00:
    Serial.print("0");
    break ;  
    case 0xE619FF00:
    Serial.print("100+");
    lcd.noDisplay(); lcd.noBacklight();
    break ;
    case 0xF20DFF00:
    Serial.print("200+");
    lcd.display(); lcd.backlight();
    break ;
    case 0xF30CFF00:
    Serial.print("1");
    break ;
    case 0xE718FF00:
    Serial.print("2");
    break ;
    case 0xA15EFF00:
    Serial.print("3");
    break ;
    case 0xF708FF00:
    Serial.print("4");     
    break ;
    case 0xE31CFF00:
    Serial.print("5");
    break ;
    case 0xA55AFF00:
    Serial.print("6");
    break ;
    case 0xBD42FF00:
    Serial.print("7");
    break ;
    case 0xAD52FF00:
    Serial.print("8");
    break ;
    case 0xB54AFF00:
    Serial.print("9");

    break ;      
  }    
}

void writeAlarmSetIntoEEPROM(int address, uint8_t numbers[], int arraySize) {
    int addressIndex = address;
    for (int i = 0; i < arraySize; i++)  {
      EEPROM.write(addressIndex, numbers[i]);
      addressIndex++;
    }
}


void readAlarmSetFromEEPROM(int address, uint8_t numbers[], int arraySize) {
    int addressIndex = address;
    for (int i = 0; i < arraySize; i++) {
      numbers[i] = EEPROM.read(addressIndex);
      addressIndex++;
    }
}




```
