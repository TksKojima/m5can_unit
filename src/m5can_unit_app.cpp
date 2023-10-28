/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配??  M5Core 示例源代??
* Visit for more information: https://docs.m5stack.com/en/unit/can
* ?取更多??料???: https://docs.m5stack.com/zh_CN/unit/can
*
* Describe: CA-IS3050G.  CAN transceiver unit
* Date: 2021/11/01
******************************************** ***********************************
  Please connect port B (26, 36), the device will automatically send and receive
messages through the CAN bus If there are phenomena such as failure of normal
communication, please check the communication rate or add a terminal resistance
between the H and L phases Please install library before compiling: ESP32CAN:
https://github.com/miwagner/ESP32-Arduino-CAN

M5Stack Basci v2.6 Grove 
    CAN_cfg.tx_pin_id = GPIO_NUM_21;
    CAN_cfg.rx_pin_id = GPIO_NUM_22;

*/
#include <Arduino.h>
#include <m5can_unit_app.h>

const int rx_queue_size = 10;  // Receive Queue size
CAN_device_t CAN_cfg;  // CAN Config


// for App
// Lite版はcanbufの配列数を0x800にせず、1000以下ぐらいで運用
const int bufNum = CANBUF_SIZE; 
canRxBuffer canbuf[bufNum];

short id2idx_arr[ 0x800 ];
int now_idx = 0;

int tx_test_flag = 0;
int show_flag = 0;

int countInterval = 1000;
int countMax = 5;


void canUnit_init(){
    CAN_cfg.speed     = CAN_SPEED_1000KBPS; // if use 500kbps , set 1000KBPS BUG
    CAN_cfg.tx_pin_id = GPIO_NUM_21;
    CAN_cfg.rx_pin_id = GPIO_NUM_22;
    CAN_cfg.rx_queue  = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
    // Init CAN Module
    ESP32Can.CANInit();
    
}

void can_init() {

    canUnit_init();
    canbuf_init();

}

//  tx_test_flagが１だと、テスト用の適当なCANメッセージを送信
//  show_flag = _show_flag;が１だと、LCDとESP32のシリアルで受信データを表示
void can_setTestFlag( int txtest, int _show_flag ){ //loopの前ぐらいに置く
  tx_test_flag = txtest;
  // rx_test_flag = rxtest;
  show_flag = _show_flag;

}


void can_loop(){

  if( tx_test_flag ){
    canTxbuf_set_test();
  }

  canbuf_send();
  canbuf_recv();
  
}


void canbuf_init(){
  for( int i=0; i<bufNum; i++){
    canbuf[i].id = -1;

    canbuf[i].dlc = 1;
    canbuf[i].txrxFlag = 0;    
    canbuf[i].cycleTime = 1500;
    canbuf[i].data.u2[0] = 0;
    canbuf[i].data.u2[1] = 0;
    canbuf[i].data.u2[2] = 0;
    canbuf[i].data.u2[3] = 0;
    
    canbuf[i].prevTime = millis();
  }

  for( int i=0; i<0x800; i++ ){
    id2idx_arr[i] = -1;

  }
}

int id2idx( int id ){
  if( id2idx_arr[id] == -1 ){
    id2idx_arr[id] = now_idx;
    now_idx++;

    if( now_idx >= bufNum ){
      now_idx = bufNum - 1;
    }

  }

  return id2idx_arr[id];

}

void canTxbuf_set_test(){
  unsigned char  testdata[8];
  // testdata[0] = 192;
  // testdata[1] = 168;
  // testdata[2] = 10;
  // testdata[3] = 10;
  // testdata[4] = 0;
  // testdata[5] = 4;
  // testdata[6] = 0;
  // testdata[7] = 0;
  // canTxbuf_set( 0x732, 8, 732, testdata, 1);

  for( int i=1; i<=10; i++){
    canbuf[i].dlc = i%8 + 1;
    canbuf[i].txrxFlag = 1;
    canbuf[i].cycleTime = canbuf[i].dlc * 100;
    if( i<5 ) {
        canbuf[i].cycleTime = canbuf[i].dlc  * 10;
    }
    canbuf[i].data.u2[0] = 0x1234;
    canbuf[i].data.u2[1] = 0x5678;
    canbuf[i].data.u2[2] = 0x9abc ;
    canbuf[i].data.u2[3] = 0;
    canTxbuf_set( i, canbuf[i].dlc, canbuf[i].cycleTime, canbuf[i].data.u1, 1 );
  }  
}

