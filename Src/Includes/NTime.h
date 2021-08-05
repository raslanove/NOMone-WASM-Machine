
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 4th of August 2021.
//////////////////////////////////////////////////////

#pragma once

#include <NTypes.h>

struct NTime {
    int64_t timeSeconds;
    int64_t timeNanos;
};

struct NTime_Interface {
    void (*getTime)(struct NTime* outputTime);
};

extern const struct NTime_Interface NTime;