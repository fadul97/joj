#include "gui/win32/jwidget_factory_win32.h"

#if JPLATFORM_WINDOWS

#include "logger.h"
#include "jmacros.h"

joj::JWin32WidgetFactory::JWin32WidgetFactory()
    : m_parent_data{ nullptr, nullptr }
{
}

joj::JWin32WidgetFactory::JWin32WidgetFactory(const ParentData& parent_data)
{
    m_parent_data = parent_data;
}

std::unique_ptr<joj::JButton> joj::JWin32WidgetFactory::create_button(i32 x, i32 y, i32 width,
    i32 height, const std::string& label, const JEvent::Callback& callback)
{
    auto button = std::make_unique<JButton>(x, y, width, height, label.c_str());
    if (!button.get())
    {
        JFATAL(ErrorCode::ERR_GUI_BUTTON_WIN32_CREATION,
            "Failed to create button.");
    }

    JWidgetCreationData data = { m_parent_data.parent_handle, m_parent_data.instance };
    button->create(data, callback);

    return button;
}

#endif // JPLATFORM_WINDOWS