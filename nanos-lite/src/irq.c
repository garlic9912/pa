#include </home/garlic/ics2023/nanos-lite/include/common.h>

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD:
      printf("hahahahaha\n"); break;
    default: panic("Unhandled event ID = %d\n111111111111111111111", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
