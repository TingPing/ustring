/* ustring
 * 
 * Copyright (c) 2014
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#if __STDC_VERSION__ < 199901L
#error C99 is required!
#else

#include <stddef.h>
#include <string.h>
#include "ustring.h"

/**
 * This library contains functions to work with utf8 strings.
 * It does not necessarily ensure any string is valid but it
 * does ensure that you never copy part of a multibyte char.
 */


/** 
 * Returns the length in bytes of a utf8 character.
 */
inline size_t
ucharlen (const char *str)
{
    // Taken from glib
    static const char utf8_skip[256] = {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
    };

    if (str == NULL)
        return 0;

    return utf8_skip[*((const unsigned char *)str)];
}

/** 
 * Returns the number of utf8 characters in a string.
 */
size_t
ustrlen (const char *str)
{
    if (str == NULL)
        return 0;

    size_t len = 0;
    for (; *str != '\0'; str += ucharlen (str))
    {
        len++;
    }
    return len;
}

// Used by both ustrlcpy() and ustrlcat()
static inline size_t
utf8_copy (char *restrict dst, const char *restrict src, size_t n_bytes)
{
    const char *src_start = src;

    while (n_bytes != 0)
    {
        size_t char_len = ucharlen (src);

        // Doesn't fit, terminate
        if (n_bytes < (char_len + 1))
        {
            *dst = '\0';
            break;
        }

        if (char_len == 1)
        {
            *dst = *src;
            if (*src == '\0')
                break;
        }
        else
        {
            memcpy (dst, src, char_len);
        }

        dst += char_len;
        src += char_len;
        n_bytes -= char_len;
    }

    // Finish going through str to return the strlen
    while (*src++ != '\0');

    // Don't include terminator
    return (src - src_start - 1);
}

/**
 * Concats utf8 string from src onto dst.
 * The result is guaranteed to be null-terminated if n_bytes > 0.
 *
 * Returns total length of dst and src.
 */
size_t
ustrlcat (char *restrict dst, const char *restrict src, size_t n_bytes)
{
    if (src == NULL)
        return 0;

    if (dst == NULL)
        return strlen (src);

    const char *dst_start = dst;
    while (n_bytes != 0 && *dst != '\0')
    {
        n_bytes--;
        dst++;
    }

    return (dst - dst_start + utf8_copy (dst, src, n_bytes));
}

/**
 * Copies n_bytes-1 of utf8 characters into dst.
 * The result is guaranteed to be null-terminated if n_bytes > 0.
 *
 * Returns length of src.
 */
size_t
ustrlcpy (char *restrict dst, const char *restrict src, size_t n_bytes)
{
    if (src == NULL)
        return 0;

    if (dst == NULL)
        return strlen (src);

    return utf8_copy (dst, src, n_bytes);
}

#endif // C99
