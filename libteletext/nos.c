#include <teletext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

#define LEN(a)  (sizeof(a)/sizeof(*(a)))

#define HTML_ENTITY_LIST \
    /* special */					\
    X("&qout;",		0x22)	X("&amp;",	0x26)	\
    X("&apos;",		0x27)	X("&lt;",	0x3C)	\
    X("&gt;",		0x3E)				\
    /* Latin */						\
    X("&AElig;",	0xC6)	X("&Aacute;",	0xC1)	\
    X("&Acirc;",	0xC2)	X("&Agrave;",	0xC0)	\
    X("&Aring;",	0xC5)	X("&Atilde;",	0xC3)	\
    X("&Auml;",		0xC4)	X("&Ccedil;",	0xC7)	\
    X("&ETH;",		0xD0)	X("&Eacute;",	0xC9)	\
    X("&Ecirc;",	0xCA)	X("&Egrave;",	0xC8)	\
    X("&Euml;",		0xCB)	X("&Iacute;",	0xCD)	\
    X("&Icirc;",	0xCE)	X("&Igrave;",	0xCC)	\
    X("&Iuml;",		0xCF)	X("&Ntilde;",	0xD1)	\
    X("&Oacute;",	0xD3)	X("&Ocirc;",	0xD4)	\
    X("&Ograve;",	0xD2)	X("&Oslash;",	0xD8)	\
    X("&Otilde;",	0xD5)	X("&Ouml;",	0xD6)	\
    X("&THORN;",	0xDE)	X("&Uacute;",	0xDA)	\
    X("&Ucirc;",	0xDB)	X("&Ugrave;",	0xD9)	\
    X("&Uuml;",		0xDC)	X("&Yacute;",	0xDD)	\
    X("&aacute;",	0xE1)	X("&acirc;",	0xE2)	\
    X("&aelig;",	0xE6)	X("&agrave;",	0xE0)	\
    X("&aring;",	0xE5)	X("&atilde;",	0xE3)	\
    X("&auml;",		0xE4)	X("&ccedil;",	0xE7)	\
    X("&eacute;",	0xE9)	X("&ecirc;",	0xEA)	\
    X("&egrave;",	0xE8)	X("&eth;",	0xF0)	\
    X("&euml;",		0xEB)	X("&iacute;",	0xED)	\
    X("&icirc;",	0xEE)	X("&igrave;",	0xEC)	\
    X("&iuml;",		0xEF)	X("&ntilde;",	0xF1)	\
    X("&oacute;",	0xF3)	X("&ocirc;",	0xF4)	\
    X("&ograve;",	0xF2)	X("&oslash;",	0xF8)	\
    X("&otilde;",	0xF5)	X("&ouml;",	0xF6)	\
    X("&szlig;",	0xDF)	X("&thorn;",	0xFE)	\
    X("&uacute;",	0xFA)	X("&ucirc;",	0xFB)	\
    X("&ugrave;",	0xF9)	X("&uuml;",	0xFC)	\
    X("&yacute;",	0xFD)	X("&yuml;",	0xFF)

#define X(Str, Cp) { Str, sizeof(Str)-1, Cp },
static const struct html_entity {
	const char *str;
	size_t str_len;
	uint32_t codepoint;
} html_entities[] = { HTML_ENTITY_LIST };
#undef X

#define HTML_COLOR_LIST \
    X(TT_COLOR_BLACK,	"black")	\
    X(TT_COLOR_RED,	"red")		\
    X(TT_COLOR_GREEN,	"green")	\
    X(TT_COLOR_YELLOW,	"yellow")	\
    X(TT_COLOR_BLUE,	"blue")		\
    X(TT_COLOR_MAGENTA,	"magenta")	\
    X(TT_COLOR_CYAN,	"cyan")		\
    X(TT_COLOR_WHITE,	"white")

