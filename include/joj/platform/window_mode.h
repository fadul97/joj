#ifndef _JOJ_WINDOW_MODE_H
#define _JOJ_WINDOW_MODE_H

namespace joj
{
    /**
     * @brief WindowMode describes the different modes a window can be in.
     *
     * Borderless: The window has no border and may not take up the whole screen.
     * Fullscreen: The window takes up the whole screen.
     * Windowed: The window has a border and may not take up the whole screen.
     */
    enum class WindowMode
    {
        Borderless,
        Fullscreen,
        Windowed
    };
}

#endif // _JOJ_WINDOW_MODE_H