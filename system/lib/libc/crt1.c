/*
 * Copyright 2019 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

// Much of this code comes from:
// https://github.com/CraneStation/wasi-libc/blob/master/libc-bottom-half/crt/crt1.c
// Converted malloc() calls to alloca() to avoid including malloc in all programs.

#include <stdlib.h>
#include <wasi/api.h>

__attribute__((weak)) void __wasm_call_ctors(void);

__attribute__((weak)) int __main_argc_argv(int argc, char** argv);
__attribute__((weak)) int main(int argc, char** argv);

int __original_main(void);

void _start(void) {
  if (__wasm_call_ctors) {
    __wasm_call_ctors();
  }

  if (!main && !__main_argc_argv) {
    return;
  }

  /*
   * Will either end up call the user's original zero argument main directly
   * or our __original_main fallback in __original_main.c which handles
   * populating argv.
   */
  int r = __original_main();

  /* If main exited successfully, just return, otherwise call _Exit.
   * TODO(sbc): switch to _Exit */
  if (r != 0) {
    __wasi_proc_exit(r);
  }
}
