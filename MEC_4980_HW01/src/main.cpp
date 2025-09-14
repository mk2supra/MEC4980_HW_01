//HW01 Cole Hill
#include <Arduino.h>

// put function declarations here:
int switchpin = 5;
int starttime =0;
//FOR GREEN LED FLASHING
//millis counts for 49 days  unsigned long make sure any
//math with millis works when millis rolls over
unsigned long flashtime=0; //saves flash start time
unsigned long lastToggle=0; //saves time since last
bool flashing = false;
bool GREENLEDSTATE = LOW;
bool REDLEDLEDLITUPONETIME = false;

//Fail Safe RED button
bool REDbuttonState =false;
bool REDbuttonlastState= false;
int redPressCount=0; //gonna try counting red presses to add saftey feature
bool yellowWorks=false; //gonna trigger yellow with this

#define redLED A0
#define yellowLED A1
#define greenLED A2
#define blueLED A3

void setup() {
  // put your setup code here, to run once:
  pinMode(5,INPUT);//pullup checks for low
  pinMode(6, INPUT);
  pinMode(9, INPUT);
  pinMode(10,INPUT);
  pinMode(redLED,OUTPUT);
  pinMode(yellowLED,OUTPUT);
  pinMode(greenLED,OUTPUT);
  pinMode(blueLED,OUTPUT);
  Serial.begin(9600);
  }

void loop() {
  // put your main code here, to run repeatedly:
  bool redbutton = digitalRead(10);
  bool yellowbutton = digitalRead(9);
  bool greenbutton = digitalRead(6);
  bool bluebutton = digitalRead(5);

  if (redbutton && starttime ==0 && !REDLEDLEDLITUPONETIME) {
    digitalWrite(redLED,HIGH);
    Serial.println("Charging");
starttime=millis();
  };
  if (millis() -starttime >=3000 && starttime !=0){
digitalWrite(redLED, LOW);
REDLEDLEDLITUPONETIME=true;
starttime=0;
Serial.println("Device charged");
  };

//RED button toggle switch memory thing saftey
REDbuttonState=digitalRead(redbutton)==LOW;
if (REDbuttonState && !REDbuttonlastState){
  redPressCount++; // ++means add 1 to current value
  if (redPressCount==1){
  Serial.println(" ");//nothing happens but code bricks with out this line
  }
  if (redPressCount>=2) {
    Serial.println("Device Ready to load");
    yellowWorks=true;
    redPressCount=0; //resets it
    } 
}
REDbuttonlastState=REDbuttonState;

if (yellowbutton&& yellowWorks){
  digitalWrite(yellowLED,HIGH);
  yellowWorks=false;
  Serial.println("Device loaded");
};

unsigned long currenttime=millis();//needs to be in loop or wont work
 // green led flash if button pressed code

 //if green button pressed and not flashing then & yellow led on
  if (greenbutton && !flashing &&digitalRead(yellowLED) == HIGH) {
    flashing = true; //then start flashing
    flashtime = currenttime; // start time counter for 5s window
    lastToggle = currenttime; // time counter of last LED flip
    GREENLEDSTATE = HIGH; // turn green led on
    digitalWrite(greenLED, GREENLEDSTATE);
    Serial.println("Explosion Imminent");
  }

  if (flashing) {
    // if been flashing for 500ms turn off
    if (currenttime - lastToggle >= 500) {
      GREENLEDSTATE = !GREENLEDSTATE; //! flips state like opposite
      digitalWrite(greenLED, GREENLEDSTATE);
      lastToggle = currenttime;
    }

    // stop green led flashing after 5 second ending countdown
    if (currenttime - flashtime >= 5000) {
      flashing = false;
      digitalWrite(greenLED, LOW);
      digitalWrite(blueLED,HIGH);
      digitalWrite(yellowLED,LOW);
      Serial.println("!!!!!!!!BOOOOM!!!!!");
    }
  }

if (bluebutton) {
      digitalWrite(blueLED,LOW);
      REDLEDLEDLITUPONETIME=false;
      delay(500);//trying to fix unloaded spammmmmmm
      Serial.println("Unloaded");
      };
}
