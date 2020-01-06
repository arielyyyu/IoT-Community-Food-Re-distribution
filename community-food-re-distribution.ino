// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>

// This code is for Siot
// Red signal
HX711ADC scale(A1, A0);

// set up servo
Servo myservo;
int servoPos = 0;
int servoPin = A3;  //declare variable for servo
int pos = 0;        //variable to keep track of the servo's position
int lastPos = 0;        //variable to keep track of the servo's position
bool flag = 1;      //variable to keep track of the button presses
bool motorOn = false;
int difference = 0;

// set up fsr
int forcePin = A5;
int forceVal = 0;

// set up LED
int blueLEDPin = D4;
int yellowLEDPin = D2;
int myLEDPin = D0;
bool blueLEDOn = false;
bool yellowLEDOn = false;
bool myLEDOn = false;

// set up button
int button = D8;
bool lastState = false;
bool currentState = false;



void setup()
{
    // set up pinMode
    Serial.begin(9600);
  pinMode(blueLEDPin, OUTPUT);
  pinMode(yellowLEDPin, OUTPUT);
  pinMode(myLEDPin, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  scale.begin();
  
  // subscribe
  Particle.subscribe(  "diot/2019/final/tea/blue" , handleBlueEvent );
  Particle.subscribe(  "diot/2019/final/tea/yellow" , handleYellowEvent );
  
  // setup servo
  myservo.attach(servoPin);  //Initialize the servo attached to pin D0
  myservo.write(180);        //set servo to furthest position
  delay(500);                //delay to give the servo time to move to its position
  myservo.detach();

}

void loop()
{
  forceVal = scale.get_units();
  
  
    currentState = (digitalRead(button) == LOW);
    pos = map(forceVal, 110000, 900000, 0, 180);
    pos = 180 - pos;
    pos = (pos > 180) ? 180 : pos;
    pos = (pos < 0) ? 0 : pos;
    Serial.print("force");
    Serial.println(forceVal, 1);
    Serial.println(pos, 1);
    //Serial.println(forceVal);
    
    // publish my event
    // you'll want some more complex stuff here
    if (currentState) {
        //publishMyEvent();
        myLEDOn = true;
        motorOn = true;
    } else {
        myLEDOn = false;
        motorOn = false;
    }
    
    if (currentState != lastState) {
       publishMyEvent(); 
       Serial.println(currentState);
    }
    
    // blue LED
    if (blueLEDOn) { 
        digitalWrite(blueLEDPin, HIGH); 
    } else {
        digitalWrite(blueLEDPin, LOW); 
    }
    
    // yellow LED
    if (yellowLEDOn) { 
        digitalWrite(yellowLEDPin, HIGH); 
    } else {
        digitalWrite(yellowLEDPin, LOW); 
    }
    
    // red LED
    if (myLEDOn) { 
        digitalWrite(myLEDPin, HIGH); 
    } else {
        digitalWrite(myLEDPin, LOW); 
    }
    
    difference = lastPos - pos;
    
    if ((difference > 5) || (difference < -5)) {
        if ((difference < 170) && (difference > -170)) {

      myservo.attach(servoPin);
      myservo.write(pos);
      delay(500);           //debounce and give servo time to move
      myservo.detach();
      //Serial.println(pos);  //prints to the serial port to keep track of the position
      
       Serial.println("::::::::::::::");
       Serial.println(difference);
       Serial.println("::::::::::::::");
       lastPos = pos;
       
            
        } else {
             Serial.println("-------------------");
             Serial.println("-------------------");
        }
    } else {
             Serial.println("no change");
        }

    
    lastState = currentState;
    
    // delay for a bit
    delay(100);
}


void publishMyEvent()
{
    
      String eventName = "diot/2019/final/tea/red" + System.deviceID();
      Particle.publish( eventName, "data goes here" );

}

void handleBlueEvent(const char *event, const char *data)
{

    blueLEDOn = !blueLEDOn;
    Serial.print(blueLEDOn);
    
}

void handleYellowEvent(const char *event, const char *data)
{

    yellowLEDOn = !yellowLEDOn;
    Serial.print(yellowLEDOn);
    
}