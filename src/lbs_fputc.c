#include <stdio.h>

#include "../include/libstream.h"

int lbs_fputc(int c, struct stream *stream)
{
    if (!stream_writable(stream))
    {
        stream->error = 1;
        return -1;
    }

    if (stream->io_operation == STREAM_READING)
    {
        int res = lbs_fflush(stream);

        if (res == EOF)
        {
            stream->error = 1;
            return -1;
        }

        stream->io_operation = STREAM_WRITING;
    }

    if (stream->buffered_size == LBS_BUFFER_SIZE)
    {
        int res = lbs_fflush(stream);

        if (res == EOF)
        {
            stream->error = 1;
            return -1;
        }
    }

    stream->buffer[stream->buffered_size++] = c;

    int res = 0;

    if (stream->buffering_mode == STREAM_UNBUFFERED)
    {
        res = lbs_fflush(stream);
    }
    else if (stream->buffering_mode == STREAM_LINE_BUFFERED)
    {
        if (c == '\n')
        {
            res = lbs_fflush(stream);
        }
    }

    if (res == EOF)
    {
        stream->error = 1;
        return -1;
    }

    return c;
}
