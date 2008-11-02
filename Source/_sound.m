#import <Cocoa/Cocoa.h>

#include "_sound.h"

void* _load_sound(const char* name) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  void *result = [[NSSound soundNamed: [NSString stringWithUTF8String: name]] retain];
  [pool release];
  return result;
}

void _play_sound(void* sound) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  [(NSSound*)sound play];
  [pool release];
}
