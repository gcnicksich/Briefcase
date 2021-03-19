/*  THIS IS A BRIEFCASE STYLE "BOMB" (PROP USE ONLY FOR ESCAPE ROOMS) BUT CAN BE USED FOR OTHER ADAPTATIONS SUCH AS OPERATING A LOCK, OPERATING A SERVO, ETC
     I FOLLOWED ALONG WITH A BOOK CALLED "ARDUINO PROJECT HANDBOOK, VOL 2"
     I MODIFIED THE CODE FROM THERE TO FIT MY NEEDS. I WANTED AN UNLIMITED NUMBER OF GUESSED PASSWORDS TO BE ABLE TO BE ENTERED
     IF THE TIMER RUNS OUT, THE "BOMB" EXPLODES


COMPONENTS:  
ARDUINO UNO 
2X16 LCD DISPLAY
PIEZO BUZZER
COLORED LED'S
POTENTIOMETER
*/


#include <Tone.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
Tone tone1;

int Scount = 00;                     //Number of seconds to start with
int Mcount = 01;                    //Number of Minutes to start with
int Hcount = 00;                    // Number of hours to start with
int DefuseTimer = 0;                //Set timer to 0
long secMillis = 60;
long Millisec = 0;                // Store last time for second add
long interval = 1000;               //Interval for seconds

char password[5];                  //number of characters in password
int currentLength = 0;              //Defines number currently writing
int i = 0;
char entered[5];

int ledPin = 2;                     //Green LED  220 ohm resistor
int ledPin2 = 3;                     //Yellow LED  220 ohm resistor
int ledPin3 = 4;                     //Red LED   220 ohm resistor //Define pins used on LCD
LiquidCrystal lcd(7, 8, 10, 11, 12, 13);

const char F1    = 'A';
const char F2    = 'B';
const char F3    = 'C';
const char F4    = 'D';
const char START = '*';
const char STOP  = '#';

const byte ROWS = 4; //4 Rows on Matrix Keypad
const byte COLS = 4; //4 Columns on Matrix Keypad
char keys [ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},        // F3 KEY WILL BE USED TO CLEAR ENTRY OF CODE IF MISTAKE MADE
  {'*', '0', '#', 'D'}      // START WILL BE USED TO ARM BOMB, STOP WILL BE USED TO DISARM BOMB AFTER  5 DIGIT CODE IS ENTERED  (NEED HELP PROGRAMMING THIS)
};

byte rowPins[ROWS] = {6, A3, A0, A1}; // Connect to the Row pinouts of the keypad

byte colPins[COLS] = {A2, A4, A5, 5};    //Connect to the Column Pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void timer(void);

void setup () {
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  tone1.begin (9);
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER CODE: ");  //THIS IS WHERE YOU ENTER YOUR SECRET CODE
  lcd.setCursor (0, 1);
  delay (200);
  lcd.setCursor (0, 0);
  while (currentLength < 5) {
    lcd.setCursor(currentLength + 11, 0);
    lcd.cursor();
    char key = keypad.getKey();
    key == NO_KEY;
    if (key != NO_KEY) {
      if ((key != START) & (key != STOP)) {
        lcd.print(key);
        password[currentLength] = key;
        currentLength++;
        tone1.play(NOTE_C6, 250);

      }
    }
  }
  if (currentLength == 5) {
    delay(500);
    lcd.noCursor();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("YOU'VE ENTERED: ");  //YOUR SECRET CODE IS STORED AND SHOWN TO YOU BEFORE GOING AWAY
    lcd.setCursor (7, 1);
    lcd.print(password[0]);
    lcd.print(password[1]);
    lcd.print(password[2]);
    lcd.print(password[3]);
    lcd.print(password[4]);
    tone1.play(NOTE_E6, 250);
    delay(3000);                //ADJUSTABLE DELAY TO SHOW YOUR ENTERED CODE FOR A SPECIFIED TIME BEFORE CLEARING
    lcd.clear();
    currentLength = 0;
    lcd.setCursor (0, 0);
    lcd.print("PRESS START");  //YOUR SECRET CODE IS NOW INVISIBLE AND UNTIL START IS PRESSED, NOTHING WILL HAPPEN
    lcd.setCursor(0, 1);
    lcd.print("TO BEGIN");
    char key2 = keypad.getKey();
    while (key2 != START) {
      key2 = keypad.getKey();
    }
  }
}

