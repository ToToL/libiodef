#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "libiodef.h"


int main(int argc, char **argv)
{
        int ret;
        libiodef_client_t *client;

        assert(libiodef_init(&argc, argv) == 0);
        assert(libiodef_client_new(&client, "Client that does not exist") == 0);
        assert((ret = libiodef_client_start(client)) < 0);
        assert(libiodef_error_get_code(ret) == LIBIODEF_ERROR_PROFILE);

        libiodef_client_destroy(client, LIBIODEF_CLIENT_EXIT_STATUS_FAILURE);
        libiodef_deinit();

        return 0;
}
