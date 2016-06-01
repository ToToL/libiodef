#include "config.h"

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "libiodef.h"

#include "glthread/lock.h"


struct asyncobj {
        LIBIODEF_ASYNC_OBJECT;
        int myval;
};


static int async_done = 0;
static int timer_count = 0;
static gl_lock_t lock = gl_lock_initializer;


static void timer_cb(void *data)
{
        gl_lock_lock(lock);
        timer_count++;
        libiodef_timer_reset(data);
        gl_lock_unlock(lock);
}


static void async_func(void *obj, void *data)
{
        struct asyncobj *ptr = obj;

        gl_lock_lock(lock);
        async_done = 1;
        assert(ptr->myval == 10);
        gl_lock_unlock(lock);
}


int main(void)
{
        libiodef_timer_t timer;
        struct asyncobj myobj;

        assert(libiodef_init(NULL, NULL) == 0);
        assert(libiodef_async_init() == 0);
        libiodef_async_set_flags(LIBIODEF_ASYNC_FLAGS_TIMER);

        libiodef_timer_set_expire(&timer, 1);
        libiodef_timer_set_data(&timer, &timer);
        libiodef_timer_set_callback(&timer, timer_cb);
        libiodef_timer_init(&timer);

        sleep(3);

        gl_lock_lock(lock);
        assert(timer_count >= 2);
        gl_lock_unlock(lock);

        myobj.myval = 10;
        libiodef_async_set_callback((libiodef_async_object_t *) &myobj, async_func);
        libiodef_async_add((libiodef_async_object_t *) &myobj);

        libiodef_async_exit();
        assert(async_done);

        libiodef_deinit();
        exit(0);
}
