#include "renderer/sprite.h"

#include "jmacros.h"

joj::Sprite::Sprite()
    : m_current_animation(nullptr),
    m_current_frame_index(0), m_time_since_last_frame(0.0f)
{
}

joj::Sprite::~Sprite()
{
}

void joj::Sprite::play_animation(const std::string& animation_name)
{
    JTODO();
}

void joj::Sprite::update(const f32 dt)
{
    if (m_current_animation == nullptr)
        return;

    m_time_since_last_frame += dt;

    if (m_time_since_last_frame >= m_current_animation->frameDuration)
    {
        ++m_current_frame_index;

        if (m_current_frame_index >= m_current_animation->frames.size())
            m_current_frame_index = 0;

        m_time_since_last_frame = 0.0f;
    }
}