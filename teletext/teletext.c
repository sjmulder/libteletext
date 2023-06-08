#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 /* unix */
#include <sysexits.h>
#include <getopt.h>
#include <err.h>
 /* libs */
#include <teletext.h>

static const char usage[] =
"usage:\n"
"  teletext -i service\n"
"  teletext [-T out_fmt] [-o outfile] service page\n"
"  teletext [-T out_fmt] [-o outfile] -F in_fmt -f infile\n"
"services:       nos\n"
"input formats:  nos-html nos-json auto\n"
"output formats: nos-html xml ascii ansi\n";

static size_t
read_file(const char *path, char *buf, size_t buf_sz)
{
	FILE *file;
	size_t len;

	if (path == NULL || !strcmp(path, "-")) {
		file = stdin;
		path = "<stdin>";
	} else if (!(file = fopen(path, "r")))
		err(1, "%s", path);

	len = fread(buf, 1, buf_sz, file);
	if (len == buf_sz)
		errx(1, "%s: file too large", path);

	buf[len] = '\0';
	fclose(file);

	return len;
}

static void
write_file(const char *path, char *buf)
{
	FILE *file;

	if (path == NULL || !strcmp(path, "-")) {
		file = stdout;
		path = "<stdout>";
	} else if (!(file = fopen(path, "w")))
		err(1, "%s", path);

	if (fputs(buf, file) == EOF)
		warn("%s", path);

	fclose(file);
}

int
main(int argc, char **argv)
{
	static char buf[256*1024];
	struct tt_page page;
	int page_no, ret, c;
	 /* options */
	char *in_fmt="auto", *in_path=NULL;
	char *out_fmt="ansi", *out_path="-";
	char *service=NULL, *page_str=NULL;

	while ((c = getopt(argc, argv, "T:o:F:f:")) != -1)
		switch (c) {
		case 'T': out_fmt  = optarg; break;
		case 'o': out_path = optarg; break;
		case 'F': in_fmt   = optarg; break;
		case 'f': in_path  = optarg; break;
		default:
			exit(EX_USAGE);
		}
	
	argc -= optind;
	argv += optind;

	if (in_path) {
		if (argc) {
			warnx("too many arguments");
			fputs(usage, stderr);
			exit(EX_USAGE);
		}

		if (!in_fmt) {
			warnx("no input format specified");
			fputs(usage, stderr);
			exit(EX_USAGE);
		}

		read_file(in_path, buf, sizeof(buf));

		if (!strcmp(in_fmt, "nos-html"))
			ret = tt_page_from_nos_html(buf, &page);
		else if (!strcmp(in_fmt, "nos-json"))
			ret = tt_page_from_nos_json(buf, &page);
		else
			errx(EX_USAGE, "unknown input format: %s",
			    in_fmt);

		if (ret)
			errx(1, "parsing error: %s", tt_strerror(ret));
	} else {
		if (argc != 2) {
			if (argc)
				warnx("bad number of arguments");
			fputs(usage, stderr);
			exit(EX_USAGE);
		}

		service = argv[0];
		page_str = argv[1];

		page_no = (int)strtol(page_str, NULL, 10);
		if (page_no < 0 || page_no > UINT16_MAX)
			errx(1, "bad page number: %s", page_str);

		if (strcmp(service, "nos") == 0)
			ret = tt_page_from_nos_api(page_no, 1, &page);
		else
			errx(1, "unkown service: %s", service);

		if (ret)
			errx(1, "input error: %s", tt_strerror(ret));
	}

	if (!strcmp(out_fmt, "nos-html"))
		ret = tt_page_to_nos_html(&page, buf, sizeof(buf));
	else if (!strcmp(out_fmt, "xml"))
		ret = tt_page_to_xml(&page, buf, sizeof(buf));
	else if (!strcmp(out_fmt, "ascii"))
		ret = tt_page_to_ascii(&page, buf, sizeof(buf));
	else if (!strcmp(out_fmt, "ansi"))
		ret = tt_page_to_ansi(&page, buf, sizeof(buf));
	else
		errx(EX_USAGE, "unknown output format: %s", out_fmt);

	if (ret)
		errx(1, "output error: %s", tt_strerror(ret));

	write_file(out_path, buf);
	return 0;
}
