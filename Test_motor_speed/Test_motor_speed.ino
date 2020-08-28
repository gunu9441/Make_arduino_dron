void setup() {
  Serial1.begin(115200);  
  Serial.begin(115200);
}

void loop() {
  static uint8_t cnt_msg;
  if(Serial1.available()>0){
    while(Serial1.available()>0){
      uint8_t msp_data = Serial1.read();
      if(msp_data == '$') cnt_msg = 0;
      else cnt_msg++;

   
      if (cnt_msg == 8){
        Serial.print(" throttle ="); 
        Serial.print(msp_data);
        Serial.println();
        uint8_t throttle = msp_data;
        analogWrite(6,0);
        analogWrite(10,throttle);
        analogWrite(9,0);
        analogWrite(5,throttle);
      }
    }
  }
}
