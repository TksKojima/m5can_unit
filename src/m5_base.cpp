#include <m5_base.h>


TFT_eSprite sprite(&M5.Lcd);



int btnA_cnt = 0;
int btnB_cnt = 0;
int btnC_cnt = 0;

int show_LCD = 0;

void M5_setup(){
    M5.begin();
    M5.Power.begin();

    M5.Speaker.begin();
    M5.Speaker.setVolume(4);

    sprite.setTextColor(GREEN);
    sprite.setColorDepth(8);
    sprite.setTextSize(1);
    sprite.createSprite(M5.Lcd.width(), M5.Lcd.height());
    delay(5);

}


int M5_loop_BtnA( int cntNum ){
    int ret_flag = 0;
    static int prevBtnA = 0;

    if (M5.BtnA.isPressed() && prevBtnA == 0 ) {
        btnA_cnt++;
        if( btnA_cnt >= cntNum ){ btnA_cnt = 0; }
        ret_flag = 1;
        M5.Speaker.tone(1000, 250); // é¸îgêî1000 HzÇ≈0.5ïbä‘âπÇ???ê∂

    }
    prevBtnA = M5.BtnA.isPressed();
    return ret_flag;

}

int  M5_loop_BtnB( int cntNum ){
    int ret_flag = 0;
    static int prevBtnB = 0;

    if (M5.BtnB.isPressed() && prevBtnB == 0 ) {
        btnB_cnt++;
        if( btnB_cnt >= cntNum ){ btnB_cnt = 0; }
        ret_flag = 1;
        M5.Speaker.tone(1300, 250); // é¸îgêî1000 HzÇ≈0.5ïbä‘âπÇ???ê∂
    }    
    prevBtnB = M5.BtnB.isPressed();
    return ret_flag;
    
}

int  M5_loop_BtnC( int cntNum ){
    int ret_flag = 0;
    static int prevBtnC = 0;
    if (M5.BtnC.isPressed() && prevBtnC == 0 ) {

        btnC_cnt++;
        if( btnC_cnt >= cntNum ){ btnC_cnt = 0; }
        ret_flag = 1;
        M5.Speaker.tone(1300, 250); // é¸îgêî1000 HzÇ≈0.5ïbä‘âπÇ???ê∂
    }    
    prevBtnC = M5.BtnC.isPressed();
    return ret_flag;
    
}

int M5_btn_loop(){
    int retflag = 0;
    retflag += M5_loop_BtnA(2);
    retflag += M5_loop_BtnB(2);
    retflag += M5_loop_BtnC(2);    

    return retflag;

}

int M5_getCntBtnA(){ return btnA_cnt; }
int M5_getCntBtnB(){ return btnB_cnt; }
int M5_getCntBtnC(){ return btnC_cnt; }

void M5_LCD_loop( int showLCD ){
    if( showLCD == 0){
        return ;
    }

    sprite.fillScreen(BLACK);
    sprite.setCursor(0, 0);

    sprite.printf(" M5 LCD Show");
    sprite.printf("\n");

    sprite.printf("Btn A: %d", btnA_cnt);
    sprite.printf("\n");
    sprite.printf("Btn B: %d", btnB_cnt);
    sprite.printf("\n");
    sprite.printf("Btn C: %d", btnC_cnt);
    sprite.printf("\n");


    // sprite.printf("Can Tx Test Msg (press A btn): %d", can_tx_test);
    // if( can_tx_test != 0 ){  sprite.printf(" TestMsg sending");
    // }else{                   sprite.printf(" No TestMsg"); }
    // sprite.printf("\n");

    // sprite.printf("Can Show Mode   (press B btn): %d", can_show_mode);
    // if( can_show_mode == 0 ){ sprite.printf(" No show"); }
    // if( can_show_mode == 1 ){ sprite.printf(" show LCD");}
    // if( can_show_mode == 2 ){ sprite.printf(" show LCD & Serial"); }     
    // sprite.printf("\n");

    // sprite.printf("Wifi start (press C btn): %d", use_wifi);
    // if( use_wifi != 0 ){  sprite.printf(" Wifi On for CAN setting ");
    // }else{                   sprite.printf(" Wifi Off"); }
    // sprite.printf("\n");

    canUnit.M5_CanShowLCD(&sprite);
    canComm.M5_CanShowLCD(&sprite);
    // main_sensorShow(  &sprite  );

    sprite.pushSprite(0, 0); 
    
}




