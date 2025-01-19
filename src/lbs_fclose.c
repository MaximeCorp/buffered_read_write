#include <stdlib.h>

#include "../include/libstream.h"

int lbs_fclose(struct stream *stream)
{
    int res = lbs_fflush(stream);

    if (res == -1)
    {
        res = close(stream->fd);
        free(stream);
        return -1;
    }

    res = close(stream->fd);
    free(stream);

    if (res == -1)
    {
        return -1;
    }

    return 0;
}
