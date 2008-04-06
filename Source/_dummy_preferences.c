#include "preferences.h"

int   getBooleanPreference(const char* key, int d/*efault*/) { return d; }
long  getIntegerPreference(const char* key, long d/*efault*/) { return d; }
float getFloatPreference(const char* key,  float d/*efault*/) { return d; }
char* getStringPreference(const char* key,  char* buffer, unsigned int bufsize)
{
    buffer[0] = 0;
    return buffer;
}

void  putBooleanPreference(const char* key, int value) {}
void  putIntegerPreference(const char* key, long value) {}
void  putFloatPreference(const char* key, float value) {}
void  putStringPreference(const char* key, const char* value) {}

void  writePreferencesToDisk() {}

