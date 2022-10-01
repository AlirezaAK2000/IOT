#define D2 4
#define D4 2
#define D6 12


int ldr_value = 0;
 int ldr_value_percent = 0;

void setup() {
   Serial.begin(9600);
   pinMode(D2, OUTPUT); // RED LED
   pinMode(D4, OUTPUT); // GREEN LED
   pinMode(D6, OUTPUT); // BUZZER
}

void loop() {
   ldr_value = analogRead(A0);
   ldr_value_percent = map(ldr_value, 0, 1023, 0, 100);
   Serial.println(ldr_value_percent);
   if (ldr_value_percent < 50){
    digitalWrite(D2,LOW);
    digitalWrite(D4,HIGH);
    digitalWrite(D6,LOW);
    }
    else{
    digitalWrite(D2,HIGH);
    digitalWrite(D4,LOW);
    digitalWrite(D6,HIGH);
      }
    delay(50);
}
