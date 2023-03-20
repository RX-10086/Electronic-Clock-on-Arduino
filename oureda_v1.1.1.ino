#include <LiquidCrystal.h>
#include "MsTimer2.h"
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5
unsigned int sec_0 = 0, min_0 = 0, hour_0 = 0;
char put[9] = { 48 };
char input[9];
void Timer() {
  if (sec_0 >= 60) {
    sec_0 = 0;
    min_0++;
  }
  if (min_0 >= 60) {
    min_0 = 0;
    hour_0++;
  }
  if (hour_0 >= 24) {
    hour_0 = 0;
  }
}
void Output(unsigned int num, char *put_e) {
  int i = num / 10;
  int j = num % 10;
  *put_e = (char)(i + 48);
  *(put_e + 1) = (char)(j + 48);
}
void Input(char *input_e) {
  unsigned int i = (int)(*input_e) - 48, j = (int)(*(input_e + 1)) - 48;
  hour_0 = i * 10 + j;
  unsigned int k = (int)(*(input_e + 3)) - 48, l = (int)(*(input_e + 4)) - 48;
  min_0 = k * 10 + l;
  unsigned int m = (int)(*(input_e + 6)) - 48, n = (int)(*(input_e + 7)) - 48;
  sec_0 = m * 10 + n;
}
int read_LCD_buttons() {
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50) return btnRIGHT;
  if (adc_key_in < 250) return btnUP;
  if (adc_key_in < 450) return btnDOWN;
  if (adc_key_in < 650) return btnLEFT;
  if (adc_key_in < 850) return btnSELECT;
  return btnNONE;
}
void Time_PLUS()
{
  sec_0 = sec_0 + 1;
}
void setup() {
  Serial.begin(19200);
  lcd.begin(16, 2);
  lcd.home();
  lcd.print("TIME:");
  put[2] = ':';
  put[5] = ':';
  put[8] = '\0';
  input[8] = '\0';
  lcd.setCursor(0, 1);
  Serial.println("ID Number:20222241285");
  Serial.println("Name:Yan Weijia");
  lcd.print("2022*****85 YWJ");
  MsTimer2::set(1000, Time_PLUS);
  MsTimer2::start();
}
void loop() 
{
  if (Serial.available()) 
  {
    Serial.readBytes(input, 9);
    Input(input);
  }  
  lcd_key = read_LCD_buttons();  
  switch (lcd_key)               
  {
    case btnRIGHT:
      {
        if (min_0 == 59) {
          min_0 = 0;
        } else {
          min_0++;
        }
        break;
      }
    case btnLEFT:
      {
        if (min_0 == 0) {
          min_0 = 59;
        } else {
          min_0--;
        }
        break;
      }
    case btnUP:
      {
        hour_0++;
        break;
      }
    case btnDOWN:
      {
        if (hour_0 == 0) {
          hour_0 = 23;
        } else {
          hour_0--;
        }
        break;
      }
    case btnSELECT:
      {
        sec_0 = 0;
        break;
      }
    case btnNONE:
      {
        break;
      }
  }
  Timer();
  Output(hour_0, put);
  Output(min_0, put + 3);
  Output(sec_0, put + 6);
  lcd.setCursor(8, 0);
  lcd.println(put);
  delay(500);
}
