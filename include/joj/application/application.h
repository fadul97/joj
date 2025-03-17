#ifndef _JOJ_APPLICATION_H
#define _JOJ_APPLICATION_H

#include "joj/core/defines.h"

extern "C"
{
    const char* JOJ_CALLBACK_NAME_ON_START = "onStart";
    const char* JOJ_CALLBACK_NAME_ON_UPDATE = "onUpdate";
    const char* JOJ_CALLBACK_NAME_ON_SHUTDOWN = "onShutdown";

    typedef void (*JoJCallbackOnStart)();
    typedef void (*JoJCallbackOnUpdate)(f32);
    typedef void (*JoJCallbackOnShutdown)();

    JOJ_GAME_API void onStart();
    JOJ_GAME_API void onUpdate(const f32 dt);
    JOJ_GAME_API void onShutdown();
}

#endif // _JOJ_APPLICATION_H