void canTxbuf_set( int id, char dlc, int cycle, unsigned char *data, int txflag ){
  int tx_idx = id2idx( id );

  canbuf[tx_idx].id = id;  
  canbuf[tx_idx].dlc = dlc;  
  canbuf[tx_idx].cycleTime = cycle;
  for( int n=0; n<dlc; n++){
    canbuf[tx_idx].data.u1[n] = data[n];
  }
  if( txflag == 1 ){
    canbuf[tx_idx].txrxFlag = canTxRxFlag::TX;
  }else{
    canbuf[tx_idx].txrxFlag = 0;
  }
  //canbuf[id].prevTime = millis();
  canbuf[tx_idx].noChange.txCnt[0] = millis();

  //Serial.println("can seted ");

}


void canbuf_sendSingle( int idx ){

  // for M5 comm MCP
  //byte sndStat = CAN0.sendMsgBuf(0x100, 0, 8, data);
  //byte sndStat = CAN0.sendMsgBuf( canbuf[idx].id, 0, canbuf[idx].dlc, canbuf[idx].data.byteData );
  CAN_frame_t tx_frame;
  tx_frame.FIR.B.FF   = CAN_frame_std;
  tx_frame.MsgID      = canbuf[idx].id;
  tx_frame.FIR.B.DLC  = canbuf[idx].dlc;
  for( int i=0; i<canbuf[idx].dlc; i++ ){
    tx_frame.data.u8[i] = canbuf[idx].data.u1[i];
  }
    
  ESP32Can.CANWriteFrame(&tx_frame);

  // Serial.print("canid: ");
  // Serial.print(id); 
  // Serial.print(" data");
  // Serial.print(" ");
  // Serial.print(canbuf[id].data.u1[i]);  

  if( show_flag == 2 ){
    Serial.printf("canid: %d  cycle: %d  dlc: %d", canbuf[idx].id, canbuf[idx].cycleTime, (int)(canbuf[idx].dlc));
    for( int n=0; n<canbuf[idx].dlc; n++){
      Serial.printf(" %d", canbuf[idx].data.u1[n]);

    } 
    Serial.printf(" time: %d", canbuf[idx].prevTime);
    Serial.println();
  }
}

void canbuf_send(){
  for( int i=0; i<bufNum; i++){
    if( canbuf[i].txrxFlag == canTxRxFlag::TX){

      if( millis() - canbuf[i].noChange.txCnt[0]  >= canbuf[i].cycleTime + 2000 ){ //サイクルタイム＋閾値ミリ秒の間、setされなかったら途絶判定 
        canbuf[i].txrxFlag = canTxRxFlag::NON;
        continue;
      }


      if( millis() - canbuf[i].prevTime >= canbuf[i].cycleTime ){
        //Serial.print("before send");
        canbuf_sendSingle(i);
        //Serial.print("after send");
        canbuf[i].prevTime = millis();

        //canbuf[i].data.u2[3]++; 
        // Serial.print("send id: ");
        // Serial.println(i);
      }
    }
  }   
}

