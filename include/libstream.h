#ifndef LIBSTREAM_H
#define LIBSTREAM_H

#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>

#define LBS_EOF (-1)

#define LBS_BUFFER_SIZE 32

enum stream_io_operation
{
    STREAM_READING = 0,
    STREAM_WRITING,
};

enum stream_buffering
{
    STREAM_UNBUFFERED = 0,
    STREAM_LINE_BUFFERED,
    STREAM_BUFFERED,
};

struct stream
{
    int flags;

    int error;

    int fd;

    enum stream_io_operation io_operation;

    enum stream_buffering buffering_mode;

    size_t buffered_size;

    size_t already_read;

    char buffer[LBS_BUFFER_SIZE];
};

static inline size_t stream_remaining_buffered(struct stream *stream)
{
    return stream->buffered_size - stream->already_read;
}

static inline size_t stream_unused_buffer_space(struct stream *stream)
{
    return sizeof(stream->buffer) - stream->buffered_size;
}

static inline bool stream_readable(struct stream *stream)
{
    int access_mode = stream->flags & O_ACCMODE;
    if (access_mode == O_RDWR)
        return true;
    return access_mode == O_RDONLY;
}

static inline bool stream_writable(struct stream *stream)
{
    int access_mode = stream->flags & O_ACCMODE;
    if (access_mode == O_RDWR)
        return true;
    return access_mode == O_WRONLY;
}

static inline int lbs_ferror(struct stream *stream)
{
    return stream->error;
}

static inline void lbs_clearerr(struct stream *stream)
{
    stream->error = 0;
}

static inline void lbs_setbufmode(struct stream *stream,
                                  enum stream_buffering mode)
{
    stream->buffering_mode = mode;
}

struct stream *lbs_fopen(const char *path, const char *mode);
struct stream *lbs_fdopen(int fd, const char *mode);
int lbs_fflush(struct stream *stream);
int lbs_fclose(struct stream *stream);
int lbs_fputc(int c, struct stream *stream);
int lbs_fgetc(struct stream *stream);

#endif
