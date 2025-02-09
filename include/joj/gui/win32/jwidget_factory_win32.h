#ifndef _JOJ_WIN32_WIDGET_FACTORY_H
#define _JOJ_WIN32_WIDGET_FACTORY_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#if JPLATFORM_WINDOWS

#include "gui/jwidget_factory.h"
#include <Windows.h>

namespace joj
{
    struct ParentData
    {
        HWND parent_handle;
        HINSTANCE instance;
    };

    class JWin32WidgetFactory : public JWidgetFactory
    {
    public:
        JWin32WidgetFactory();
        JWin32WidgetFactory(const ParentData& parent_data);
        ~JWin32WidgetFactory() = default;

        JButton* create_button(i32 x, i32 y, i32 width, i32 height,
            const std::string& label, const JEvent::Callback& callback = nullptr) override;

    private:
        ParentData m_parent_data;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_WIN32_WIDGET_FACTORY_H