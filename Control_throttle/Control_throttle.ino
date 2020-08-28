void setup() {
  Serial.begin(115200);

}

void loop() {
  if(Serial.available()>0){
    char userInput = Serial.read();
    Serial.println(userInput);
  
    if(userInput >='0' && userInput <='9'){
      int throttle = (userInput - '0') * 10;
      analogWrite(6,throttle);
      analogWrite(10,throttle);
      analogWrite(9,throttle);
      analogWrite(5,throttle);
      }
  }
}
