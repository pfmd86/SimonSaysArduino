// Simon Says 2
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#define LED_1 8
#define LED_2 9
#define LED_3 10
#define LED_4 11
#define BUZZER 6
#define BTN_1 2
#define BTN_2 3
#define BTN_3 4
#define BTN_4 5
#define START_BTN 1
const char LED[] = {LED_1, LED_2, LED_3, LED_4};
const char BTN[] = {BTN_1, BTN_2, BTN_3, BTN_4};
const int FRQ[] = {300, 500, 700, 900};
const int MAX_LEVEL = 100;
int GAME[MAX_LEVEL];
int player_input[MAX_LEVEL];
int level = 1;
int velocity = 500;
int points = 0;
int len = 0;
char *words[] = {"FAIL","POOP","BOOO","DAMN","FUCK","SHIT","MIST","UPPS","LOST"};
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

void setup() {
  // Start serial communication for debugging
  // Serial.begin(9600);
  // Start display communication
  alpha4.begin(0x70);
  // SET LED PINS TO OUTPUT
  for (int n = 0; n < sizeof(LED); n ++) {
    pinMode(LED[n], OUTPUT);
  }
  // SET BUTTON PINS TO INPUT
  for (int n = 0; n < sizeof(BTN); n ++) {
    pinMode(BTN[n], INPUT_PULLUP);
  }
  // SET BUZZER PIN TO OUTPUT
  pinMode(BUZZER, OUTPUT);
  pinMode(START_BTN, INPUT);

}

void loop() {
  if (level == 1) {
    generatepattern();
  }
  if (digitalRead(START_BTN) == LOW || level != 1) //If start button is pressed or you're winning
  {
    showpattern();    //show the sequence
    getpattern();     //wait for your sequence
  }
}

void generatepattern() {
  // generates the game pattern
  // called in loop()
  randomSeed(millis());
  for (int i = 0; i < MAX_LEVEL; i++) {
    GAME[i] = random(0, sizeof(LED));
  }
}
void showpattern() {
  //function to play the game pattern
  for (int i = 0; i < sizeof(LED); i++) {
    digitalWrite(LED[i], LOW);
  }
  for (int i = 0; i < level; i++) {
    int seq = GAME[i];
    digitalWrite(LED[seq], HIGH);
    tone(BUZZER, FRQ[seq]);
    delay(velocity);
    digitalWrite(LED[seq], LOW);
    noTone(BUZZER);
    delay(200);
  }
}
void getpattern() {
  int flag = 0;
  for (int i = 0; i < level; i++) {
    flag = 0;
    while (flag == 0) {
      if (digitalRead(BTN_1) == LOW) {
        digitalWrite(LED_1, HIGH);
        tone(BUZZER, FRQ[0]);
        player_input[i] = 0;
        flag = 1;
        delay(200);
        if (player_input[i] != GAME[i]) {
          wrongpattern();
          return;
        }
        digitalWrite(LED_1, LOW);
        noTone(BUZZER);
      }
      if (digitalRead(BTN_2) == LOW) {
        digitalWrite(LED_2, HIGH);
        tone(BUZZER, FRQ[1]);
        player_input[i] = 1;
        flag = 1;
        delay(200);
        if (player_input[i] != GAME[i]) {
          wrongpattern();
          return;
        }
        digitalWrite(LED_2, LOW);
        noTone(BUZZER);
      }
      if (digitalRead(BTN_3) == LOW) {
        digitalWrite(LED_3, HIGH);
        tone(BUZZER, FRQ[2]);
        player_input[i] = 2;
        flag = 1;
        delay(200);
        if (player_input[i] != GAME[i]) {
          wrongpattern();
          return;
        }
        digitalWrite(LED_3, LOW);
        noTone(BUZZER);
      }
      if (digitalRead(BTN_4) == LOW) {
        digitalWrite(LED_4, HIGH);
        tone(BUZZER, FRQ[3]);
        player_input[i] = 3;
        flag = 1;
        delay(200);
        if (player_input[i] != GAME[i]) {
          wrongpattern();
          return;
        }
        digitalWrite(LED_4, LOW);
        noTone(BUZZER);
      }
    }
  }
  rightpattern();
}

void wrongpattern() {
  // triggered when the input pattern was not equal to the game pattern
  // called in getpattern()
  for (int i = 0; i < 1; i++) {
    for (int n = 0; n < sizeof(LED); n++) {
      digitalWrite(LED[n], HIGH);
    }
    tone(BUZZER, 400);
    delay(200);
    tone(BUZZER, 200);
    delay(200);
    tone(BUZZER, 100);
    delay(200);
    showLostMessage();
    for (int n = 0; n < sizeof(LED); n++) {
      digitalWrite(LED[n], LOW);

    }
    noTone(BUZZER);
    delay(250);
  }
  delay(1000);
  alpha4.clear();
  int lastscore = level - 1;
  showPoints(lastscore);
  level = 1;
  velocity = 500;

}

void rightpattern() {
  //triggered when the input pattern was equal to the game getpattern
  //called in getpattern()
  for (int i = 0; i < 1; i++) {
    for (int n = 0; n < sizeof(LED); n++) {
      digitalWrite(LED[n], LOW);
    }
    delay(250);
    for (int n = 0; n < sizeof(LED); n++) {
      //digitalWrite(LED[n], HIGH);
      tone(BUZZER, 1600, 20);
      delay(20);
      tone(BUZZER, 1600, 20);
    }
    delay(500);
  }
  if (level < MAX_LEVEL) {
    // show points
    showPoints(level);
    // increase level
    level++;
    // reduce velocity to speed up the game
    velocity -= 20;

  }
}

void showPoints(int points) {
  //shows the current points after each correct round & in the end of the game
  // called in rightpattern() and wrongpattern() function
  alpha4.clear();
  String punkte = String(points);
  len = punkte.length();
  if (len == 1) {
    alpha4.writeDigitAscii(3, punkte.charAt(0));
  } else if (len == 2) {
    alpha4.writeDigitAscii(3, punkte.charAt(1));
    alpha4.writeDigitAscii(2, punkte.charAt(0));
  }
  // write it out!
  alpha4.writeDisplay();
}

void showLostMessage() {
  // Shows one of the 4-character words from the words array when user lost the game
  // called in the wrongpattern() function
  // Random number, change the max if to your words array size
  randomSeed(analogRead(7));
  int randnum = random(0, 9);
  String currentword = words[randnum];
  alpha4.writeDigitAscii(0, currentword.charAt(0));
  alpha4.writeDigitAscii(1, currentword.charAt(1));
  alpha4.writeDigitAscii(2, currentword.charAt(2));
  alpha4.writeDigitAscii(3, currentword.charAt(3));
  alpha4.writeDisplay();
  delay(1000);
}
