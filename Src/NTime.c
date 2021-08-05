
#include <NTime.h>
#include <NSystemUtils.h>

static void getTime(struct NTime* outputTime) {
    NSystemUtils.getTime(&(outputTime->timeSeconds), &(outputTime->timeNanos));
}

const struct NTime_Interface NTime = {
    .getTime = getTime
};