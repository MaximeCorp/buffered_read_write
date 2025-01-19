#include <stdio.h>

#include "../include/libstream.h"

int lbs_fflush(struct stream *stream)
{
    if (stream->io_operation == STREAM_WRITING)
    {
        if (!stream_writable(stream))
        {
            return EOF;
        }

        if (stream->buffered_size)
        {
            int res = write(stream->fd, stream->buffer, stream->buffered_size);
            if (res == -1)
            {
                stream->error = 1;
                return EOF;
            }
        }

        stream->already_read = 0;
        stream->buffered_size = 0;
    }
    else
    {
        if (stream->already_read - stream->buffered_size)
        {
            int res =
                lseek(stream->fd, -stream_remaining_buffered(stream), SEEK_CUR);

            if (res == -1)
            {
                stream->error = 1;
                return -1;
            }
        }

        stream->buffered_size = 0;
        stream->already_read = 0;
    }

    return 0;
}
