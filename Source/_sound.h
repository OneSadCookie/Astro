#ifndef _SOUND_H
#define _SOUND_H

#if defined(__cplusplus)
extern "C" {
#endif

void* _load_sound(const char* name);
void  _play_sound(void* sound);

#if defined(__cplusplus)
}
#endif

#endif
