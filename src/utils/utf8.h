/*
 * utf8.h
 *
 *  Created on: Apr 9, 2017
 *      Author: Clemens Kresser
 *      License: MIT
 */

#ifndef UTILS_UTF8_H_
#define UTILS_UTF8_H_

#include <stdbool.h>
#include <stddef.h>

enum utf8_state
{
  UTF8_STATE_OK,
  UTF8_STATE_FAIL,
  UTF8_STATE_BUSY,
};

enum utf8_state utf8_validate(char *string, size_t len, unsigned long *handle);

#endif /* UTILS_UTF8_H_ */
