#include "terminal.h"

void disable_raw_mode(void) {
  // Set the previous terminal attributes back, before updating.
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term_attrs);
}

void enable_raw_mode(struct termios _term) {
  struct termios term_attrs;
  // Load the terminal attributes to "orig_term_attrs"
  tcgetattr(STDIN_FILENO, &orig_term_attrs);
  // Let's tell the program to run "disable_raw_mode" on exit so we don't lock
  // the terminal after the program exit.
  atexit(disable_raw_mode);

  // Termios terminal attributes, set as "orig" ones.
  term_attrs = orig_term_attrs;

  // --- EXPLANATION OF STATEMENT IN NEXT LINE ---
  //
  // There's two things happening.
  // First, ECHO is the 4th last bit in the 32-bit terminal attribute flag,
  // which is "00000000000000000000000000001000"
  // Second, the '~' operator is "bit-wise NOT", which will convert it to
  // "11111111111111111111111111110111", which on bit-wise and with a terminal
  // attribute will just disable echo for terminal, as every other bit will
  // retain it's current values.
  //
  //
  // --- ABOUT term_attrs.c_lflag STRUCTURE ---
  // The `c_lflag` is for "local_flags". Consider it as miscellaneous flags
  // after `c_iflags` (input), `c_oflags` (output), and `c_cflag` (control).
  term_attrs.c_lflag &= ~(ECHO | ICANON);
  // Set the flags
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_attrs);
}
