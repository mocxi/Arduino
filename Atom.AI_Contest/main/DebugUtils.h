/*
* Utility functions to help debugging running code.
*/

#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#define SIMPLE_DEBUG_LOG

#include "Define.h"

#include <LiquidCrystal_I2C.h>
// set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display
static LiquidCrystal_I2C lcd(0x3F,16,2);
extern bool t1,t2;

#include <Arduino.h>
	#ifdef DEBUG_BUILD
		#ifdef SIMPLE_DEBUG_LOG
			#define DEBUG_PRINT(str) //Serial.print(str)
			#define DEBUG_PRINTLN(str)// Serial.println(str)

		#if DEBUG_LEVEL_INFO
			#define DBG(str) Serial.print(str)
			#define DBG_LN(str) Serial.println(str)
		#else
			#define DBG(str)
			#define DBG_LN(str)
		#endif

			#define LCD_PRINT_1(x,str) lcd.setCursor(0,0); lcd.print("                ");\
				lcd.setCursor(x,0); lcd.print(str); lcd.setCursor(15,0);lcd.print(t1); t1 = !t1;

			#define LCD_PRINT_2(x,str) lcd.setCursor(0,1); lcd.print("                ");\
				lcd.setCursor(x,1); lcd.print(str); lcd.setCursor(15,1);lcd.print(t2); t2 = !t2;
		#else //SIMPLE_DEBUG_LOG
			#define DBG(str)
			#define DBG_LN(str)

			#define DEBUG_PRINT(str)
			//I have no idea how to write this
			#define DEBUG_PRINTLN(str) \
				Serial.print(millis()); \
				Serial.print(": "); \
				Serial.print(__FUNCTION__); \
				Serial.print("() in "); \
				Serial.print(__FILE__); \
				Serial.print(':'); \
				Serial.print(__LINE__); \
				Serial.print('    '); \
				Serial.println(str);

				#define LCD_PRINT_1(x,str) //lcd.setCursor(0,0); lcd.print("BBBBBB");
				#define LCD_PRINT_2(x,str)
		#endif //SIMPLE_DEBUG_LOG
	#else //DEBUG_BUILD
		#define DEBUG_PRINT(str)
		#define DEBUG_PRINTLN(str)

		#define DBG(str)
		#define DBG_LN(str)

		#define LCD_PRINT_1(x,str) // lcd.setCursor(0,0); lcd.print("AAAAAA");
		#define LCD_PRINT_2(x,str)
	#endif //DEBUG_BUILD

extern void DebugUtils_init();



#endif
