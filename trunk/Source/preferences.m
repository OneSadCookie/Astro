#import "preferences.h"

#import <Foundation/Foundation.h>

//int getBooleanPreference(const char* key, int d/*efault*/) {
//  return d;
//}

long  getIntegerPreference(const char* key, long d/*efault*/) {
  NSString* k = [NSString stringWithCString: key];
  NSUserDefaults* prefs = [NSUserDefaults standardUserDefaults];
  NSDictionary *dv = [NSDictionary dictionaryWithObject: [NSNumber numberWithLong: d]
                                   forKey: k];
  [prefs registerDefaults: dv];
  return [prefs integerForKey: k];
}

//float getFloatPreference(const char* key, float d/*efault*/) {
//  return d;
//}

char* getStringPreference(const char* key, char* buffer, unsigned int bufsize) {
  NSString* v;
  NSString* k = [NSString stringWithCString: key];
  NSUserDefaults* prefs = [NSUserDefaults standardUserDefaults];
  NSDictionary *dv = [NSDictionary dictionaryWithObject: [NSString stringWithCString: buffer]
                                   forKey: k];
  [prefs registerDefaults: dv];
  
  v = [prefs objectForKey: k];
  [v getCString: buffer maxLength: bufsize - 1];
  
  return buffer;
}

//void  putBooleanPreference(const char* key, int value) {
//}

void  putIntegerPreference(const char* key, long value) {
  [[NSUserDefaults standardUserDefaults] setInteger: value
                                         forKey: [NSString stringWithCString: key]];
}

//void  putFloatPreference(const char* key, float value) {
//}

void  putStringPreference(const char* key, const char* value) {
  [[NSUserDefaults standardUserDefaults] setObject: [NSString stringWithCString: value]
                                         forKey: [NSString stringWithCString: key]];
}

void  writePreferencesToDisk() {
  [[NSUserDefaults standardUserDefaults] synchronize];
}
