#include <Servo.h>

// functions 
void printvals();


// pins
#define D8 15
#define D0 16 
#define D1 5
#define D2 4


// angels 
#define LEFT_ANGEL 45
#define RIGHT_ANGEL 135
#define STRAIGHT_ANGEL 90

// sensor readings
int left_ldr = 0;
int center_ldr = 0;
int right_ldr = 0;

Servo servo;


void setup() {
  Serial.begin(9600);
  
//  initialize servo
  servo.attach(D8);
  servo.write(STRAIGHT_ANGEL);
  delay(1000);

//  LDR pins
  pinMode(D0, INPUT); // left ldr
  pinMode(D1, INPUT); // center ldr
  pinMode(D2, INPUT); // right ldr

}

void loop() {
  left_ldr = digitalRead(D0);
  center_ldr = digitalRead(D1);
  right_ldr = digitalRead(D2);
  printVals();
  if(center_ldr == LOW)
    servo.write(STRAIGHT_ANGEL);
  else if(right_ldr == LOW)
    servo.write(LEFT_ANGEL);
  else if(left_ldr == LOW)
    servo.write(RIGHT_ANGEL);

}
void printVals(){
  Serial.print(" left ");
  Serial.print(left_ldr);

  Serial.print(" center ");
  Serial.print(center_ldr);

  Serial.print(" right ");
  Serial.print(right_ldr);

  Serial.println("");
}
