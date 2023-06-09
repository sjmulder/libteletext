#include <teletext.h>
#include <stdexcept>
#include <array>

using namespace teletext;

void
teletext::check_error(int error)
{
	if (error)
		throw teletext_error(error);
}

[[nodiscard]] std::string
teletext::page::to_xml() const
{
	std::array<char, 4096> buf = {};
	check_error(tt_page_to_xml(&data(), buf.data(), 4096));
	return { buf.data() };
}

[[nodiscard]] std::string
teletext::page::to_ascii() const
{
	std::array<char, 4096> buf = {};
	check_error(tt_page_to_ascii(&data(), buf.data(), 4096));
	return { buf.data() };
}

[[nodiscard]] std::string
teletext::page::to_ansi() const
{
	std::array<char, 4096> buf = {};
	check_error(tt_page_to_ansi(&data(), buf.data(), 4096));
	return { buf.data() };
}

[[nodiscard]] std::string
teletext::page::to_nos_html() const
{
	std::array<char, 4096> buf = {};
	check_error(tt_page_to_nos_html(&data(), buf.data(), 4096));
	return { buf.data() };
}

page
teletext::from_nos_html(const std::string &html)
{
	page page;
	check_error(tt_page_from_nos_html(html.c_str(), &page.data()));
	return page;
}

page
teletext::from_nos_json(const std::string &json)
{
	page page;
	check_error(tt_page_from_nos_json(json.c_str(), &page.data()));
	return page;
}

page
teletext::from_nos_api(int page_no, int sub_no)
{
	page page;
	check_error(tt_page_from_nos_api(page_no, sub_no, &page.data()));
	return page;
}
