# Ideas

## Add raw strings

Example `raw" ... "war`, and the template mode

- Will keep only `'\\'` and `'\n'`

## Add print formats

As I will use fmt, I can use the extra tokens

- `PRINT_FMT_START`,`@%`
- `PRINT_FMT_STR`
- `PRINT_FMT_END`,`%@`

As long as `PRINT_FMT_START` suceeds a `STR_EXPR_END`, mandatorily