#include "config.h"

#include <stdlib.h>
#include <assert.h>
#include "prelude.h"

#define TEST_STR "abcdefghijklmnopqrstuvwxyz"
#define MAX_LAG_SEC 3


int main(void)
{
        time_t now;
        iodef_time_t *ctime;
        iodef_alert_t *alert;
        iodef_document_t *iodef;

        assert(iodef_document_new(&iodef) == 0);
        assert(iodef_document_new_alert(iodef, &alert) == 0);

        ctime = iodef_alert_get_create_time(alert);
        assert(ctime != NULL);

        now = time(NULL);
        assert(now - iodef_time_get_sec(ctime) < MAX_LAG_SEC);

        exit(0);
}
