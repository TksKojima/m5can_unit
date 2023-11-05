
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
#include <M5Stack.h>
#include <M5GFX.h>

#include <m5can_unit_app.h>
#include <m5_base.h>
#include <fifo.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/timer.h"

#define AHRS_SAMPLE_RATE 1000
#define AHRS_SAMPLE_MS (1000.0 / AHRS_SAMPLE_RATE)    

M5GFX display;
M5Canvas canvas(&display);

CanApp canUnit;
CanApp canComm;

//CAN_device_t CAN_cfg;  // CAN Config

int can_tx_test = 1;
int can_tx_test2 = 1;
int can_show_mode = 2;
int test = 0;

// TimerHandle_t thand_test;
// void timer_handler( void *param )
// {
//   //Serial.print("timer_handler()\n");
// //   canComm.buf_send();
// //   canComm.buf_recv();
// //   canUnit.buf_send();
// //   canUnit.buf_recv();
//     canUnit.loop();
//     canComm.loop();

// }
  unsigned long ms_begin;
  unsigned long ms_end;

void canLoopTask(void *pvParameters)
{
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  ms_begin = millis();

  while (1)
  {
    ms_begin = millis();

    // Serial.print("test ");
    // Serial.println(test);

    M5.update(); 
    can_tx_test   = M5_getCntBtnA();
    can_tx_test2   = M5_getCntBtnB();
    can_show_mode = M5_getCntBtnC();

    //can_setTestFlag( can_tx_test , can_show_mode );
    canComm.setTestFlag( can_tx_test , can_show_mode );
    canUnit.setTestFlag( can_tx_test2 , can_show_mode );

    M5_loop_BtnA(2);
    M5_loop_BtnB(2);
    M5_loop_BtnC(3);    


    canUnit.loop();
    canComm.loop();

    ms_end = millis();

    // このタスクは周期的に実行される
    vTaskDelayUntil(&xLastWakeTime, 10 / portTICK_PERIOD_MS);
  }
}

void otherLoopTask(void *pvParameters)
{
  while(1){
//    test++;

    Serial.print( "tictoc: ");
    Serial.println( ms_end - ms_begin);


    // canUnit.loop();
    // canComm.loop();
    canUnit.buf_send_show();
    canComm.buf_send_show();



    static int prebBtnC = -1;
    int LCDflag = 0;
    if( M5_getCntBtnC() == 2 || M5_getCntBtnC() != prebBtnC ){
        LCDflag = 1;
    }

    prebBtnC = M5_getCntBtnC();
    M5_LCD_loop( LCDflag );

    vTaskDelay(1);
  }
}


void setup() {
    M5.begin();
    Serial.println("Basic Demo - ESP32-Arduino-CAN");

    display.begin();

    canvas.setColorDepth(1);  // mono color
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize(1);
    canvas.setPaletteColor(1, GREEN);
    canvas.setTextScroll(true);
    canvas.println("Init CAN Module.....");
    canvas.println("Waiting Frame");
    canvas.pushSprite(0, 0);


    M5_setup();
    //canUnit.init( CAN_cfg );
    //canUnit.init( CanApp::HARD_CANBUS_UNIT );
    canUnit.init( CanApp::HARD_CANBUS_UNIT );
    canComm.init( CanApp::HARD_COMM_MODULE );

  // /* create timer */
  // thand_test = xTimerCreate( "TIM_TEST",  /* タイマの名称 */
  //                            1,        /* 遅延時間 */
  //                            pdTRUE,      /* 自動繰り返しの有無 */
  //                            NULL,        /* ID変数のポインタ */
  //                            timer_handler ); /* タイマハンドラの関数名 */

  // xTimerStart( thand_test, 0 ); /* タイマの開始 */
    TaskHandle_t h_canLoopTask, h_otherLoopTask;
    xTaskCreatePinnedToCore(canLoopTask,   "CanLoopTask", 4096, NULL,  1, &h_canLoopTask, 0);
    xTaskCreatePinnedToCore(otherLoopTask, "OtherLoopTask",   4096, NULL,  0, &h_otherLoopTask, 1);

  vTaskDelay(500);    

}



void loop() {

  vTaskDelay(500);    

}
