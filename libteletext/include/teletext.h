#ifndef TELETEXT_H
#define TELETEXT_H

#ifndef __cplusplus
# include <stddef.h>
# include <stdint.h>
#else
# include <cstddef>
# include <cstdint>
# include <string>

namespace teletext {
extern "C" {
#endif

#define TT_LIST_ERRORS \
    X(TT_OK,		"Success")				\
    X(TT_MISC_ERROR,    "Misc. error")				\
    X(TT_UNSUPPORTED,	"Not supported or implemented")		\
    X(TT_BAD_ARG,	"Bad argument")				\
    X(TT_BAD_SIZE,	"Insufficient buffer size")		\
    X(TT_BAD_RANGE,	"Index out of range")			\
    X(TT_BAD_DATA,	"Bad data")				\
    X(TT_CURL_ERROR,	"An error occurred in libcurl")

#define X(Name, Str) Name,
enum { TT_LIST_ERRORS };
#undef X

struct tt_char {
	uint8_t charset;
#define TT_CHARSET_GO_LATIN	0
#define TT_CHARSET_G1		1
	uint8_t code;
};

struct tt_attrs {
	uint8_t bg_color;
	uint8_t fg_color;
#define TT_COLOR_BLACK		0
#define TT_COLOR_RED		1
#define TT_COLOR_GREEN		2
#define TT_COLOR_YELLOW		3
#define TT_COLOR_BLUE		4
#define TT_COLOR_MAGENTA	5
#define TT_COLOR_CYAN		6
#define TT_COLOR_WHITE		7
	uint8_t flags;
#define TT_CHAR_DOUBLE_WIDTH		(1 << 0)
#define TT_CHAR_DOUBLE_HEIGHT		(1 << 1)
#define TT_CHAR_FLASH			(1 << 2)
#define TT_CHAR_BOX			(1 << 3)
#define TT_CHAR_CONCEAL			(1 << 4)
#define TT_CHAR_MOSAIC_SEPARATED	(1 << 5)
#define TT_CHAR_MOSAIC_HOLD		(1 << 6)
};

struct tt_cell {
	struct tt_char ch;
	struct tt_attrs attrs;
};

struct tt_page {
	uint16_t page_no;
	uint8_t sub_no;
#define TT_NUM_ROWS	24
#define TT_NUM_COLS	40
	struct tt_cell cells[TT_NUM_ROWS][TT_NUM_COLS];
};

const char *tt_strerror(int code);

char tt_char_to_ascii(struct tt_char ch);

int tt_page_to_xml(const struct tt_page *, char *buf, size_t buf_sz);
int tt_page_to_ascii(const struct tt_page *, char *buf, size_t buf_sz);
int tt_page_to_ansi(const struct tt_page *, char *buf, size_t buf_sz);
int tt_page_to_nos_html(const struct tt_page *, char *buf, size_t buf_sz);
int tt_page_from_nos_html(const char *html, struct tt_page *);
int tt_page_from_nos_json(const char *json, struct tt_page *);
int tt_page_from_nos_api(uint16_t page_no, uint8_t sub_no, struct tt_page *);

#ifdef __cplusplus
}; /* extern C */

class page {
	struct tt_page data_ {};

public:
	page() = default;
	page(const page &other) : data_(other.data()) {}
	explicit page(const tt_page &data) : data_(data) {}

	[[nodiscard]] tt_page &data() { return data_; }
	[[nodiscard]] const tt_page &data() const { return data_; }

	[[nodiscard]] std::string to_xml() const;
	[[nodiscard]] std::string to_ascii() const;
	[[nodiscard]] std::string to_ansi() const;
	[[nodiscard]] std::string to_nos_html() const;
};

class teletext_error : public std::exception {
	const char *msg_;
public:
	explicit teletext_error(const char *msg) : msg_(msg) {}
	explicit teletext_error(std::string &msg) : msg_(msg.c_str()) {}
	explicit teletext_error(int error) : msg_(tt_strerror(error)) {}

	[[nodiscard]]
	const char * what() const noexcept override { return msg_; }
};

void check_error(int error);

page from_nos_html(const std::string &html);
page from_nos_json(const std::string &json);
page from_nos_api(int page_no, int sub_no);

}; /* namespace */
#endif /* C++ */
#endif /* header guard */
