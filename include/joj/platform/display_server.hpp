#ifndef _JOJ_DISPLAY_SERVER_HPP
#define _JOJ_DISPLAY_SERVER_HPP

#include "joj/core/typedefs.h"
#include "joj/core/types.h"

#include "joj/core/error/error_code.hpp"

struct xcb_connection_t;

namespace joj {

// struct DisplayServerData;
struct DisplayServerData {
    xcb_connection_t* connection{ nullptr };
    u32 handle{ 0 };
};

struct DisplayServer {
    DisplayServerData* data{ nullptr };
};

JOJ_API ErrorCode display_server_create(DisplayServer* const display_server);
JOJ_API void display_server_destroy(DisplayServer* const display_server);
JOJ_API b8 display_server_process_events(DisplayServer* const display_server);

} // namespace joj

#endif // _JOJ_DISPLAY_SERVER_HPP
