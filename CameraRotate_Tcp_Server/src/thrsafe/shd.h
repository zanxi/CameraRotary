#pragma once

#include "thr_safe.h"
#include "tsmap.h"
#include "libh.h"

#include "../sessionclient.h"

const int numStrokiKvant = 128 * 8;
const std::chrono::milliseconds sleepTimer = 500ms;
const std::chrono::milliseconds sleepTimerMain = 750ms;

//typedef tsmap<int, SessionClient*>  map_SessionClients;
//map_SessionClients map_sess;





