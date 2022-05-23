#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//---------------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);
//PIN 5 R/W to Ground, for writing
//LCD Mode : 4 data pin
//Functions: Start, stop, saving up to 4 partial time in memory.
//10 millis = 1 hundredth of a second. The chronometer measures hour, minutes, seconds, and hundredths of a second.

//Buttons with internal Pullup, so if they get pressed we get LOW
const int start = 32;      //Start the stopwatch
const int partial_2 = 25;  //Pausa // sensor 2
const int partial = 4 ;   //Save a partial // sensor 1
const int scroll_partial = 33;  //If paused, check the 4 last saved partial
int x = 0; //Variable to manage the loop

//LCD
int lcdclear = 0; //this to variables are needed for managing
int Display = 0; //the display, to clear and print

//chronometer
int cents = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;

int ayam = 0;
const int interval = 10; //Every 10 milliseconds i increase  1 cent
unsigned long previousMillis = 0, Millissblum = 0;
int c1, c2, c3, s1, s2, m1, m2, h; //Variables used to put in the form
//h:m2m1:s2s1:c3c2c1

//Partial: I save 4 partial, that can be seen if stopwatch is stopped
int partial2[8]; //penultimate partial (The one that stays in Old). The last partial stays in New:
int partial3[8];
int partial4[8];
int partial5[8];
int partial6[8];
int partial7[8];
int partial8[8];
int partial9[8];
int scrolling = 0; //Used to scroll the saved partial times
int status = false;


