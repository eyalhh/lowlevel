#include <elfparser/colors.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mytypes.h>

static b8 g_use_color = 0;


void color_init(void) {

    const char *no_color = getenv("NO_COLOR");
    const char *term = getenv("TERM");
    g_use_color = isatty(STDOUT_FILENO) && (no_color == NULL || no_color[0] == '\0') && (term == NULL || strcmp(term, "dumb") != 0);

}

const char *c_cyan(void)   { return g_use_color ? "\x1b[36m"   : ""; }  /* cyan   */
const char *c_green(void)   { return g_use_color ? "\x1b[32m"   : ""; }  /* green  */
const char *c_yellow(void)  { return g_use_color ? "\x1b[33m"   : ""; }  /* yellow */
const char *c_blue(void)   { return g_use_color ? "\x1b[34m"   : ""; }  /* blue   */
const char *c_dim(void)    { return g_use_color ? "\x1b[2m"    : ""; }  /* dim    */
const char *c_red(void)   { return g_use_color ? "\x1b[1;31m" : ""; }  /* bold red */
const char *c_reset(void)  { return g_use_color ? "\x1b[0m"    : ""; }
