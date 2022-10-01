// states 
#define LIGHT_OFF 0
#define LIGHT_ON 1


// delay 
#define STATE_CHANGE_DELAY 1000

#define LED_PIN 5

int ldr_value = 0;
int state = LIGHT_OFF;
int shadow_theshold = 200;
bool is_on_sensor = false;


void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  Serial.print("start");
}

void loop() {
  ldr_value = analogRead(A0);
  Serial.print("value: ");
  Serial.print(ldr_value);
  Serial.print("\n");
  if (ldr_value < shadow_theshold){
    if(state == LIGHT_OFF && !is_on_sensor ){
      state = LIGHT_ON;
      is_on_sensor = true;
      digitalWrite(LED_PIN,HIGH);
    }
    else if(!is_on_sensor){
      state = LIGHT_OFF;
      is_on_sensor = true;
      digitalWrite(LED_PIN,LOW);
    }
   delay(STATE_CHANGE_DELAY);
  }else{
    is_on_sensor = false;
  }
    
}
