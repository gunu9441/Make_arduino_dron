void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);
}

void loop() {
  if(Serial.available()){
    char serialData = Serial.read();
    Serial1.write(serialData);}
  if (Serial1.available()){
    char blueData = Serial1.read();
    Serial.write(blueData);
  }
}
