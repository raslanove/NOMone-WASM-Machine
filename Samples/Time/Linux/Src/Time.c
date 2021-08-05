
#include <stdio.h>

#include <NTime.h>

void main(void) {
    struct NTime time;
    NTime.getTime(&time);
    printf("Time, seconds: %ld, nanos: %ld", time.timeSeconds, time.timeNanos);
}
