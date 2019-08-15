#include <stdio.h>
#include <assert.h>
#include <threads.h>

once_flag flag = ONCE_FLAG_INIT;
_Atomic int counter = 0;

void do_once(void) {
  counter++;
  printf("in do_once\n");
}

int run(void* arg)
{
  printf("in run %p\n", thrd_current());
  call_once(&flag, do_once);
  printf("done run\n");
  return 42;
}

/*
int run_with_exit() {
  thrd_yield();
  thrd_exit(43);
}
*/

int main()
{
  int result = 0;
  printf("thrd_current: %p", thrd_current());

  // Test call_once
  thrd_t t1;
  thrd_t t2;
  thrd_t t3;
  thrd_t t4;

  thrd_create(&t1, run, NULL);
  thrd_create(&t2, run, NULL);
  thrd_create(&t3, run, NULL);
  thrd_create(&t4, run, NULL);

  assert(!thrd_equal(t1, t2));
  assert(thrd_equal(t1, t1));

  thrd_join(t1, &result);
  thrd_join(t2, &result);
  thrd_join(t3, &result);
  thrd_join(t4, &result);
  assert(result == 42);
  assert(counter == 1);

  // Test thrd_exit return value
  /*
  thrd_t t5;
  thrd_create(&t1, run_with_exit, NULL);
  thrd_join(t5, &result);
  assert(result == 43);
  */

  // Test thrd_detach
  thrd_t t6;
  thrd_create(&t6, run, NULL);
  thrd_detach(t6);

#ifdef REPORT_RESULT
  REPORT_RESULT(0);
#endif
  return 0;
}
