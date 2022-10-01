#include<Servo.h>

#define D2 4
#define D3 0
#define D8 15

#define OPEN_ANGLE 90
#define CLOSE_ANGLE 0

#define OPEN_DISTANCE 10
#define KEEP_TIME 3000

int trigger_pin = D3;
int echo_pin = D2;
Servo S;
unsigned long previous_time = 0;
unsigned long current_time = 0;
long distance;

void setup() {
   Serial.begin(9600);
   pinMode(echo_pin, INPUT);
   pinMode(trigger_pin, OUTPUT);
   
   S.attach(D8);
   S.write(CLOSE_ANGLE);
}

void loop() {

//  calculate the distance of an object
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigger_pin, LOW);
  distance = (pulseIn(echo_pin, HIGH) / 2) / 29.1;


  current_time  = millis();
  if((distance <= OPEN_DISTANCE && distance > 0)){
   Serial.println(current_time - previous_time);
   if((unsigned long)(current_time - previous_time >= KEEP_TIME))
      S.write(OPEN_ANGLE);
  }else{
    previous_time = current_time; 
  }
  if(distance > OPEN_DISTANCE){
    S.write(CLOSE_ANGLE);
   }
}
