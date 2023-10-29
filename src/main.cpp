
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


M5GFX display;
M5Canvas canvas(&display);

CanApp canUnit;
CAN_device_t CAN_cfg;  // CAN Config

int can_tx_test = 1;
int can_show_mode = 2;

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
    canUnit.init( CAN_cfg );


}



void loop() {

    M5.update(); 

    can_tx_test   = M5_getCntBtnA();
    can_show_mode = M5_getCntBtnC();
    //can_setTestFlag( can_tx_test , can_show_mode );
    canUnit.setTestFlag( can_tx_test , can_show_mode );

    canUnit.loop();

    M5_loop_BtnA(2);
    M5_loop_BtnB(2);
    M5_loop_BtnC(3);
    

    static int prebBtnC = -1;
    int LCDflag = 0;
    if( M5_getCntBtnC() == 2 || M5_getCntBtnC() != prebBtnC ){
        LCDflag = 1;
    }
    M5_LCD_loop( LCDflag );

    prebBtnC = M5_getCntBtnC();

    delay(1);
}
