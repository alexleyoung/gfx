#define SOKOL_APP_IMPL
#define SOKOL_GLCORE
#define SOKOL_NO_ENTRY
#include "vendor/sokol/sokol_app.h"

void init(void) {}
void frame(void) {}
void cleanup(void) {}
void event(const sapp_event *ev) {}

int main() {
  sapp_run(&(sapp_desc){
      .init_cb = init,
      .frame_cb = frame,
      .cleanup_cb = cleanup,
      .event_cb = event,
  });

  return 1;
}