static const uint8_t html_cp_char_map[] = {
    [' '] =  0, ['!'] =  6, ['"'] = 12, ['#'] = 18, ['$'] =  0, ['%'] = 30,
    ['&'] = 36, ['\'']= 42, ['('] = 48, [')'] = 54, ['*'] = 60, ['+'] = 66,
    [','] = 72, ['-'] = 78, ['.'] = 84, ['/'] = 90, ['0'] =  1, ['1'] =  7,
    ['2'] = 13, ['3'] = 19, ['4'] = 25, ['5'] = 31, ['6'] = 37, ['7'] = 43,
    ['8'] = 49, ['9'] = 55, [':'] = 61, [';'] = 67, ['<'] = 73, ['='] = 79,
    ['>'] = 85, ['?'] = 91, ['@'] =  0, ['A'] =  8, ['B'] = 14, ['C'] = 20,
    ['D'] = 26, ['E'] = 32, ['F'] = 38, ['G'] = 44, ['H'] = 50, ['I'] = 56,
    ['J'] = 62, ['K'] = 68, ['L'] = 74, ['M'] = 80, ['N'] = 86, ['O'] = 92,
    ['P'] =  3, ['Q'] =  9, ['R'] = 15, ['S'] = 21, ['T'] = 27, ['U'] = 33,
    ['V'] = 39, ['W'] = 45, ['X'] = 51, ['Y'] = 57, ['Z'] = 63, ['['] = 69,
    ['\\']= 75, [']'] = 81, ['^'] = 87, ['_'] = 93, ['`'] =  4, ['a'] = 10,
    ['b'] = 16, ['c'] = 22, ['d'] = 28, ['e'] = 34, ['f'] = 40, ['g'] = 46,
    ['h'] = 52, ['i'] = 58, ['j'] = 64, ['k'] = 70, ['l'] = 76, ['m'] = 82,
    ['n'] = 88, ['o'] = 94, ['p'] =  5, ['q'] = 11, ['r'] = 17, ['s'] = 23,
    ['t'] = 29, ['u'] = 35, ['v'] = 41, ['w'] = 47, ['x'] = 53, ['y'] = 59,
    ['z'] = 65, ['{'] = 71, ['|'] = 77, ['}'] = 83, ['~'] = 89
};

static const struct tt_cell blank_cell = {
	.attrs = {
		.bg_color = TT_COLOR_BLACK,
		.fg_color = TT_COLOR_WHITE
	}
};

static const char *
find_space(const char *s, const char *end)
{
	while (*s && s < end && !isspace(*s))
		s++;

	return s;
}

static uint32_t
html_get_cp(const char *html, const char **endp)
{
	size_t i;
	uint32_t wc;
	const struct html_entity *entity;

	if (html[0] != '&') {
		*endp = &html[1];
		return html[0];
	}

	if (!strncmp("&#x", html, 3)) {
		wc = (uint32_t)strtol(&html[3], (char **)endp, 16);
		if (**endp == ';')
			(*endp)++;
		return wc;
	}

	for (i = 0; i < LEN(html_entities); i++) {
		entity = &html_entities[i];
		if (strncmp(html, entity->str, entity->str_len) != 0)
			continue;
		*endp = &html[entity->str_len];
		return entity->codepoint;
	}

	*endp = &html[1];
	return html[0];
}

static struct tt_char
html_cp_to_char(uint32_t wc)
{
	struct tt_char ch = {};

	if (wc > 0xF000)
		ch.code = html_cp_char_map['%']; /* TODO */
	else if (wc < LEN(html_cp_char_map))
		ch.code = html_cp_char_map[wc];

	return ch;
}

static void
attrs_to_classnames(
    struct tt_attrs attrs,
    const char **fg_class,
    const char **bg_class)
{
	*fg_class = "";
	*bg_class = "";

#define X(Name, Str) case Name: *fg_class = Str; break;
	switch (attrs.fg_color) { HTML_COLOR_LIST }
#undef X

#define X(Name, Str) case Name: *bg_class = "bg-" Str; break;
	switch (attrs.bg_color) { HTML_COLOR_LIST }
#undef X
}

static void
classnames_to_attrs(const char *classes, const char *end,
    struct tt_attrs *attrs)
{
	const char *p;
	const char *word_end;
	size_t len;

