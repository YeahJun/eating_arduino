int i = 0;

void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT_PULLUP);
}

void loop() {
  int value = digitalRead(8);
  if(value == LOW){
    Serial.print(i);
    Serial.println(" - push");
    i++;    
  }
}
