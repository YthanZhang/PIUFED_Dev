//
// Created by YthanZhang on 2021/12/30.
//


#include "piu_sim_uart.h"

#include "catch2/catch_all.hpp"

#include <iostream>


bool tx = true;

uint16_t rxVal = 0b1111110000;
uint16_t txVal = 0b1111110000;


void setTxBit(bool txVal) { tx = txVal; }


piu_SimUART simUART;




TEST_CASE("Sim UART Test", "[sim_uart]")
{
    piu_SimUART_construct(&simUART, setTxBit);
    
    /** Test Tx ***************************************************************/
    piu_SimUART_sendTx(&simUART, txVal & 0xFF);
    REQUIRE(tx);
    
    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE_FALSE(tx);

    while(piu_SimUART_TIMUpdate(&simUART, true))
    {
        REQUIRE(tx == (txVal & 0x01));
        txVal = txVal >> 1;
    }
    txVal = 0b1110010011;
    REQUIRE(tx);

    /** Test Rx ***************************************************************/
    piu_SimUART_GPIOUpdate(&simUART, false);
    
    while(!simUART.flag_rxComplete)
    {
        piu_SimUART_TIMUpdate(&simUART, rxVal & 0x01);
        rxVal = rxVal >> 1;
    }
    
    REQUIRE(piu_SimUART_getRx(&simUART) == 0xF0);
    
    /** Test simultaneous Rx Tx ***********************************************/
    piu_SimUART_sendTx(&simUART, txVal & 0xFF);
    REQUIRE(tx);
    
    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE_FALSE(tx);

    while(piu_SimUART_TIMUpdate(&simUART, true))
    {
        REQUIRE(tx == (txVal & 0x01));
        txVal = txVal >> 1;
    }
    txVal = 0b1110010011;
    REQUIRE(tx);
    
    piu_SimUART_GPIOUpdate(&simUART, false);
    
    rxVal = 0b1101010101;
    while(!simUART.flag_rxComplete)
    {
        piu_SimUART_TIMUpdate(&simUART, rxVal & 0x01);
        rxVal = rxVal >> 1;
    }
    
    REQUIRE(piu_SimUART_getRx(&simUART) == 0b01010101);
}
