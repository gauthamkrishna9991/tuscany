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
#include "terminal.h"

#include <glib.h>

gpointer *run_thread(gboolean*);

gpointer *run_thread(gboolean *flag) {
  gint i = 1;
  g_print("TH: Thread started.\n");
  while (!(*flag)) {
    g_print("\rTH: REPEAT %d", i);
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
  // Flag to stop the thread
  gboolean stop_flag = FALSE;
  // GError to catch errors
  GError *th_err;
  // Thread to switch to
  GThread *thread = NULL;
  // Character to terminal keys from
  char ch = 0;

  enable_raw_mode(orig_term_attrs);

  g_print("MAIN: Starting thread\n");
  // Thread Setup
  thread = g_thread_try_new("thread-test",
                            (GThreadFunc)run_thread,
                            &stop_flag,
                            &th_err);
  // If in any case the thread doesn't start
  if (thread == NULL) {
    g_printerr("Error in thread creation: %s", th_err->message);
    return EXIT_FAILURE;
  }

  g_print("To stop gracefully, Click 'q'\n");
  while (read(STDIN_FILENO, &ch, 1 ) == 1 && ch != 'q');
  stop_flag = TRUE;

  g_print("\nMAIN: Waiting for other threads to finish\n");
  g_thread_join(thread);
  g_print("MAIN: Thread finished.\n");
}

