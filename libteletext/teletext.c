#include <teletext.h>
#include "util.h"

#define LEN(a)  (sizeof(a)/sizeof(*(a)))

#define X(Name, Str) [Name] = Str,
static const char *err_strs[] = { TT_LIST_ERRORS };
#undef X

static const char g0_latin_ascii_map[] =
    " 0@P`p"
    "!1AQaq"
    "\"2BRbr"
    "#3CScs"
    "@4DTdt"
    "%5EUeu"
    "&5FVfv"
    "'7GWgw"
    "(8HXhx"
    ")9IYiy"
    "*:JZjz"
    "+;K[k{"
    ",<L\\l|"
    "-=M]m}"
    ".>N^n~"
    "/?O_o#";

const char *
tt_strerror(int code)
{
	if (code < 0 || code > LEN(err_strs) || !err_strs[code])
		return "Unknown error";

	return err_strs[code];
}

char
tt_char_to_ascii(struct tt_char ch)
{
	char c = 0;

	if (ch.code < LEN(g0_latin_ascii_map))
		c = g0_latin_ascii_map[ch.code];
	if (!c)
		c = ' ';

	return c;
}

int
tt_page_to_ascii(const struct tt_page *page, char *buf, size_t buf_sz)
{
	int row, col;
	size_t pos=0;
	const struct tt_cell *cur;
	char ascii;

	if (!page || !buf)
		return TT_BAD_ARG;

	for (row = 0; row < TT_NUM_ROWS; row++)
	for (col = 0; col < TT_NUM_COLS; col++) {
		cur = &page->cells[row][col];
		ascii = tt_char_to_ascii(cur->ch);
		tt_buf_append_char(buf, buf_sz, &pos, ascii);

		if (col == 79)
			tt_buf_append_char(buf, buf_sz, &pos, '\n');
	}

	tt_buf_append_char(buf, buf_sz, &pos, '\0'); // NOLINT(readability-misleading-indentation)

	return pos < buf_sz ? TT_OK : TT_BAD_SIZE;
}

int
tt_page_to_ansi(const struct tt_page *page, char *buf, size_t buf_sz)
{
	int row, col;
	size_t pos=0;
	const struct tt_cell *prev=NULL, *cur;
	char ascii;

	if (!page || !buf)
		return TT_BAD_ARG;

	for (row = 0; row < TT_NUM_ROWS; row++)
	for (col = 0; col < TT_NUM_COLS; col++) {
		cur = &page->cells[row][col];

		if (!prev ||
		    cur->attrs.bg_color != prev->attrs.bg_color ||
		    cur->attrs.fg_color != prev->attrs.fg_color) {
			if (cur->attrs.bg_color > 7 || cur->attrs.fg_color > 7)
				return TT_BAD_DATA;

			tt_buf_append_fmt(buf, buf_sz, &pos, "\033[%d;%dm",
			    30 + cur->attrs.fg_color,
			    40 + cur->attrs.bg_color);
		}

		prev = cur;
		ascii = tt_char_to_ascii(cur->ch);
		tt_buf_append_char(buf, buf_sz, &pos, ascii);

		if (col == 79)
			tt_buf_append_str(buf, buf_sz, &pos, "\n\033[0m");
	}

	tt_buf_append_char(buf, buf_sz, &pos, '\0'); // NOLINT(readability-misleading-indentation)

	return pos < buf_sz ? TT_OK : TT_BAD_SIZE;
}

