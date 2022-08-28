#include <teletext.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <sysexits.h>
#include <stdio.h>

static const char usage[] =
"usage: teletext service page\n"
"  services: nos";

int
main(int argc, char **argv)
{
	static char buf[256*1024];
	int page_no, ret;
	struct tt_page page;

	if (argc != 3) {
		puts(usage);
		return EX_USAGE;
	}

	page_no = (int)strtol(argv[2], NULL, 10);
	if (page_no < 0 || page_no > UINT16_MAX)
		errx(1, "bad page number: %s", argv[2]);

	if (strcmp(argv[1], "nos") == 0)
		ret = tt_page_from_nos_api(page_no, 1, &page);
	else
		errx(1, "unkown service: %s", argv[1]);

	if (ret)
		errx(1, "input error: %s", tt_strerror(ret));

	ret = tt_page_to_ansi(&page, buf, sizeof(buf));
	if (ret)
		errx(1, "input error: %s", tt_strerror(ret));

	fputs(buf, stdout);
	return 0;
}
