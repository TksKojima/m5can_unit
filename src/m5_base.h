
#ifndef M5_BASE_H
#define M5_BASE_H

#include <Arduino.h>
#include <M5Stack.h>
#include <M5GFX.h>

void M5_setup();
int M5_loop_BtnA( int cntNum );
int M5_loop_BtnB( int cntNum );
int M5_loop_BtnC( int cntNum );
int M5_btn_loop();
int M5_getCntBtnA();
int M5_getCntBtnB();
int M5_getCntBtnC();

void M5_LCD_loop( int showLCD );

#endif