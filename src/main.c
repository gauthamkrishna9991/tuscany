/* MIT License
 *
 * Copyright (c) 2022 Goutham Krishna K V
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <glib.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>

struct termios orig_term_attrs;

void disable_raw_mode(void);

void disable_raw_mode(void) {
  // Set the previous terminal attributes back, before updating.
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term_attrs);
}

void enable_raw_mode(void);

void enable_raw_mode(void) {
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

gpointer *run_thread(gpointer*);

gpointer *run_thread(gpointer *_nodata) {
  int maxval = 20;
  int i = 0;
  g_print("TH: Thread started.\n");
  while (i <= maxval) {
    g_print("\r%ds remaining", maxval - i);
    g_usleep(1000 * 1000);
    i += 1;
  }
  g_print("\rTH: Thread ending.\n");
  return NULL;
}

gint
main (gint   argc,
      gchar *argv[])
{
  GError *th_err;
  GThread *thread = NULL;
  char ch = 0;

  enable_raw_mode();

  g_print("MAIN: Starting thread\n");
  // Thread Setup
  thread = g_thread_try_new("thread-test",
                            (GThreadFunc)run_thread,
                            (gpointer*)NULL,
                            &th_err);
  if (thread == NULL) {
    g_printerr("Error in thread creation: %s", th_err->message);
  }

  g_print("To stop gracefully, Click 'q'\n");
  while (read(STDIN_FILENO, &ch, 1 ) == 1 && ch != 'q');

  g_print("MAIN: Waiting for other threads to finish\n");
  g_thread_join(thread);
  g_print("MAIN: Thread finished.\n");
}

