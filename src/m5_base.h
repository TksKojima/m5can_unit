
#ifndef M5_BASE_H
#define M5_BASE_H

#include <Arduino.h>
// #include <M5Stack.h>
#include <M5GFX.h>
#include <M5Unified.h>

#include <m5can_unit_app.h>


extern CanApp canUnit;
extern CanApp canComm;

void M5_setup();
int M5_loop_BtnA( int cntNum );
int M5_loop_BtnB( int cntNum );
int M5_loop_BtnC( int cntNum );
int M5_btn_loop();
void  M5_setCntBtnA(int setcnt);
void  M5_setCntBtnB(int setcnt);
void  M5_setCntBtnC(int setcnt);
int M5_getCntBtnA();
int M5_getCntBtnB();
int M5_getCntBtnC();
int M5_getCntPrevBtnA();
int M5_getCntPrevBtnB();
int M5_getCntPrevBtnC();

void M5_LCD_loop( M5Canvas canvas );

#endif