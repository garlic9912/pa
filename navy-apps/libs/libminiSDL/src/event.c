#include <NDL.h>
#include <SDL.h>
#include <string.h>
#include <stdio.h>
#include </home/garlic/ics2023/navy-apps/libs/libminiSDL/include/sdl-event.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  return 0;
}

static int flag = 0;
int SDL_WaitEvent(SDL_Event *event) {
  char buf[64];
  int code = 0;
  code = NDL_PollEvent(buf, sizeof(buf));
  printf("%d\n", code);

  // DOWN or UP
  if (code & 1) event->type = SDL_KEYDOWN;
  // KeyCode
  event->key.keysym.sym = SDL_Keys(code >> 1);
  // event->key.keysym.sym = SDLK_J;
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
