#ifndef _JOJ_GUI_TEST_H
#define _JOJ_GUI_TEST_H

#include "joj/application/app.h"

class GUITest : public joj::App
{
public:
    GUITest();
    ~GUITest();

    void init() override;
    void update(const f32 dt) override;
    void draw() override;
    void shutdown() override;

    // ---------------------------------------------------
};

#endif // _JOJ_GUI_TEST_H