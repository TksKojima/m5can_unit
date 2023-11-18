
/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配�?  M5Core 示例源代?��?
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

int can_tx_test = 0;
int can_tx_test2 = 0;
int disp_show_mode = 0;
int test = 0;

unsigned long ms_begin;
unsigned long ms_end;

int LCDflag = 1;

unsigned long last_disp_ms = 0;;
int can_cnt = 0;
unsigned long prevmillis=0;


void main_disp_show(){
    last_disp_ms = millis();
    canUnit.buf_send_show();
    canComm.buf_send_show();
    canUnit.buf_recv_show();
    canComm.buf_recv_show();

    if( LCDflag ){
      LCDflag = 0;
      M5_LCD_loop( canvas );
    }
}

void canLoopTask2(void *pvParameters)
{
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  while (1)
  {
    canComm.loop_send();
    //canUnit.loop_send();

    int taskwait = 200;

    // このタスクは周期的に実行される
    vTaskDelayUntil(&xLastWakeTime, taskwait / portTICK_PERIOD_MS);
  }

}

void canLoopTask(void *pvParameters)
{
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  while (1){


    // このタスクは周期的に実行される
    // int taskwait = 10;
    // vTaskDelayUntil(&xLastWakeTime, taskwait / portTICK_PERIOD_MS);
  }
}

void otherLoopTask(void *pvParameters)
{
  while(1){
    main_disp_show();
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
  M5.Display.setTextSize(2);               // �?キストサイズを変更
  M5.Display.setTextColor(GREEN);
  M5.Display.setCursor(0, 0);
  M5.Display.endWrite();

  M5_setup();
  canUnit.init( CanApp::HARD_CANBUS_UNIT );
  canComm.init( CanApp::HARD_COMM_MODULE );

  TaskHandle_t h_canLoopTask, h_canLoopTask2, h_otherLoopTask;
  // xTaskCreatePinnedToCore(canLoopTask2,   "CanLoopTask2", 4096, NULL,  1, &h_canLoopTask2, 1);
  // xTaskCreatePinnedToCore(canLoopTask,   "CanLoopTask", 4096, NULL,  2, &h_canLoopTask, 0);
  // //xTaskCreatePinnedToCore(otherLoopTask, "OtherLoopTask",   4096, NULL,  0, &h_otherLoopTask, 1);

  vTaskDelay(500);    

}



void loop() {

  static unsigned long txwait_millis = 0;
  if(  millis() - prevmillis >= 5 ){

    canComm.loop_recv();
    //canUnit.loop_recv();
    can_cnt++;

    canComm.setTestFlag( can_tx_test , disp_show_mode );
    canUnit.setTestFlag( can_tx_test2 , disp_show_mode );

    M5.update(); 
    M5_loop_BtnA(2);
    M5_loop_BtnB(2);
    M5_loop_BtnC(3); 

    //ここではAとBは排他
    if( M5_getCntBtnA() == 1 && M5_getCntPrevBtnA()==0 ){
      M5_setCntBtnB(0);
    }
    else if( M5_getCntBtnB() == 1 && M5_getCntPrevBtnB()==0 ){
      M5_setCntBtnA(0);
    }

    can_tx_test   = M5_getCntBtnA();
    can_tx_test2   = M5_getCntBtnB();
    disp_show_mode = M5_getCntBtnC();


    if( M5_getCntBtnA() != M5_getCntPrevBtnA() ||
        M5_getCntBtnB() != M5_getCntPrevBtnB() || 
        M5_getCntBtnC() != M5_getCntPrevBtnC()){
        LCDflag = 1;
        txwait_millis = millis();
    }
    if( disp_show_mode == 2 ){
        LCDflag = 1;
    }

    if( millis() - txwait_millis > 500 ){
      canComm.loop_send();
      //canUnit.loop_send();
    }


    int taskwait = 10;
    //if( LCDflag ) taskwait = 100;

    if( LCDflag && millis() - last_disp_ms > 100 && can_cnt > 20 ){
      can_cnt = 0;
      main_disp_show();
    }

//  }
//    vTaskDelay(1);    
   prevmillis = millis();

  }

}
