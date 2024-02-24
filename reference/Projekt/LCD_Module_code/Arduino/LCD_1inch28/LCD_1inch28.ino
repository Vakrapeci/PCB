#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

void setup()
{
  //LDC setup
  Config_Init();
  LCD_Init();
  LCD_SetBacklight(255);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);
  Paint_Clear(BLACK);
  Paint_DrawCircle(120,120, 120, BLUE ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
  Paint_DrawLine  (120, 0, 120, 12,GREEN ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
  Paint_DrawLine  (120, 228, 120, 240,GREEN ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
  Paint_DrawLine  (0, 120, 12, 120,GREEN ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
  Paint_DrawLine  (228, 120, 240, 120,GREEN ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
  Paint_DrawImage (gImage_70X70, 85, 25, 70, 70); 
  Paint_DrawString_EN(123, 123, "WAVESHARE",&Font16,  BLACK, GREEN);
  Paint_DrawLine  (120, 120, 70, 70,YELLOW ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
  Paint_DrawLine  (120, 120, 176, 64,BLUE ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
  Paint_DrawLine  (120, 120, 120, 210,RED ,DOT_PIXEL_2X2,LINE_STYLE_SOLID); 

  //pinmode setup
  pinMode(5, OUTPUT);
}
void loop()
{
  analogWrite(5, 128);
  delay(125);
  analogWrite(5, 0);
  delay(125);
  analogWrite(5, 128);
  delay(125);
  analogWrite(5, 0);
  delay(125);
  analogWrite(5, 128);
  delay(125);
  analogWrite(5, 0);
  delay(125);
  analogWrite(5, 128);
  delay(125);
  analogWrite(5, 0);
  delay(125);

  //Paint_DrawPoint(50, 50, RED, DOT_PIXEL_5X5, DOT_FILL_AROUND);
  analogWrite(5, 0);
  delay(500);
}



/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

