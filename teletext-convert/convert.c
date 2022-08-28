#include <teletext.h>
#include <string.h>
#include <getopt.h>
#include <err.h>
#include <sysexits.h>
#include <stdio.h>

static const char usage[] =
"usage: teletext-convert -T in-format -O out-format [in-file] [out-file]\n"
"  input formats:  nos-html\n"
"  output formats: text ansi nos-html";

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
write_file(const char *path, char *buf, size_t buf_len)
{
	FILE *file;

	if (path == NULL || !strcmp(path, "-")) {
		file = stdout;
		path = "<stdout>";
	} else if (!(file = fopen(path, "w")))
		err(1, "%s", path);

	if (fwrite(buf, buf_len, 1, file) != 1)
		warn("%s", path);

	fclose(file);
}

int
main(int argc, char **argv)
{
	static char in_buf[16*1024];
	static char out_buf[16*1024];
	static struct tt_page page;

	int c, ret;
	char *input_format = NULL;
	char *output_format = NULL;
	char *in_path = NULL;
	char *out_path = NULL;

	while ((c = getopt(argc, argv, "?T:O:")) != -1)
		switch (c) {
		case 'T': input_format = optarg; break;
		case 'O': output_format = optarg; break;
		case '?': puts(usage); return 0;
		default: return EX_USAGE;
		}

	if (!input_format)
		errx(EX_USAGE, "missing required -T flag");
	if (!output_format)
		errx(EX_USAGE, "missing required -O flag");

	if (optind < argc)
		in_path = argv[optind];
	if (optind+1 < argc)
		out_path = argv[optind+1];
	if (optind+2 < argc)
		errx(EX_USAGE, "too many arguments");

	read_file(in_path, in_buf, sizeof(in_buf));

	if (!strcmp(input_format, "nos-html"))
		ret = tt_page_from_nos_html(in_buf, &page);
	else
		errx(EX_USAGE, "unknown input format: %s", input_format);

	if (ret)
		errx(1, "parsing error: %s", tt_strerror(ret));

	if (!strcmp(output_format, "text"))
		ret = tt_page_to_ascii(&page, out_buf, sizeof(out_buf));
	else if (!strcmp(output_format, "ansi"))
		ret = tt_page_to_ansi(&page, out_buf, sizeof(out_buf));
	else if (!strcmp(output_format, "nos-html"))
		ret = tt_page_to_nos_html(&page, out_buf, sizeof(out_buf));

	if (ret)
		errx(1, "output error: %s", tt_strerror(ret));

	write_file(out_path, out_buf, strlen(out_buf));
	return 0;
}
