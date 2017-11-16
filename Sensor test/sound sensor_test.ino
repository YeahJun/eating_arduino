int Sound_Sensor = A0;
int Sensor_val;
int count_clap;

void setup() {
  Serial.begin(9600);
  }

void loop() {
  //normal value of 'Sensor val' = 200~380
  Sensor_val = analogRead(Sound_Sensor);
  
  Serial.println(Sensor_val);
  
if(Sensor_val > 0 && Sensor_val < 60) {
    count_clap++;
    Serial.print("Clappppp");
    delay(500);
    }

  if(Sensor_val>=60 && Sensor_val < 250 ) {
    count_clap++;
    Serial.print("qqqqqqpppp");
    Serial.print(count_clap);
    Serial.println("번");
    delay(500);
    }

  if(Sensor_val > 251)
   {
    count_clap++;
    Serial.print("qwerqwerqwerqwerqwer");
    Serial.print(count_clap);
    Serial.println("번");
    delay(500);
    }
}
