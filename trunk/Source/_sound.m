#import <Cocoa/Cocoa.h>

#include "_sound.h"

void* _load_sound(const char* name) {
  return [[NSSound soundNamed: [NSString stringWithCString: name]] retain];
}

void _play_sound(void* sound) {
  [(NSSound*)sound play];
}
