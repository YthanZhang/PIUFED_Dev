//
// Created by YthanZhang on 2021/12/24.
//

#include "piu_vtimer.h"

#include "catch2/catch_all.hpp"


static bool flag_callback = false;
void callback()
{
    flag_callback = true;
}


TEST_CASE("virtual timer test", "[vtimer]")
{
    piu_VTimer vtimer = {};
    piu_VTimer_construct(&vtimer,
                         3,
                         piu_VTCDir_Down,
                         piu_VTMode_OneShot,
                         nullptr);

    piu_VTimer_startCounter(&vtimer);
    
    piu_VTimer_tick(&vtimer);    // c = 2
    piu_VTimer_tick(&vtimer);    // c = 1
    piu_VTimer_tick(&vtimer);    // c = 0
    
    REQUIRE(piu_VTimer_getCounter(&vtimer) == 0);
    
    piu_VTimer_tick(&vtimer);    // counter overflow

    // Counter should reload on overflow
    REQUIRE(piu_VTimer_getCounter(&vtimer) ==
            piu_VTimer_getCounterReloadValue(&vtimer));

    // Check overflow flag
    REQUIRE(piu_VTimer_getOverflow(&vtimer));
    // Overflow flag should reset on check
    REQUIRE_FALSE(piu_VTimer_getOverflow(&vtimer));

    piu_VTimer_startCounter(&vtimer);

    piu_VTimer_setCounterReloadValue(&vtimer, 5);
    
    piu_VTimer_tick(&vtimer);   // c = 2
    piu_VTimer_tick(&vtimer);   // c = 1
    piu_VTimer_tick(&vtimer);   // c = 0
    piu_VTimer_tick(&vtimer);   // overflow
    
    REQUIRE(piu_VTimer_getCounter(&vtimer) == 5);

    piu_VTimer_startCounter(&vtimer);

    piu_VTimer_setCountDirection(&vtimer, piu_VTCDir_Up);
    
    REQUIRE(piu_VTimer_getCountDirection(&vtimer) == piu_VTCDir_Down);
    
    piu_VTimer_stopCounter(&vtimer);
    
    REQUIRE_FALSE(piu_VTimer_getCounterActive(&vtimer));
    
    piu_VTimer_setCountDirection(&vtimer, piu_VTCDir_Up);
    
    REQUIRE(piu_VTimer_getCountDirection(&vtimer) == piu_VTCDir_Up);
    
    piu_VTimer_setCountDirection(&vtimer, piu_VTCDir_Down);
    piu_VTimer_setTimerMode(&vtimer, piu_VTMode_Continuous);
    piu_VTimer_startCounter(&vtimer);
    
    piu_VTimer_tick(&vtimer);   // c = 4
    piu_VTimer_tick(&vtimer);   // c = 3
    piu_VTimer_tick(&vtimer);   // c = 2
    piu_VTimer_tick(&vtimer);   // c = 1
    piu_VTimer_tick(&vtimer);   // c = 0
    piu_VTimer_tick(&vtimer);   // overflow
    
    REQUIRE(piu_VTimer_getOverflow(&vtimer));
    REQUIRE(piu_VTimer_getOverOverflow(&vtimer));
    REQUIRE(piu_VTimer_clearOverOverflow(&vtimer));
    REQUIRE_FALSE(piu_VTimer_getOverOverflow(&vtimer));
    REQUIRE(piu_VTimer_getCounterActive(&vtimer));

    piu_VTimer_stopCounter(&vtimer);
    piu_VTimer_setCallback(&vtimer, callback);
    piu_VTimer_startCounter(&vtimer);

    piu_VTimer_setCounterReloadValue(&vtimer, 0);
    piu_VTimer_reloadCounter(&vtimer);
    piu_VTimer_setCounterReloadValue(&vtimer, 5);
    
    piu_VTimer_tick(&vtimer);
    
    REQUIRE(flag_callback);
}
