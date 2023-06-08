#include <teletext.h>
#include <string.h>
#include <err.h>
#include <sysexits.h>
#include <stdio.h>

static const char usage[] =
"usage: teletext-convert in-format out-format [in-file] [out-file]\n"
"  input formats:  nos-html nos-json \n"
"  output formats: nos-html xml ascii ansi";

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
	static char in_buf[16*1024];
	static char out_buf[256*1024];
	static struct tt_page page;

	int ret;
	char *input_format = NULL;
	char *output_format = NULL;
	char *in_path = NULL;
	char *out_path = NULL;

	if (argc < 3 || argc > 5) {
		puts(usage);
		return EX_USAGE;
	}

	input_format = argv[1];
	output_format = argv[2];

	if (argc > 3)
		in_path = argv[3];
	if (argc > 4)
		out_path = argv[4];

	read_file(in_path, in_buf, sizeof(in_buf));

	if (!strcmp(input_format, "nos-html"))
		ret = tt_page_from_nos_html(in_buf, &page);
	else if (!strcmp(input_format, "nos-json"))
		ret = tt_page_from_nos_json(in_buf, &page);
	else
		errx(EX_USAGE, "unknown input format: %s", input_format);

	if (ret)
		errx(1, "parsing error: %s", tt_strerror(ret));

	if (!strcmp(output_format, "nos-html"))
		ret = tt_page_to_nos_html(&page, out_buf, sizeof(out_buf));
	else if (!strcmp(output_format, "xml"))
		ret = tt_page_to_xml(&page, out_buf, sizeof(out_buf));
	else if (!strcmp(output_format, "ascii"))
		ret = tt_page_to_ascii(&page, out_buf, sizeof(out_buf));
	else if (!strcmp(output_format, "ansi"))
		ret = tt_page_to_ansi(&page, out_buf, sizeof(out_buf));
	else
		errx(EX_USAGE, "unknown output format: %s", output_format);

	if (ret)
		errx(1, "output error: %s", tt_strerror(ret));

	write_file(out_path, out_buf);
	return 0;
}
