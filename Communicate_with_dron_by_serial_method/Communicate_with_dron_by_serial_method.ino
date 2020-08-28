void setup() {
  Serial.begin(11520);
  pinMode(5,OUTPUT);
}

void loop() {
  if(Serial.available()>0){
    char value = Serial.read();
    Serial.write(value);
    if(value == '1'){
      digitalWrite(5,1);
      Serial.println(" : LED ON");
    }
    else if (value == '0'){
      digitalWrite(5,0);
      Serial.println(" : LED OFF");
    }
  }

}
