void tt_buf_append_char(char *buf, size_t buf_sz, size_t *pos, char c);
void tt_buf_append_str(char *buf, size_t buf_sz, size_t *pos, const char *s);
void tt_buf_append_fmt(char *buf, size_t buf_sz, size_t *pos, const char
    *fmt, ...);
int tt_buf_terminate(char *buf, size_t buf_sz, size_t pos);
