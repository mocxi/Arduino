#include "DebugUtils.h"
//LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display
bool t1 = false;
bool t2 = false;
void DebugUtils_init()
{
  //lcd. (0x27,16,2);  // set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display
  Serial.begin(9600);
  delay(1000);

  lcd.init();
  t1 = t2 = false;
  // Print a message to the LCD.
  lcd.backlight();
  LCD_PRINT_1(4, "Mocxi");
  LCD_PRINT_2(3, "AI 2017");
  //lcd.setCursor(0,0); lcd.print("str");
}
