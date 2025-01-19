#include <stdio.h>

#include "../include/libstream.h"

int lbs_fgetc(struct stream *stream)
{
    if (!stream_readable(stream))
    {
        stream->error = 1;
        return -1;
    }

    if (stream->io_operation == STREAM_WRITING)
    {
        int res = lbs_fflush(stream);

        if (res == EOF)
        {
            stream->error = 1;
            return -1;
        }

        stream->io_operation = STREAM_READING;
    }

    if (stream->buffered_size == stream->already_read)
    {
        int res = lbs_fflush(stream);

        if (res == EOF)
        {
            stream->error = 1;
            return -1;
        }
    }

    if (stream->buffered_size == 0)
    {
        ssize_t bytes = read(stream->fd, stream->buffer, LBS_BUFFER_SIZE);

        if (bytes == 0)
        {
            return LBS_EOF;
        }

        stream->already_read = 0;
        stream->buffered_size = bytes;
    }

    int c = stream->buffer[stream->already_read++];

    if (c == EOF)
    {
        return EOF;
    }

    unsigned char c4 = c;

    return c4;
}
