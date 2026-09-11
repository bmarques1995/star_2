# Ideas

## Add raw strings

Example `raw" ... "war`, and the template mode

- Will keep only `'\\'` and `'\n'`

## Add print formats

As I will use fmt, I can use the extra tokens

- `PRINT_FMT_START`,`@{`
- `PRINT_FMT_STR`
- `PRINT_FMT_END`,`}@`

As long as `PRINT_FMT_START` suceeds a `STR_EXPR_END`, mandatorily

## Add promote, demote, truncate and cast as builtin functions

## Add builtin promotion and alert demotion

## Add exit as builtin function

## Fix arrow not moving on REPL at linux and freebsd

## Github workflow

- `wix eula accept wix7`: this will be required to install wix on github workflow, as it will not be possible to accept the eula interactively
- `wix extension add WixToolset.UI.wixext`: this will be required to add the wix extension to the workflow, as it will not be possible to add it interactively
- `wix extension add WixToolset.Util.wixext`: this will be required to add the wix extension to the workflow, as it will not be possible to add it interactively