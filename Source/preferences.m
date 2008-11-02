#import "preferences.h"

#import <Foundation/Foundation.h>

//int getBooleanPreference(const char* key, int d/*efault*/) {
//  return d;
//}

long  getIntegerPreference(const char* key, long d/*efault*/) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  
  NSString* k = [NSString stringWithUTF8String: key];
  NSUserDefaults* prefs = [NSUserDefaults standardUserDefaults];
  NSDictionary *dv = [NSDictionary dictionaryWithObject: [NSNumber numberWithLong: d]
                                   forKey: k];
  [prefs registerDefaults: dv];
  long result = [prefs integerForKey: k];
  
  [pool release];
  return result;
}

//float getFloatPreference(const char* key, float d/*efault*/) {
//  return d;
//}

char* getStringPreference(const char* key, char* buffer, unsigned int bufsize) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  
  NSString* v;
  NSString* k = [NSString stringWithUTF8String: key];
  NSUserDefaults* prefs = [NSUserDefaults standardUserDefaults];
  NSDictionary *dv = [NSDictionary dictionaryWithObject: [NSString stringWithUTF8String: buffer]
                                   forKey: k];
  [prefs registerDefaults: dv];
  
  v = [prefs objectForKey: k];
  [v getCString: buffer maxLength: bufsize encoding: NSUTF8StringEncoding];
  
  [pool release];
  return buffer;
}

//void  putBooleanPreference(const char* key, int value) {
//}

void  putIntegerPreference(const char* key, long value) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  
  [[NSUserDefaults standardUserDefaults] setInteger: value
                                         forKey: [NSString stringWithUTF8String: key]];
  
  [pool release];
}

//void  putFloatPreference(const char* key, float value) {
//}

void  putStringPreference(const char* key, const char* value) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  
  [[NSUserDefaults standardUserDefaults] setObject: [NSString stringWithUTF8String: value]
                                         forKey: [NSString stringWithUTF8String: key]];
  
  [pool release];
}

void  writePreferencesToDisk() {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  
  [[NSUserDefaults standardUserDefaults] synchronize];
  
  [pool release];
}
