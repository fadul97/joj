#ifndef _JOJ_WIN32_WIDGET_FACTORY_H
#define _JOJ_WIN32_WIDGET_FACTORY_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/gui/jwidget_factory.h"
#include <Windows.h>

namespace joj
{
    struct ParentData
    {
        HWND parent_handle;
        HINSTANCE instance;
    };

    class JOJ_API JWin32WidgetFactory : public JWidgetFactory
    {
    public:
        JWin32WidgetFactory();
        JWin32WidgetFactory(const ParentData& parent_data);
        ~JWin32WidgetFactory() = default;

        JButton* create_button(i32 x, i32 y, i32 width, i32 height,
            const std::string& label, const JGUIEvent::Callback& callback = nullptr) override;

    private:
        ParentData m_parent_data;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_WIN32_WIDGET_FACTORY_H