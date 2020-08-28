void setup() {
  Serial1.begin(115200);
  Serial.begin(115200);
}

void loop() {
  if(Serial1.available()>0){
    char msp_data = Serial1.read();

    Serial.print((char)msp_data);
    Serial.println();
  }
}
