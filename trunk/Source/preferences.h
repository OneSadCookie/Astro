#ifndef PREFERENCES_H
#define PREFERENCES_H

#if defined(__cplusplus)
extern "C" {
#endif

int   getBooleanPreference(const char* key, int d/*efault*/);
long  getIntegerPreference(const char* key, long d/*efault*/);
float getFloatPreference(const char* key,  float d/*efault*/);
char* getStringPreference(const char* key,  char* buffer, unsigned int bufsize);

void  putBooleanPreference(const char* key, int value);
void  putIntegerPreference(const char* key, long value);
void  putFloatPreference(const char* key, float value);
void  putStringPreference(const char* key, const char* value);

void  writePreferencesToDisk();

#if defined(__cplusplus)
}
#endif

#endif