void canbuf_recv() {

  long unsigned int rxId;
  unsigned char len = 0;
  unsigned char rxBuf[8];
  char msgString[128];

  int rx_id = -1;
  int rx_dlc = -1;

  CAN_frame_t rx_frame;

  //if (!digitalRead(CAN0_INT))  // If CAN0_INT pin is low, read receive buffer
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE)
  {
      //CAN0.readMsgBuf( &rxId, &len, rxBuf);  // Read data: len = data length, buf = data byte(s)

      rxId = rx_frame.MsgID;
      len = rx_frame.FIR.B.DLC;
      for( int i=0; i<len; i++ ){
        rxBuf[i] = rx_frame.data.u8[i];

      }
      
      if ((rxId & 0x80000000) == 0x80000000)  // Determine if ID is standard (11 bits) or extended // (29 bits)
          sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
      else
          sprintf(msgString, "Standard ID: 0x%.3lX  DLC: %1d  Data:", rxId, len);

      // if( rx_test_flag == 2 ){
      //       Serial.print(msgString);
      // }

      if ((rxId & 0x40000000) == 0x40000000) {  // Determine if message is a remote request frame.
          sprintf(msgString, " REMOTE REQUEST FRAME");
      } 
      else {
        rx_id  = rxId;
        rx_dlc = (int)len;
        int rx_idx = id2idx( rx_id );
        canbuf[rx_idx].id  = rx_id;
        canbuf[rx_idx].dlc = rx_dlc;
        canbuf[rx_idx].cycleTime = millis() - canbuf[rx_id].prevTime;
        canbuf[rx_idx].prevTime  = millis();
        canbuf[rx_idx].txrxFlag = canTxRxFlag::RX;

        if( show_flag == 2 ){
          Serial.print("packet with id 0x");
          Serial.print(rx_id, HEX);    //Serial.print(" and length ");
          //Serial.println(packetSize);
          Serial.print(" dlc: ");
          Serial.print( rx_dlc );
          Serial.print(" size: ");

        }

          for (byte idx = 0; idx < len; idx++) {
            if( canbuf[rx_idx].data.u1[idx] != rxBuf[idx]){
              canbuf[rx_idx].noChange.rxCnt[idx] = 0;
            }

            canbuf[rx_idx].data.u1[idx] = rxBuf[idx];
            canbuf[rx_idx].noRecvCnt[idx] = 0;

            if( show_flag == 2 ){
              Serial.print(" ");
              Serial.print( rxBuf[idx], HEX );
              Serial.print(", ");
            }            

            sprintf(msgString, " 0x%.2X", rxBuf[idx]);

        }      
      }

      if( show_flag == 2 ){
        Serial.print(msgString);
        Serial.println();
        
      }      
  }
}

void recvDataTimeCount(){
  static unsigned long prevtime=millis();
  if( millis() - prevtime > countInterval ){
    for( int i=0; i<bufNum; i++){
      if( canbuf[i].txrxFlag == canTxRxFlag::RX ){
        for( int k=0; k<canbuf[i].dlc; k++){
          canbuf[i].noChange.rxCnt[k] += ( canbuf[i].noChange.rxCnt[k] >= countMax ? 0 : 1 );
          canbuf[i].noRecvCnt[k] += ( canbuf[i].noRecvCnt[k] >= countMax ? 0 : 1 );

        }
      }
    }
    prevtime =  millis();
  }
}

void printRecv(){
   Serial.println("printRecv");
//    for( int i=0; i<0x800; i++){
    for( int i=0; i<bufNum; i++){
      if( canbuf[i].txrxFlag == canTxRxFlag::RX ){
        canbuf[i].txrxFlag =  canTxRxFlag::NON;
        Serial.print(" ID: ");
        Serial.print(i, HEX);
        Serial.print(" cycl: ");
        Serial.print(canbuf[i].cycleTime);

        Serial.print(" data: ");
        for( int k=0; k<canbuf[i].dlc; k++){
          Serial.print(canbuf[i].data.u1[k], HEX);
          Serial.print(", ");

        }
        Serial.println("");
      }
    }

}



void M5_CanShowLCD( TFT_eSprite* sprite ){
  if( show_flag == 0 ){
    return;
  }
  (*sprite).setTextSize(1);

  (*sprite).printf("   :ID :DLC :Cycle : Data\n" );
  for( int i=0; i<CANBUF_SIZE; i++ ){
    if( canbuf[i].txrxFlag == canTxRxFlag::TX ||  canbuf[i].txrxFlag == canTxRxFlag::RX ){
      if( canbuf[i].txrxFlag == canTxRxFlag::TX ){ (*sprite).printf("Tx "); }
      if( canbuf[i].txrxFlag == canTxRxFlag::RX ){ (*sprite).printf("Rx "); } 
      //(*sprite).printf("i:%3X L:%d T:%5d D: ", canbuf[i].id, canbuf[i].dlc, canbuf[i].cycleTime );
      (*sprite).printf(":%3X :%d :%5d : ", canbuf[i].id, canbuf[i].dlc, canbuf[i].cycleTime );
      for( int j=0; j<canbuf[i].dlc; j++ ){
        (*sprite).printf("%2X ", canbuf[i].data.u1[j] );
      }
      (*sprite).printf("\n");
    }
  }

}



