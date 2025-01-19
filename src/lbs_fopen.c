#include <stdlib.h>
#include <string.h>

#include "../include/libstream.h"

struct stream *lbs_fopen(const char *path, const char *mode)
{
    struct stream *s = calloc(1, sizeof(struct stream));

    if (!strcmp(mode, "r"))
    {
        s->flags = O_RDONLY;
        s->io_operation = STREAM_READING;
    }
    else if (!strcmp(mode, "w"))
    {
        s->flags = O_WRONLY | O_CREAT | O_TRUNC;
        s->io_operation = STREAM_WRITING;
    }
    else if (!strcmp(mode, "r+"))
    {
        s->flags = O_RDWR;
        s->io_operation = STREAM_READING;
    }
    else if (!strcmp(mode, "w+"))
    {
        s->flags = O_RDWR | O_CREAT | O_TRUNC;
        s->io_operation = STREAM_WRITING;
    }
    else
    {
        return NULL;
    }

    int fd = open(path, s->flags);

    if (fd == -1)
    {
        free(s);
        return NULL;
    }

    s->fd = fd;

    s->buffering_mode = isatty(fd) ? STREAM_LINE_BUFFERED : STREAM_BUFFERED;

    return s;
}
