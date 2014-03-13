/*
*   $Id$
*
*   Copyright (c) 1996-2003, Darren Hiebert
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License.
*
*   This module contains functions for checking multibyte character set.
*/

/*
*   INCLUDE FILES
*/

#ifdef SUPPORT_MULTIBYTE

#include <stdio.h>
#include <iconv.h>
#include "options.h"
#include "routines.h"
#include "mbcs.h"

static iconv_t iconv_fd = (iconv_t) -1;

extern boolean openConverter (char* encoding)
{
	if (!encoding)
		return FALSE;
	iconv_fd = iconv_open("UTF-8", encoding);
	return iconv_fd != (iconv_t) -1;
}

extern boolean convertString (vString *const string)
{
	size_t utf8_len = 0, mbcs_len, utf8_len_old;
	char *utf8, *mbcs, *utf8ptr;
	if (iconv_fd == (iconv_t) -1)
		return FALSE;
	mbcs_len = vStringLength (string);
	utf8_len_old = utf8_len = mbcs_len * 6;
	utf8 = xMalloc (utf8_len, char);
	if (!utf8)
		return FALSE;
	*utf8 = 0;
	utf8ptr = utf8;
	mbcs = vStringValue (string);
	if (iconv (iconv_fd, &mbcs, &mbcs_len, &utf8ptr, &utf8_len) == (size_t) -1)
	{
	  	eFree (utf8);
		return FALSE;
	}
	utf8_len = utf8_len_old - utf8_len;

	vStringClear (string);
	while (vStringSize (string) <= utf8_len + 1)
		vStringAutoResize (string);
	strcpy (vStringValue (string), utf8);
	vStringLength (string) = utf8_len;
  	eFree (utf8);
	return TRUE;
}

extern void closeConverter ()
{
	if (iconv_fd != (iconv_t) -1)
		iconv_close(iconv_fd);
}

#endif	/* SUPPORT_MULTIBYTE */
