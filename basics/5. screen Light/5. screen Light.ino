#define D2 4

int ldr_value = 0;
int light_intensity = 0;

void setup() {
  Serial.begin(9600);
  pinMode(D2, OUTPUT);
}

void loop() {
  ldr_value = analogRead(A0);
  light_intensity = 255 - map(ldr_value, 0, 1023, 0, 255);
  analogWrite(D2, light_intensity);
  delay(750);
}
