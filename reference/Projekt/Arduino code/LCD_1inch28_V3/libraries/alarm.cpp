


#include "alarm.h"
#include "pin_config.h"
#include "GUI_Paint.h"

void ALARM_LOUD(){
  //int i;
  //while(1){
  //  i = digitalRead(UBTN);
  //  if(i == 1){break;}
    
    LCD_SetBacklight(255);
    Paint_Clear(RED);
    Paint_DrawString_EN(80, 80, "ALARM", &Font24, RED, BLACK);

    digitalWrite(RESONATOR, 1);

    for(int j = 0; j < NumberOfBeeps; j++){
      analogWrite(BUZZER, 128);
      delay(LengthOfBeep);
      analogWrite(BUZZER, 0);
      delay(LengthOfBeep);
    }

    digitalWrite(RESONATOR, 0);

  //  delay(LengthOfSilence);
  //}
}

void ALARM_SILENT(){
  //int i;
  //while(1){
  //  i = digitalRead(UBTN);
  //  if(i == 1){break;}
    LCD_SetBacklight(255);
    Paint_Clear(RED);
    Paint_DrawString_EN(80, 80, "ALARM", &Font24, RED, BLACK);

    digitalWrite(RESONATOR, 1);
    delay(LengthOfSilence);
    digitalWrite(RESONATOR, 0);
  //  delay(LengthOfSilence);
  //}
}




