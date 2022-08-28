#include <teletext.h>
#include <stdio.h>
#include <stdarg.h>
#include "util.h"

void
tt_buf_append_char(char *buf, size_t buf_sz, size_t *pos, char c)
{
	if (*pos < buf_sz)
		buf[(*pos)++] = c;
}

void
tt_buf_append_str(char *buf, size_t buf_sz, size_t *pos, const char *s)
{
	while (*s && *pos < buf_sz)
		buf[(*pos)++] = *(s++);
}

void
tt_buf_append_fmt(char *buf, size_t buf_sz, size_t *pos, const char *fmt, ...)
{
	va_list ap;

	if (*pos >= buf_sz)
		return;

	va_start(ap, fmt);
	*pos += vsnprintf(buf + *pos, buf_sz - *pos, fmt, ap);
	va_end(ap);

	if (*pos > buf_sz)
		*pos = buf_sz;
}

int
tt_buf_terminate(char *buf, size_t buf_sz, size_t pos)
{
	if (pos < buf_sz) {
		buf[pos] = '\0';
		return TT_OK;
	} else {
		buf[buf_sz-1] = '\0';
		return TT_BAD_SIZE;
	}
}
