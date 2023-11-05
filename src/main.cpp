
/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配�?  M5Core 示例源代�?
* Visit for more information: https://docs.m5stack.com/en/unit/can
* 获取更多�?料请访问: https://docs.m5stack.com/zh_CN/unit/can
*
* Describe: CA-IS3050G.  CAN transceiver unit
* Date: 2021/11/01
******************************************** ***********************************
  Please connect port B (26, 36), the device will automatically send and receive
messages through the CAN bus If there are phenomena such as failure of normal
communication, please check the communication rate or add a terminal resistance
between the H and L phases Please install library before compiling: ESP32CAN:
https://github.com/miwagner/ESP32-Arduino-CAN
*/
#include <Arduino.h>
// #include <M5Stack.h>
// #include <M5GFX.h>
#include <M5Unified.h>

#include <m5can_unit_app.h>
#include <m5_base.h>
#include <fifo.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/timer.h"
  

// M5GFX display;
// M5Canvas canvas(&display);

CanApp canUnit;
CanApp canComm;
M5Canvas canvas;

//CAN_device_t CAN_cfg;  // CAN Config

int can_tx_test = 1;
int can_tx_test2 = 1;
int can_show_mode = 2;
int test = 0;

  unsigned long ms_begin;
  unsigned long ms_end;

void canLoopTask(void *pvParameters)
{
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  while (1)
  {
    ms_begin = millis();

//    Serial.print("test ");
//    Serial.println(test);

    canUnit.loop();
    canComm.loop();

    canComm.setTestFlag( can_tx_test , can_show_mode );
    canUnit.setTestFlag( can_tx_test2 , can_show_mode );

    M5.update(); 
    can_tx_test   = M5_getCntBtnA();
    can_tx_test2   = M5_getCntBtnB();
    can_show_mode = M5_getCntBtnC();

    M5_loop_BtnA(2);
    M5_loop_BtnB(2);
    M5_loop_BtnC(3);    

    ms_end = millis();

    // このタスクは周期的に実行される
    vTaskDelayUntil(&xLastWakeTime, 10 / portTICK_PERIOD_MS);
  }
}

void otherLoopTask(void *pvParameters)
{

  while(1){

    canUnit.buf_send_show();
    canComm.buf_send_show();

    static int prebBtnC = -1;
    int LCDflag = 0;
    if( M5_getCntBtnC() == 2 || M5_getCntBtnC() != prebBtnC ){
        LCDflag = 1;
    }

    prebBtnC = M5_getCntBtnC();
    M5.Display.startWrite();
    M5_LCD_loop( canvas );
      canvas.pushSprite(&M5.Display,0,0);

    M5.Display.endWrite();
    vTaskDelay(1);
  }
}


void setup() {

  auto cfg = M5.config(); 
  M5.begin(cfg);
  Serial.println("Basic Demo - ESP32-Arduino-CAN");

  canvas.setColorDepth(8);  // mono color
  canvas.createSprite(M5.Display.width(), M5.Display.height());
  canvas.setTextSize(1);
  canvas.setTextColor(GREEN);
  //canvas.setPaletteColor(1, GREEN);
  canvas.setTextScroll(true);
  canvas.println("Init CAN Module.....");
  canvas.println("Waiting Frame");
  canvas.pushSprite(&M5.Display, 0, 0);

  M5.Display.startWrite();
  M5.Display.setTextSize(2);               // テキストサイズを変更
  M5.Display.setTextColor(GREEN);
  M5.Display.setCursor(0, 0);
  M5.Display.endWrite();

  M5_setup();
  canUnit.init( CanApp::HARD_CANBUS_UNIT );
  canComm.init( CanApp::HARD_COMM_MODULE );

  TaskHandle_t h_canLoopTask, h_otherLoopTask;
  xTaskCreatePinnedToCore(canLoopTask,   "CanLoopTask", 4096, NULL,  1, &h_canLoopTask, 0);
  xTaskCreatePinnedToCore(otherLoopTask, "OtherLoopTask",   4096, NULL,  0, &h_otherLoopTask, 1);

  vTaskDelay(500);    

}



void loop() {



    // unsigned long start_ms, end_ms;
    // static unsigned long diff_ms=0;
    // start_ms = millis();
    //M5.Display.startWrite();
    //   canvas.fillScreen(RGBColor(0x003000));
    //   canvas.setTextColor(WHITE);
    //   canvas.setCursor(0, 20);
    //   canvas.println( diff_ms + millis());
    //M5_LCD_loop( canvas );
    //  canvas.pushSprite(&M5.Display,0,0);
    //M5.Display.endWrite();
    // end_ms = millis();
    // diff_ms = end_ms- start_ms;

//   delay(1000);                              // 1秒待つ

    vTaskDelay(1);    

}