void setup() {
  Serial.begin(115200);
  pinMode(start, INPUT); //In the schematic from right to left
  pinMode(partial_2, INPUT);//there are start-pausa-partial-scroll_partial
  pinMode(partial, INPUT);
  pinMode(scroll_partial, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press start");

}
int i = 0;
int a = millis();
int c ;


void loop() {
  if (x == 0) {  //Initially is 0
    while (digitalRead(start) == LOW) {}; //Until i press the button, the chronometer doesn't start
    status = !status;
    a = millis();
    x++; //When i press the button, i go out from this cycle, x++ and i cannot return here anymore
  }
  if (lcdclear == 0) {  //Condition to clear the display, used in the various functions
    lcd.clear();
    lcdclear++;
  }
  if (Display == 0) { //Also this is used to clear display
    lcd.home();

    lcd.print("Now:");  //With this condition, i can print "Now: " one single time, otherwise the chronometer wouldn't be precise
    Display++;
    scrolling = 0; //When i exit from the partial menu, then if i go in the partial menu again i always get in PAR1 and PAR2
  }
  chronometer(); //At the end of every increase, i control if stop is pressed. And also if the partial function is pressed
  pause1();
  f_partial();
}

void chronometer(void) {  //This function print:   "New: Actual time"

  unsigned long milisnow = millis();
  //cents= milisnow - Millissblum;

  c = millis();
  i = (c - a) ;


  unsigned long currentMillis = millis();  //If for the updating. If it is true, it means 1 cent of a second had passed. Update cents, minutes, seconds, hours and then i write on the lcd

  if (currentMillis - previousMillis >= interval) {
    previousMillis += currentMillis;



    cents = i;

    //Serial.println(cents);
    if (cents > 989) { //KALAU LAJU DARI ORIGINAL STOPWATCH +VALUE. KALAU LAMBAT -VALUE
      a = millis();
      cents = 0;
      seconds++;


      if (seconds == 60) {
        seconds = 0;
        minutes++;
        if (minutes == 60) {
          minutes = 0;
          hours++;
          if (hours == 24)
            hours = 0;
        }
      }
    }
    int cent = cents;
    int sec = seconds;
    int minu = minutes; //Taking the digits separeted
    h = hours;  //For the other funcionts, so i can put hours = 0 and h still is the last value



    //Serial.print("      milis: ");  Serial.print(millis()); Serial.print("      micros: ");  Serial.println(micros());
    c1 = cent % 10;
    cent /= 10;
    c2 = cent % 10;
    cent /= 10;
    c3 = cent % 10;

    s1 = sec % 10;
    sec /= 10;
    s2 = sec % 10;

    m1 = minu % 10;
    minu /= 10;
    m2 = minu % 10;

    lcd.setCursor(5, 0);
    lcd.print(h);
    lcd.print(':');
    lcd.print(m2);
    lcd.print(m1);
    lcd.print(':');
    lcd.print(s2);
    lcd.print(s1);
    lcd.print(':');
    lcd.print(c3);
    lcd.print(c2);
    lcd.print(c1);

  }
}

void scrollPartial(void) {
  while (digitalRead(scroll_partial) == HIGH) {}; //Debounce, as long as i press the button the real function doesn't start
  if (scrolling == 0) { //Visualize the last 2 partials
    lcd.clear();
    lcd.home();
    lcd.print("PAR1:");
    lcd.setCursor(5, 0);
    lcd.print(h);
    lcd.print(':');
    lcd.print(m2);
    lcd.print(m1);
    lcd.print(':');
    lcd.print(s2);
    lcd.print(s1);
    lcd.print(':');
    lcd.print(c3);
    lcd.print(c2);
    lcd.print(c1);
    lcd.setCursor(0, 1);
    lcd.print("PAR2:");
    lcd.setCursor(5, 1);
    lcd.print(partial2[0]);
    lcd.print(':');
    lcd.print(partial2[1]);
    lcd.print(partial2[2]);
    lcd.print(':');
    lcd.print(partial2[3]);
    lcd.print(partial2[4]);
    lcd.print(':');
    lcd.print(partial2[5]);
    lcd.print(partial2[6]);
    lcd.print(partial2[7]);
    Display = 0; //When i press start the display must be cleared
    lcdclear = 0; //When i press start the display must be cleared
    cents = seconds = minutes = hours  = 0;
    scrolling++;
  }
  else if (scrolling == 1) { //Visualize 3rd and 4th partial
    lcd.clear();
    lcd.home();
    lcd.print("PAR3:");
    lcd.setCursor(5, 0);
    lcd.print(partial3[0]);
    lcd.print(':');
    lcd.print(partial3[1]);
    lcd.print(partial3[2]);
    lcd.print(':');
    lcd.print(partial3[3]);
    lcd.print(partial3[4]);
    lcd.print(':');
    lcd.print(partial3[5]);
    lcd.print(partial3[6]);
    lcd.print(partial3[7]);
    lcd.setCursor(0, 1);
    lcd.print("PAR4:");
    lcd.setCursor(5, 1);
    lcd.print(partial4[0]);
    lcd.print(':');
    lcd.print(partial4[1]);
    lcd.print(partial4[2]);
    lcd.print(':');
    lcd.print(partial4[3]);
    lcd.print(partial4[4]);
    lcd.print(':');
    lcd.print(partial4[5]);
    lcd.print(partial4[6]);
    lcd.print(partial4[7]);
    Display = 0; //When i press start the display must be cleared
    lcdclear = 0; //When i press start the display must be cleared
    cents = seconds = minutes = hours  = 0;
    scrolling++;

  }

  else if (scrolling == 2) { //Visualize 5th and 6th partial
    lcd.clear();
    lcd.home();
    lcd.print("PAR5:");
    lcd.setCursor(5, 0);
    lcd.print(partial5[0]);
    lcd.print(':');
    lcd.print(partial5[1]);
    lcd.print(partial5[2]);
    lcd.print(':');
    lcd.print(partial5[3]);
    lcd.print(partial5[4]);
    lcd.print(':');
    lcd.print(partial5[5]);
    lcd.print(partial5[6]);
    lcd.print(partial5[7]);
    lcd.setCursor(0, 1);
    lcd.print("PAR6:");
    lcd.setCursor(5, 1);
    lcd.print(partial6[0]);
    lcd.print(':');
    lcd.print(partial6[1]);
    lcd.print(partial6[2]);
    lcd.print(':');
    lcd.print(partial6[3]);
    lcd.print(partial6[4]);
    lcd.print(':');
    lcd.print(partial6[5]);
    lcd.print(partial6[6]);
    lcd.print(partial6[7]);
    Display = 0; //When i press start the display must be cleared
    lcdclear = 0; //When i press start the display must be cleared
    cents = seconds = minutes = hours  = 0;
    scrolling++;

  }

  else if (scrolling == 3) { //Visualize 7th and 8th partial
    lcd.clear();
    lcd.home();
    lcd.print("PAR7:");
    lcd.setCursor(5, 0);
    lcd.print(partial7[0]);
    lcd.print(':');
    lcd.print(partial7[1]);
    lcd.print(partial7[2]);
    lcd.print(':');
    lcd.print(partial7[3]);
    lcd.print(partial7[4]);
    lcd.print(':');
    lcd.print(partial7[5]);
    lcd.print(partial7[6]);
    lcd.print(partial7[7]);
    lcd.setCursor(0, 1);
    lcd.print("PAR8:");
    lcd.setCursor(5, 1);
    lcd.print(partial8[0]);
    lcd.print(':');
    lcd.print(partial8[1]);
    lcd.print(partial8[2]);
    lcd.print(':');
    lcd.print(partial8[3]);
    lcd.print(partial8[4]);
    lcd.print(':');
    lcd.print(partial8[5]);
    lcd.print(partial8[6]);
    lcd.print(partial8[7]);
    Display = 0; //When i press start the display must be cleared
    lcdclear = 0; //When i press start the display must be cleared
    cents = seconds = minutes = hours  = 0;
    scrolling++;

  }

  else if (scrolling == 4) { //Visualize 9th and 10th partial
    lcd.clear();
    lcd.home();
    lcd.print("PAR9:");
    lcd.setCursor(5, 0);
    lcd.print(partial9[0]);
    lcd.print(':');
    lcd.print(partial9[1]);
    lcd.print(partial9[2]);
    lcd.print(':');
    lcd.print(partial9[3]);
    lcd.print(partial9[4]);
    lcd.print(':');
    lcd.print(partial9[5]);
    lcd.print(partial9[6]);
    lcd.print(partial9[7]);
    /*lcd.setCursor(0, 1);
      lcd.print("PAR10:");
      lcd.setCursor(6, 1);
      lcd.print(partial10[0]);
      lcd.print(':');
      lcd.print(partial10[1]);
      lcd.print(partial10[2]);
      lcd.print(':');
      lcd.print(partial10[3]);
      lcd.print(partial10[4]);
      lcd.print(':');
      lcd.print(partial10[5]);
      lcd.print(partial10[6]);*/
    Display = 0; //When i press start the display must be cleared
    lcdclear = 0; //When i press start the display must be cleared
    cents = seconds = minutes = hours  = 0;
    scrolling = 0;

  }

}


void pause1(void) { //If pause is pressed, i stop in this function until start doesn't get pressed again
  if (digitalRead(start) == LOW)
    return;
  else if (digitalRead(start) == status) { //Stuck in this cycle until i press start
    delay(150);
    while (digitalRead(start) == HIGH) {
      if (digitalRead(scroll_partial) == HIGH) //If i press the button for seeing the partial, i enter in that function
        scrollPartial();    //When scrollPartial() ends, i'm still in this function, so if i press start the chronometer starts back normal
    }
  }
}

void f_partial(void) {  //If this button is pressed, i put the current value of New in Old, and a new crhonometer starts
  if (digitalRead(partial) == HIGH || digitalRead(partial_2) == HIGH)
    return;
  else if (digitalRead(partial) == LOW || digitalRead(partial_2) == LOW  ) {
    lcd.clear();
    lcd.setCursor(0, 1); //The values calculated in the function chronometer can be used,  h,m,s,c
    lcd.print("Old: ");
    lcd.setCursor(5, 1);
    lcd.print(h);
    lcd.print(':');
    lcd.print(m2);
    lcd.print(m1);
    lcd.print(':');
    lcd.print(s2);
    lcd.print(s1);
    lcd.print(':');
    lcd.print(c3);
    lcd.print(c2);
    lcd.print(c1);   //When i come here, i've got the old values for h,m,s,c,                       i save it in the partial array
    //    Display = 0;   //The new is written again
    //    cents = 0;
    //    seconds = 0;
    //    minutes = 0;
    //    hours = 0;

    String checkpoint = String(h)+":"+String(m2)+String(m1)+":"+String(s2)+String(s1)+":"+String(c3)+String(c2)+String(c1);
    Serial.println(checkpoint);

    partial2[0] = partial3[0]; //Partial6[] is updated with the old partial3[]
    partial2[1] = partial3[1];
    partial2[2] = partial3[2];
    partial2[3] = partial3[3];
    partial2[4] = partial3[4];
    partial2[5] = partial3[5];
    partial2[6] = partial3[6];
    partial2[7] = partial3[7];


    partial3[0] = partial4[0]; //Partial5[] is updated with the old partial2[]
    partial3[1] = partial4[1];
    partial3[2] = partial4[2];
    partial3[3] = partial4[3];
    partial3[4] = partial4[4];
    partial3[5] = partial4[5];
    partial3[6] = partial4[6];
    partial3[7] = partial4[7];

    partial4[0] = partial5[0]; //Partial4[] is updated with the old partial3[]
    partial4[1] = partial5[1];
    partial4[2] = partial5[2];
    partial4[3] = partial5[3];
    partial4[4] = partial5[4];
    partial4[5] = partial5[5];
    partial4[6] = partial5[6];
    partial4[7] = partial5[7];


    partial5[0] = partial6[0]; //Partial3[] is updated with the old partial2[]
    partial5[1] = partial6[1];
    partial5[2] = partial6[2];
    partial5[3] = partial6[3];
    partial5[4] = partial6[4];
    partial5[5] = partial6[5];
    partial5[6] = partial6[6];
    partial5[7] = partial6[7];

    partial6[0] = partial7[0]; //Partial3[] is updated with the old partial2[]
    partial6[1] = partial7[1];
    partial6[2] = partial7[2];
    partial6[3] = partial7[3];
    partial6[4] = partial7[4];
    partial6[5] = partial7[5];
    partial6[6] = partial7[6];
    partial6[7] = partial7[7];

    partial7[0] = partial8[0]; //Partial3[] is updated with the old partial2[]
    partial7[1] = partial8[1];
    partial7[2] = partial8[2];
    partial7[3] = partial8[3];
    partial7[4] = partial8[4];
    partial7[5] = partial8[5];
    partial7[6] = partial8[6];
    partial7[7] = partial8[7];

    partial8[0] = partial9[0]; //Partial3[] is updated with the old partial2[]
    partial8[1] = partial9[1];
    partial8[2] = partial9[2];
    partial8[3] = partial9[3];
    partial8[4] = partial9[4];
    partial8[5] = partial9[5];
    partial8[6] = partial9[6];
    partial8[7] = partial9[7];

    partial9[0] = h;    //Update partial6 with OLD
    partial9[1] = m2;
    partial9[2] = m1;
    partial9[3] = s2;
    partial9[4] = s1;
    partial9[5] = c3;
    partial9[6] = c2;
    partial9[7] = c1;



    while (digitalRead(partial) == LOW) {}; //Debounce, until i press the button i stay here
  }
}
