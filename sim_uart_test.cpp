//
// Created by YthanZhang on 2021/12/30.
//


#include "piu_sim_uart.h"

#include "catch2/catch_all.hpp"


bool tx = true;

uint16_t rxVal = 0b1100110101;


void setTxBit(bool txVal) { tx = txVal; }


piu_SimUART simUART;




TEST_CASE("Sim UART Test", "[sim_uart]")
{
    piu_SimUART_construct(&simUART, setTxBit);
    
    /** Test Tx ***************************************************************/
    piu_SimUART_sendTx(&simUART, 0b10010011);
    REQUIRE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE_FALSE(tx);
    REQUIRE_FALSE(simUART.flag_txComplete);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE_FALSE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE_FALSE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE_FALSE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE_FALSE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(simUART.flag_txComplete);
    
    piu_SimUART_TIMUpdate(&simUART, true);
    piu_SimUART_TIMUpdate(&simUART, true);
    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);

    /** Test Rx ***************************************************************/
    piu_SimUART_GPIOUpdate(&simUART, false);
    
    while(!simUART.flag_rxComplete)
    {
        piu_SimUART_TIMUpdate(&simUART, rxVal & 0x01);
        rxVal = rxVal >> 1;
    }
    
    REQUIRE(piu_SimUART_getRx(&simUART) == 0b00110101);
    
    /** Test simultaneous Rx Tx ***********************************************/
    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);

    piu_SimUART_sendTx(&simUART, 0b10010011);
    REQUIRE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE_FALSE(tx);
    REQUIRE_FALSE(simUART.flag_txComplete);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE_FALSE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE_FALSE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE_FALSE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE_FALSE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);

    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(simUART.flag_txComplete);
    
    piu_SimUART_TIMUpdate(&simUART, true);
    piu_SimUART_TIMUpdate(&simUART, true);
    piu_SimUART_TIMUpdate(&simUART, true);
    REQUIRE(tx);
    
    piu_SimUART_GPIOUpdate(&simUART, false);
    
    while(!simUART.flag_rxComplete)
    {
        piu_SimUART_TIMUpdate(&simUART, rxVal & 0x01);
        rxVal = rxVal >> 1;
    }
    
    REQUIRE(piu_SimUART_getRx(&simUART) == 0b00110101);
}
