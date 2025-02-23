/*
 * dyn_buffer.c
 *
 *  Created on: Mar 23, 2017
 *      Author: Clemens Kresser
 *      License: MIT
 */


#include "dyn_buffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log.h"


/**
 * \brief: initializes a dynamic buffer
 *
 * \param *buffer: pointer to the buffer
 */
void dynBuffer_init(struct dyn_buffer *buffer)
{
  buffer->buffer = NULL;
  buffer->size = 0;
  buffer->used = 0;
}

/**
 * \brief: increases the buffer that it has the given amount of free memory
 *
 * \param *buffer: pointer to the buffer
 * \param numFreeBytes: the number of bytes that should be free after increase
 *
 * \return: 0 if successful else -1
 *
 */
int dynBuffer_increase_to(struct dyn_buffer *buffer, size_t numFreeBytes)
{
  if(buffer->buffer == NULL)
  {
    buffer->buffer = malloc(numFreeBytes);
    if(!buffer->buffer)
    {
      log_err("malloc failed");
      return -1;
    }

    buffer->size = numFreeBytes;
    buffer->used = 0;
  }
  else
  {
    char *newbuf;

    if(buffer->size - buffer->used < numFreeBytes)
    {
      buffer->size = buffer->used + numFreeBytes;
      newbuf = realloc(buffer->buffer, buffer->size);
      if(!newbuf)
      {
        free(buffer->buffer);
        buffer->buffer = NULL;
        buffer->size = 0;
        buffer->used = 0;
        log_err("realloc failed");
        return -1;
      }

      buffer->buffer = newbuf;
    }
  }
  return 0;
}

/**
 * \brief: removes the given amount of trailing bytes from the buffer
 *
 * \param *buffer: pointer to the buffer
 * \param count: the number of bytes that should be removed
 *
 * \return 0 if successful else -1
 */
int dynBuffer_removeTrailingBytes(struct dyn_buffer *buffer, size_t count)
{
  char *newbuf = NULL;

  if(buffer->buffer == NULL)
  {
    log_err("empty buffer");
    return -1;
  }

  if (!count)
    return 0;

  if(buffer->used < count)
  {
    log_err("not enough bytes in buffer");
    return -1;
  }

  if(buffer->used != count)   //TODO: make this more intelligent
  {
    newbuf = malloc(buffer->size - count);
    if(!newbuf)
    {
      log_err("malloc failed");
      return -1;
    }
    memcpy(newbuf, &buffer->buffer[count], buffer->used - count);
    free(buffer->buffer);
    buffer->buffer = newbuf;
    buffer->used = buffer->used - count;
    buffer->size = buffer->size - count;
  }
  else
  {
    free(buffer->buffer);
    buffer->buffer = NULL;
    buffer->used = 0;
    buffer->size = 0;
  }

  return 0;
}

/**
 * \brief: deallocates all memory that was allocated by the buffer
 *
 * \param *buffer: pointer to the buffer
 *
 * \return: 0 if successful else -1
 *
 */
int dynBuffer_delete(struct dyn_buffer *buffer)
{
  if (!buffer->buffer)
    return -1;

  free(buffer->buffer);
  buffer->buffer = NULL;
  buffer->size = 0;
  buffer->used = 0;

  return 0;
}

