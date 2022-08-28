#include <teletext.h>
#include <stdio.h>
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

#define XML_COLOR_LIST \
    X(TT_COLOR_BLACK,	"black")	\
    X(TT_COLOR_RED,	"red")		\
    X(TT_COLOR_GREEN,	"green")	\
    X(TT_COLOR_YELLOW,	"yellow")	\
    X(TT_COLOR_BLUE,	"blue")		\
    X(TT_COLOR_MAGENTA,	"magenta")	\
    X(TT_COLOR_CYAN,	"cyan")		\
    X(TT_COLOR_WHITE,	"white")

static const char *
color_to_xml(uint8_t color)
{
	static char buf[16];

#define X(Name, Str) case Name: return Str; break;
	switch (color) { XML_COLOR_LIST } // NOLINT(hicpp-multiway-paths-covered)
#undef X

	snprintf(buf, sizeof(buf), "%d", color);
	return buf;
}

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
tt_page_to_xml(const struct tt_page *page, char *buf, size_t buf_sz)
{
	int row, col;
	size_t pos=0;

	if (!page || !buf)
		return TT_BAD_ARG;

	tt_buf_append_fmt(buf, buf_sz, &pos,
	    "<?xml version=\"1.0\">\n"
	    "<page num=\"%d\" sub=\"%d\">\n",
	    page->page_no, page->sub_no);

	for (row = 0; row < TT_NUM_ROWS; row++)
	for (col = 0; col < TT_NUM_COLS; col++) {
		if (col == 0)
			tt_buf_append_str(buf, buf_sz, &pos, " <line>\n");

		tt_buf_append_fmt(buf, buf_sz, &pos, "  <char"
		    " set=\"g0\""
		    " code=\"%d\""
		    " fg=\"%s\""
		    " bg=\"%s\" />\n",
		    page->cells[row][col].ch.code,
		    color_to_xml(page->cells[row][col].attrs.fg_color),
		    color_to_xml(page->cells[row][col].attrs.bg_color));

		if (col == TT_NUM_COLS-1)
			tt_buf_append_str(buf, buf_sz, &pos, " </line>\n");
	}

	tt_buf_append_str(buf, buf_sz, &pos, "</page>\n"); // NOLINT(readability-misleading-indentation)

	if (pos >= buf_sz) {
		buf[buf_sz-1] = '\0';
		return TT_BAD_SIZE;
	}

	buf[pos] = '\0';
	return TT_OK;
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

	if (pos >= buf_sz) { // NOLINT(readability-misleading-indentation)
		buf[buf_sz-1] = '\0';
		return TT_BAD_SIZE;
	}

	buf[pos] = '\0';
	return TT_OK;
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

	if (pos >= buf_sz) { // NOLINT(readability-misleading-indentation)
		buf[buf_sz-1] = '\0';
		return TT_BAD_SIZE;
	}

	buf[pos] = '\0';
	return TT_OK;
}

