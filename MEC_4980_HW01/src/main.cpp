//HW01 Cole Hill
#include <Arduino.h>

//Timers for green and red LED stuffs
int starttime =0;
unsigned long flashtime=0; //saves flash start time
unsigned long lastToggle=0; //saves time since last

bool flashing = false; // for green led
bool GREENLEDSTATE = LOW; // green led start state
bool REDLEDLEDLITUPONETIME = false; // is true after full 3 seconds
bool redTimerTiming =false; // is 3 second red led on

//Fail Safe RED button
//bool REDbuttonState =false; // failed attempt
bool REDbuttonlastState= false;
int redPressCount=0; //counting red presses to add saftey feature
bool yellowWorks=false; //gonna trigger yellow with this

#define redLED A0
#define yellowLED A1
#define greenLED A2
#define blueLED A3

void setup() {
  // put your setup code here, to run once:
  pinMode(5,INPUT);
  pinMode(6, INPUT);
  pinMode(9, INPUT);
  pinMode(10,INPUT);
  pinMode(redLED,OUTPUT);
  pinMode(yellowLED,OUTPUT);
  pinMode(greenLED,OUTPUT);
  pinMode(blueLED,OUTPUT);
  Serial.begin(9600);//make sure to add this or wokwki breaks
  }

void loop() {
  // put your main code here, to run repeatedly:
unsigned long now =millis();

  bool redbutton = digitalRead(10);
  bool yellowbutton = digitalRead(9);
  bool greenbutton = digitalRead(6);
  bool bluebutton = digitalRead(5);

  //button edge for red
  bool redPressedEdge = (redbutton &&!REDbuttonlastState);

  if (redPressedEdge) {
    // Starts 3s 
      if (!redTimerTiming && !REDLEDLEDLITUPONETIME) {
      redTimerTiming = true;
      starttime = now;
      digitalWrite(redLED, HIGH);
    }
    // else if to cancel if pressed whike LED ON
    else if (redTimerTiming && (now - starttime < 3000)) {
      redTimerTiming = false; // stops led timer
      digitalWrite(redLED, LOW);
      starttime = 0; //resets it
      yellowWorks = false; //lock out yellow just in case
    }
//counts presses for yellow lock out
    redPressCount++;
    //if (redPressCount == 1) {
      //Serial.println(" ");
    //}
    if (redPressCount >= 2) {
      Serial.println("Device Ready to load");
      yellowWorks = true; //lets yellow work
      redPressCount = 0; //rests count after yellow allowed to work
    }
  }

  // turns off red led if not canceled
  if (redTimerTiming && (now - starttime >= 3000)) {
    redTimerTiming = false;
    digitalWrite(redLED, LOW);
    REDLEDLEDLITUPONETIME = true;
    starttime = 0;
    Serial.println("Device charged");
  }
  REDbuttonlastState = redbutton;

//YELLOW LED

if (yellowbutton&& yellowWorks&& REDLEDLEDLITUPONETIME){
  digitalWrite(yellowLED,HIGH);
  yellowWorks=false; //resets after pressed once
  Serial.println("Device loaded");
};

//GREEN LED
 if (greenbutton && !flashing && digitalRead(yellowLED) == HIGH) {
    flashing = true; // allowed to flash
    flashtime = now;//starts 5 second timer
    lastToggle = now;//so it can flip between on and off
    GREENLEDSTATE = HIGH;
    digitalWrite(greenLED, GREENLEDSTATE);
    Serial.println("Explosion Imminent");
  }

  if (flashing) {
    // if allowed to flash,toggle every 500ms
    if (now - lastToggle >= 500) {
      GREENLEDSTATE = !GREENLEDSTATE;
      digitalWrite(greenLED, GREENLEDSTATE);
      lastToggle = now; //restarts 500ms count
    }

    // stop flash after 5s
    if (now - flashtime >= 5000) {
      flashing = false;
      digitalWrite(greenLED, LOW);
      digitalWrite(blueLED, HIGH);
      digitalWrite(yellowLED, LOW);
      Serial.println("!!!!!!!!BOOOOM!!!!!");
    }
  }

if (bluebutton) {
      digitalWrite(blueLED,LOW);
      REDLEDLEDLITUPONETIME=false;
      yellowWorks=false;
      delay(500);//trying to fix unloaded spammmmmmm
      Serial.println("Unloaded");
      };
}