	p = classes;
	while (*p && p < end) {
		if (isspace(*p))
			{ p++; continue; }

		word_end = find_space(p+1, end);
		len = word_end - p;

#define X(Name, Str) \
	if (!strncmp(p, Str, len)) attrs->fg_color = Name; else \
	if (!strncmp(p, "bg-" Str, len)) attrs->bg_color = Name; else
		HTML_COLOR_LIST ;
#undef X

		p = word_end;
	}
}

int
tt_page_to_nos_html(const struct tt_page *page, char *buf, size_t buf_sz)
{
	int row, col;
	int in_span=0;
	size_t pos=0;
	struct tt_attrs prev_attrs;
	const struct tt_cell *cur;
	const char *fg_class, *bg_class;
	char ascii;

	if (!page || !buf)
		return TT_BAD_ARG;

	for (row = 0; row < TT_NUM_ROWS; row++)
	for (col = 0; col < TT_NUM_COLS; col++) {
		cur = &page->cells[row][col];

		if (col == 0) {
			prev_attrs = blank_cell.attrs;
			in_span = 0;
		}

		if (memcmp(&cur->attrs, &prev_attrs, sizeof(prev_attrs)) != 0) {
			attrs_to_classnames(cur->attrs, &fg_class, &bg_class);
			if (in_span)
				tt_buf_append_str(buf, buf_sz, &pos, "</span>");
			tt_buf_append_fmt(buf, buf_sz, &pos,
			    "<span class=\"%s %s\">",
			    fg_class, bg_class);
			in_span = 1;
			prev_attrs = cur->attrs;
		}

		ascii = tt_char_to_ascii(cur->ch);
		tt_buf_append_char(buf, buf_sz, &pos, ascii);

		if (col == 79)
			tt_buf_append_str(buf, buf_sz, &pos, "</span>\n");
	}

	tt_buf_append_char(buf, buf_sz, &pos, '\0'); // NOLINT(readability-misleading-indentation)

	return pos < buf_sz ? TT_OK : TT_BAD_SIZE;
}

int
tt_page_from_nos_html(const char *html, struct tt_page *page)
{
	const char *p;
	uint32_t wc;
	int row, col;
	struct tt_attrs attrs_stack[8];
	struct tt_attrs attrs;
	int stack_depth=0;
	const char *open_quotes;
	const char *close_quotes;
	const char *close_bracket;

	if (!html || !page)
		return TT_BAD_ARG;

	for (row = 0; row < TT_NUM_ROWS; row++)
	for (col = 0; col < TT_NUM_COLS; col++)
		page->cells[row][col] = blank_cell;

	p = html; // NOLINT(readability-misleading-indentation)
	col = row = 0;
	attrs = blank_cell.attrs;

	while (*p) {
		if (row >= TT_NUM_ROWS)
			break;

		if (*p == '\n') {
			col = 0;
			row++;
			p++;
		} else if (p[0] == '<' && p[1] == '/') {
			p++;
			close_bracket = strchr(p, '>');
			if (!close_bracket)
				return TT_BAD_DATA;
			p = close_bracket+1;

			if (stack_depth && stack_depth <= LEN(attrs_stack))
				attrs = attrs_stack[stack_depth-1];
			if (stack_depth)
				stack_depth--;
		} else if (p[0] == '<') {
			p++;
			if ((open_quotes = strchr(p, '"'))) {
				p = open_quotes+1;
				if (!(close_quotes = strchr(p, '"')))
					return TT_BAD_DATA;
				p = close_quotes+1;
			}

			if (!(close_bracket = strchr(p, '>')))
				return TT_BAD_DATA;
			p = close_bracket+1;

			stack_depth++;
			if (stack_depth <= LEN(attrs_stack))
				attrs_stack[stack_depth-1] = attrs;
			if (open_quotes)
				classnames_to_attrs(open_quotes+1, close_quotes,
				    &attrs);
		} else {
			wc = html_get_cp(p, &p);
			col++;

			if (col < TT_NUM_COLS) {
				page->cells[row][col].ch = html_cp_to_char(wc);
				page->cells[row][col].attrs = attrs;
			}
		}
	}

	return TT_OK;
}
