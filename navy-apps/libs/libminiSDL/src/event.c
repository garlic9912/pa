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

int SDL_WaitEvent(SDL_Event *event) {
  char buf[64];
  // Read Error
  if (NDL_PollEvent(buf, sizeof(buf)) == 0) 
    return -1;

  // char status[5];
  // int keycode = 0;
  // sscanf(buf, "%d %s", &keycode, status);

  // // DOWN or UP
  // if (strcmp(status, "DOWN") == 0) event->type = SDL_KEYDOWN;
  // // KeyCode
  // event->key.keysym.sym = SDL_Keys(keycode);
  event->type = SDL_KEYDOWN;
  event->key.keysym.sym = SDLK_J;
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
