#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "libiodef.h"

#ifndef MAX
# define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif


typedef struct {
        libiodef_timer_t timer;
        time_t start_time;
} test_timer_t;


static unsigned int timer_alive = 0;


static unsigned int get_random_expire(unsigned int min, unsigned int max)
{
        return rand() % (max - min) + min;
}



static void timer_callback(void *data)
{
        test_timer_t *timer = data;
        unsigned int elapsed = time(NULL) - timer->start_time;

        assert(elapsed == libiodef_timer_get_expire(&timer->timer));

        libiodef_timer_destroy(&timer->timer);
        free(timer);

        timer_alive--;
}



int main(int argc, char **argv)
{
        int ret;
        time_t start;
        test_timer_t *timer;
        unsigned int i, expire, max_expire = 0;

        libiodef_init(NULL, NULL);
        start = time(NULL);

        /*
         * Create a bunch of timer for the first 3 seconds.
         */
         i = 100;
        while ( i-- ) {
                timer = malloc(sizeof(*timer));
                if ( ! timer )
                        exit(1);

                expire = get_random_expire(1, 60);
                max_expire = MAX(max_expire, expire);

                libiodef_timer_set_callback(&timer->timer, timer_callback);
                libiodef_timer_set_data(&timer->timer, timer);
                libiodef_timer_set_expire(&timer->timer, expire);
                libiodef_timer_init(&timer->timer);

                timer->start_time = timer->timer.start_time;

                timer_alive++;

                if ( time(NULL) - start >= 1 )
                        break;
        }

        printf("DONE initializing tmer_alive=%d max_expire=%u\n", timer_alive, max_expire);

        unsigned int prev_timer_alive = timer_alive;

        for ( i = 0; i <= max_expire && timer_alive; i++ ) {
                ret = libiodef_timer_wake_up();
                printf("%u timer woke up, %u remaining, next wake up in %d seconds.\n", prev_timer_alive - timer_alive, timer_alive, ret);

                prev_timer_alive = timer_alive;
                if ( ret )
                        sleep(ret);
        }

        assert(timer_alive == 0);
        libiodef_deinit();

        return 0;
}
