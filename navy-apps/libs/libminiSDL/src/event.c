#include <NDL.h>
#include <SDL.h>
#include <string.h>
#include <stdio.h>

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

  char keyname[10], status[5];
  sscanf("%s %s", keyname, status);
  if (strcmp(status, "DOWN")) event->type = SDL_KEYDOWN;
  if (strcmp(keyname, "J")) event->key.keysym.sym = SDLK_j;
  if (strcmp(keyname, "UP")) event->key.keysym.sym = SDLK_UP;
  if (strcmp(keyname, "K")) event->key.keysym.sym = SDLK_k;
  if (strcmp(keyname, "DOWN")) event->key.keysym.sym = SDLK_DOWN;
  if (strcmp(keyname, "G")) event->key.keysym.sym = SDLK_g;
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
