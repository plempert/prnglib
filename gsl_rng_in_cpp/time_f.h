//
// Created by Patrick Lempert on 5/22/16.
//

#include <sys/time.h>

static struct timeval tm1;

static inline void start()
{
    gettimeofday(&tm1, NULL);
}

static inline void stop()
{
    struct timeval tm2;
    gettimeofday(&tm2, NULL);

    unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
//    printf("%llu ms\n", t);
    printf("%.3f ms\n", t/1000.0);

}