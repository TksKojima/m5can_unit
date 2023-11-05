// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef M5CAN_UNIT_APP_H
#define M5CAN_UNIT_APP_H

#include <Arduino.h>
#include <M5Stack.h>
#include <CAN_config.h>
#include <ESP32CAN.h>
//#include <can_bus_unit.h>
#include <M5Stack.h>
#include <M5GFX.h>
#include <mcp_can.h>

#include <fifo.h>


//#include "ArduinoJson-v6.19.4.h"
//#include "wifi_Server.h"


// typedef struct canApp{
// }canApp;

typedef struct canTxRxFlag
{
    static const int NON = 0;
    static const int TX = 1;
    static const int RX = 2;

}canTxRxFlag;

//#define CANBUF_SIZE (100)

typedef struct canRxBuffer
{
    char  txrxFlag;
    char  dlc;
    short id;  
    unsigned long prevTime;
    short cycleTime;  
    //unsigned int setTimeCnt;

	union {
        byte   byteData[8]; // for mcp_can 
		unsigned char  u1[8];   /**< \brief Payload byte access*/
		unsigned short u2[4]; /**< \brief Payload u32 access*/
		unsigned long  u4[2];    /**< \brief Payload u64 access*/
	} data;

	union {
        unsigned char rxCnt[8];
        unsigned long txCnt[2];

    }noChange;

    unsigned char noRecvCnt[8];


}canRxBuffer;

typedef struct canTxBuffer
{
    char  txrxFlag;
    char  dlc;
    short id;  
    unsigned long prevTime;
    unsigned int setTimeCnt;
    short cycleTime;  

	union {
		unsigned char  u1[8];   /**< \brief Payload byte access*/
		unsigned short u2[4]; /**< \brief Payload u32 access*/
		unsigned long  u4[2];    /**< \brief Payload u64 access*/
	} data;


}canTxBuffer;


//canBuffer canbuf[0x800];


typedef struct CanApp
{
    // for App
    // Lite”Å‚Ícanbuf‚Ì”z—ñ”‚ð0x800‚É‚¹‚¸A1000ˆÈ‰º‚®‚ç‚¢‚Å‰^—p
    //const int bufNum = 200; 
    static const int bufNum = 200;
    canRxBuffer canbuf[bufNum];

    int rx_queue_size = 10;  // Receive Queue size
    
    short id2idx_arr[ 0x800 ];
    int now_idx = 0;

    int tx_test_flag = 0;
    int show_flag = 0;

    int countInterval = 1000;
    int countMax = 5;

    static const int HARD_COMM_MODULE = 0;
    static const int HARD_CANBUS_UNIT = 1;

    int hardware = 1;

    FIFO<int> fifo_send;
    FIFO<int> fifo_recv;

    void init( int hard );
    void init( CAN_device_t _CAN_cfg );
    void Unit_init();
    void Comm_init();

    void setTestFlag( int txtest, int show );
    void loop();
    void buf_init();
    void Txbuf_set( int id, char dlc, int cycle, unsigned char *data, int txflag  );
    void Txbuf_set_test( );
    void buf_sendSingle( int id );
    void buf_send_show();
    void buf_recv_show();
    void buf_send();
    void buf_recv();
    //void onReceive(int packetSize);
    void printRecv();
    void recvDataTimeCount();
    int id2idx( int id );
    void M5_CanShowLCD( TFT_eSprite* sprite );
    
}CanApp;

// void can_init();
// void can_setTestFlag( int txtest, int show );
// void can_loop();
// void canbuf_init();
// void canTxbuf_set( int id, char dlc, int cycle, unsigned char *data, int txflag  );
// void canTxbuf_set_test( );
// void canbuf_sendSingle( int id );
// void canbuf_send();
// void canbuf_recv();
// //void onReceive(int packetSize);
// void printRecv();

// int id2idx( int id );

//void M5_CanShowLCD( TFT_eSprite* sprite );

#endif
