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

      if(cnt_msg == 4){
        Serial.print("Type = ");Serial.print(msp_data);
      }
       else if ( cnt_msg == 5){
         Serial.print(" | R = "); Serial.print(msp_data);
      }
      else if (cnt_msg == 6){
        Serial.print(" | P ="); Serial.print(msp_data);
      }
      else if (cnt_msg == 7){
        Serial.print(" | Y ="); Serial.print(msp_data);
      }
      else if (cnt_msg == 8){
        Serial.print(" | T ="); Serial.print(msp_data);
      }
      else if (cnt_msg == 9){
        Serial.print(" | Aux ="); Serial.print(msp_data);
      }
      else if (cnt_msg == 10){
        Serial.print(" | ctc ="); Serial.print(msp_data);
        Serial.println();
      }
    }
  }

}
