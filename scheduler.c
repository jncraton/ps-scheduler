#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

/* Adjust DEBUG_TICKS to change the number of ticks printed to stdout */
#define DEBUG_TICKS 10

const unsigned char RUNNABLE = 1;
const unsigned char RUNNING = 2;
const unsigned char WAITING = 3;

typedef struct {
  unsigned char weight;
  unsigned char state;
  unsigned int accumulated_runtime;
} Thread;

#define NUM_THREADS 6
Thread threads[NUM_THREADS] = {
  {50, 1, 0},
  {25, 1, 0},
  {12, 1, 0},
  {8, 1, 0},
  {4, 1, 0},
  {1, 1, 0},
};

Thread *schedule_lottery(Thread * threads) {
  /*
   * Implements a lottery scheduler
   *
   * Considers thread weights to determine which thread to run
   * 
   * Returns a pointer to the thread to run next
   * 
   * Should return `NULL` if there is no eligible thread to run
   */

  return &threads[0];
}

Thread *schedule_wfq(Thread * threads) {
  /*
   * Implements a weighted fair queueing scheduler
   *
   * Considers thread weights to determine which thread to run
   * 
   * Returns a pointer to the thread to run next
   * 
   * Should return `NULL` if there is no eligible thread to run
   */

  return &threads[0];
}

/*
 * Test code
 *
 * The following code may help understand the task, but it should not 
 * be modified
 *
 */

void run(Thread * thread) {
  /*
   * Simulates running the provided thread for 1 tick
   */

  assert(thread != NULL);
  assert(thread->state == RUNNABLE);

  thread->state = RUNNING;
  thread->accumulated_runtime += 1;

  if (rand() % 100 < 99) {
    thread->state = RUNNABLE;
  } else {
    thread->state = WAITING;
  }
}

int main() {
  /*
   * Simulate scheduling a number of ticks
   */
  const unsigned int ticks = 100000;
  srand(time(NULL));

  // Lottery tests 
  for (unsigned time = 0; time < ticks; time++) {
    Thread *next_thread = schedule_lottery(threads);

    run(next_thread);

    int thread_num = (next_thread - threads);

    if (time < DEBUG_TICKS) {
      printf("Lottery: Thread %d ran for 1 tick (%d work completed)\n", thread_num, next_thread->accumulated_runtime);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
      if (&threads[i] != next_thread)
        threads[i].state = RUNNABLE;
    }
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    int error = threads[i].accumulated_runtime - ((ticks * threads[i].weight) / 100.0);
    printf("Lottery: Thread %d: Ran for %d ticks (off by %d)\n", i, threads[i].accumulated_runtime, error);
    assert(error < 500 && error > -500);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    threads[i].accumulated_runtime = 0;
    threads[i].state = WAITING;
  }
  Thread *next_thread = schedule_lottery(threads);
  if (next_thread != NULL) {
    printf("Lottery: Scheduler does not return NULL when nothing is runnable.\n");
    exit(1);
  }

  printf("Lottery: Scheduler probably works correctly.\n");

  // Reset

  for (int i = 0; i < NUM_THREADS; i++) {
    threads[i].accumulated_runtime = 0;
    threads[i].state = RUNNABLE;
  }

  // WFQ Tests

  for (unsigned time = 0; time < ticks; time++) {
    Thread *next_thread = schedule_wfq(threads);

    run(next_thread);

    int thread_num = (next_thread - threads);

    if (time < DEBUG_TICKS) {
      printf("WFQ: Thread %d ran for 1 tick (%d work completed)\n", thread_num, next_thread->accumulated_runtime);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
      if (&threads[i] != next_thread)
        threads[i].state = RUNNABLE;
    }
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    int error = threads[i].accumulated_runtime - ((ticks * threads[i].weight) / 100.0);
    printf("WFQ: Thread %d: Ran for %d ticks (off by %d)\n", i, threads[i].accumulated_runtime, error);
    assert(error == 0);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    threads[i].accumulated_runtime = 0;
    threads[i].state = WAITING;
  }
  next_thread = schedule_wfq(threads);
  if (next_thread != NULL) {
    printf("WFQ: Scheduler does not return NULL when nothing is runnable.\n");
    exit(1);
  }

  printf("WFQ: Scheduler works correctly.\n");

  printf("Tests complete.\n");

  return 0;
}