void loop() {

  lcd.clear();
  currentLength = 0;
  delay(500);
  timer();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print ("CODE:        F3");
  while (currentLength < 5) {
    timer();
    char key2 = keypad.getKey();
    if (key2 == F3) {
      currentLength = 0;
      lcd.clear();
      lcd.setCursor (0, 0);
      lcd.print("CODE:        F3");
      timer();
    }
    else if (key2 != NO_KEY) {
      lcd.setCursor(currentLength + 5, 0);
      lcd.cursor();
      entered[currentLength] = key2;
      currentLength++;
      tone1.play(NOTE_C6, 250);
      delay(100);
      lcd.noCursor();
      lcd.setCursor(currentLength + 5, 0);
      lcd.print(key2);
      lcd.setCursor(currentLength + 6, 0);
      lcd.cursor();
    }
  }
  if (currentLength == 5) {
    if (entered[0] == password[0] && entered [1] == password[1] && entered[2] == password[2] && entered[3] == password[3] && entered[4] == password[4]) {
      lcd.noCursor();
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("CORRECT CODE");
      lcd.setCursor(2, 1);
      lcd.print("BOMB DEFUSED");
      currentLength = 0;
      digitalWrite(ledPin3, HIGH);
      delay (3000);
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor (2, 0);
      lcd.print("CASE IS SAFE");
      lcd.setCursor (4, 1);
      lcd.print("TO OPEN");
      delay (3000);
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("CASE CODE IS:");
      lcd.noCursor();
      lcd.setCursor(7, 1);
      lcd.print("223");
      while (1);//infinite loop to first line
    }

    else {
      lcd.noCursor();
      lcd.clear();
      lcd.home();
      lcd.print("WRONG PASSWORD!");   //TIME WILL CONTINUE TO COUNT DOWN FROM PREVIOUS SECOND AFTER THE DELAY
      lcd.setCursor(0, 1);
      lcd.print("TRY AGAIN");
      delay (3000);
      timer();
    }
  }
}




  void timer() {
    Serial.print(Scount);
    Serial.println();
    if (Hcount <= 0) { // Timer reaches 0, LCD Displays Explosion, and buzzer sounds, Red Light turns on
      if ( Mcount < 0 ) {
        lcd.noCursor();
        lcd.clear();
        lcd.home();
        lcd.print("BOMB HAS BEEN");
        lcd.setCursor(0, 1);
        lcd.print("DETONATED");
        while (Mcount < 0) {
          digitalWrite(ledPin, HIGH);       //Red Light on
          tone1.play(NOTE_A2, 90);
          delay(100);
          digitalWrite(ledPin, LOW);        //Red Light off
          tone1.play(NOTE_A2, 90);
          delay(100);
          digitalWrite(ledPin2, HIGH);       //Yellow Light on
          tone1.play(NOTE_A2, 90);
          delay(100);
          digitalWrite(ledPin2, LOW);        //Yellow Light off
          tone1.play(NOTE_A2, 90);
          delay(100);
          digitalWrite(ledPin3, HIGH);       //Green Light on
          tone1.play(NOTE_A2, 90);
          delay(100);
          digitalWrite(ledPin3, LOW);        //Green Light off
          tone1.play(NOTE_A2, 90);
          delay(100);
        }
      }
    }
    lcd.setCursor(0, 1); //sets cursor to second line
    lcd.print("TIMER: ");
    if (Hcount >= 10) {
      lcd.setCursor (7, 1);
      lcd.print (Hcount);
    }
    if (Hcount < 10) {
      lcd.setCursor (7, 1);
      lcd.print(Hcount);
      lcd.write("0");
      lcd.setCursor (8, 1);
      lcd.print(Hcount);
    }
    lcd.print(":");
    if (Mcount >= 10) {
      lcd.setCursor(10, 1);
      lcd.print(Mcount);
    }
    if (Mcount < 10) {
      lcd.setCursor(10, 1);
      lcd.write("0");
      lcd.setCursor(11, 1);
      lcd.print(Mcount);
    }
    lcd.print (":");
    if (Scount >= 10) {
      lcd.setCursor(13, 1);
      lcd.print(Scount);
    }
    if (Scount < 10) {
      lcd.setCursor(13, 1);
      lcd.write("0");
      lcd.setCursor(14, 1);
      lcd.print(Scount);
    }
    if (Hcount < 0) {
      Hcount = 0;
    }
    if (Mcount < 0) {
      Hcount --;
      Mcount = 59;
    }
    if (Scount < 1) { // if 60 do this operation
      Mcount --; // Add 1 to Mcount
      Scount = 59; //Reset Scount
    }
    if (Scount > 0) { // Do this operation 59 times
      unsigned long currentMillis = millis();
      if (currentMillis - secMillis > interval) {
        tone1.play(NOTE_G5, 500);
        secMillis = currentMillis;
        Scount --; //Add 1 to Scount
        digitalWrite(ledPin2, HIGH); // Sets yellow light on
        delay(50); //waits for .5 seconds
        digitalWrite(ledPin2, LOW); // Sets yellow LED off
        delay(50); // Waits .5 seconds
      }
    }
  }
