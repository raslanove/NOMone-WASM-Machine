
#include <NTime.h>
#include <NSystemUtils.h>

void NMain(void) {
    struct NTime time;
    NTime.getTime(&time);
    NLOGI("Time test", "Time, seconds: %ld, nanos: %ld", time.timeSeconds, time.timeNanos);
}
