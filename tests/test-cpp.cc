#include <teletext.h>
#include <fstream>
#include <iostream>

int
main()
{
	std::ifstream json_stream("../samples/nos.json");
	std::ifstream html_stream("../samples/nos.html");

	std::string json(std::istreambuf_iterator<char>(json_stream), {});
	std::string html(std::istreambuf_iterator<char>(html_stream), {});

	auto page1 = teletext::from_nos_json(json);
	auto page2 = teletext::from_nos_html(html);

	std::cout << page1.to_ansi() << std::endl;
	std::cout << page2.to_ascii() << std::endl;

	return 0;
}
