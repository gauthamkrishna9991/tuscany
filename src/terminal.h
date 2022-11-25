#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

static struct termios orig_term_attrs;

void disable_raw_mode(void);

void enable_raw_mode(struct termios);

