#include <m5_base.h>


//TFT_ecanvas canvas(&M5.Lcd);



int btnA_cnt = 0;
int btnB_cnt = 0;
int btnC_cnt = 0;
int btnA_cntPrev = 0;
int btnB_cntPrev = 0;
int btnC_cntPrev = 0;

int show_LCD = 0;

void M5_setup(){
    M5.begin();
    M5.Power.begin();

    M5.Speaker.begin();
    M5.Speaker.setVolume(90);

    // sprite.setTextColor(GREEN);
    // sprite.setColorDepth(8);
    // sprite.setTextSize(1);
    // sprite.createSprite(M5.Lcd.width(), M5.Lcd.height());
    delay(5);

}


int M5_loop_BtnA( int cntNum ){
    int ret_flag = 0;
    static int prevBtnA = 0;

    btnA_cntPrev = btnA_cnt;
    if (M5.BtnA.wasReleased() && prevBtnA == 0 ) {
        btnA_cnt++;
        if( btnA_cnt >= cntNum ){ btnA_cnt = 0; }
        ret_flag = 1;
        M5.Speaker.tone(1000, 250); //

    }
    prevBtnA = M5.BtnA.wasReleased();
    return ret_flag;

}

int  M5_loop_BtnB( int cntNum ){
    int ret_flag = 0;
    static int prevBtnB = 0;

    btnB_cntPrev = btnB_cnt;
    if (M5.BtnB.wasReleased() && prevBtnB == 0 ) {
        btnB_cnt++;
        if( btnB_cnt >= cntNum ){ btnB_cnt = 0; }
        ret_flag = 1;
        M5.Speaker.tone(1300, 250); //
    }    
    prevBtnB = M5.BtnB.wasReleased();
    return ret_flag;
    
}

int  M5_loop_BtnC( int cntNum ){
    int ret_flag = 0;
    static int prevBtnC = 0;
    btnC_cntPrev = btnC_cnt;
    if (M5.BtnC.wasReleased() && prevBtnC == 0 ) {
        btnC_cnt++;
        if( btnC_cnt >= cntNum ){ btnC_cnt = 0; }
        ret_flag = 1;
        M5.Speaker.tone(1300, 250); // 
    }    
    prevBtnC = M5.BtnC.wasReleased();
    return ret_flag;
    
}

int M5_btn_loop(){
    int retflag = 0;
    retflag += M5_loop_BtnA(2);
    retflag += M5_loop_BtnB(2);
    retflag += M5_loop_BtnC(2);    

    return retflag;

}

void  M5_setCntBtnA(int setcnt){ btnA_cnt = setcnt; }
void  M5_setCntBtnB(int setcnt){ btnB_cnt = setcnt; }
void  M5_setCntBtnC(int setcnt){ btnC_cnt = setcnt; }
int M5_getCntBtnA(){ return btnA_cnt; }
int M5_getCntBtnB(){ return btnB_cnt; }
int M5_getCntBtnC(){ return btnC_cnt; }
int M5_getCntPrevBtnA(){ return btnA_cntPrev; }
int M5_getCntPrevBtnB(){ return btnB_cntPrev; }
int M5_getCntPrevBtnC(){ return btnC_cntPrev; }

void M5_LCD_loop( M5Canvas canvas ){

    M5.Display.startWrite();
    canvas.fillScreen(BLACK);
    canvas.setColor(GREEN);
    canvas.setCursor(0, 0);
    canvas.printf(" M5 LCD Show");
    canvas.printf("\n");
    canvas.printf("Btn A: %d", btnA_cnt);
    //canvas.printf("\n");
    canvas.printf("       ");
    canvas.printf("Btn B: %d", btnB_cnt);
    //canvas.printf("\n");
    canvas.printf("       ");
    canvas.printf("Btn C: %d", btnC_cnt);
    canvas.printf("\n");

    // canvas.printf("Can Tx Test Msg (press A btn): %d", can_tx_test);
    // if( can_tx_test != 0 ){  canvas.printf(" TestMsg sending");
    // }else{                   canvas.printf(" No TestMsg"); }
    // canvas.printf("\n");

    // canvas.printf("Can Show Mode   (press B btn): %d", can_show_mode);
    // if( can_show_mode == 0 ){ canvas.printf(" No show"); }
    // if( can_show_mode == 1 ){ canvas.printf(" show LCD");}
    // if( can_show_mode == 2 ){ canvas.printf(" show LCD & Serial"); }     
    // canvas.printf("\n");

    // canvas.printf("Wifi start (press C btn): %d", use_wifi);
    // if( use_wifi != 0 ){  canvas.printf(" Wifi On for CAN setting ");
    // }else{                   canvas.printf(" Wifi Off"); }
    // canvas.printf("\n");

    canComm.M5_CanShowLCD(&canvas);
    canUnit.M5_CanShowLCD(&canvas);

    canvas.pushSprite(&M5.Display,0,0);
    M5.Display.endWrite();
    
}




