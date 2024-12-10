#include <iostream>
#include "joj/logger.h"

int main()
{
    JINFO("Working!");
    JDEBUG("Working!");
    JWARN("Working!");
    JERROR(joj::ErrorCode::FAILED, "Working!");
    JFATAL(joj::ErrorCode::FAILED, "Working!");
    return 0;
}
