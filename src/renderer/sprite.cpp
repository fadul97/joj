#include "renderer/sprite.h"

#include "core/jmacros.h"

joj::Sprite::Sprite()
    : m_current_animation(nullptr),
    m_current_frame_index(0), m_time_since_last_frame(0.0f)
{
    m_time_since_last_frame = 5.0f;
}

joj::Sprite::~Sprite()
{
}

void joj::Sprite::add_animation(const SpriteAnimationData& animation)
{
    m_animations[animation.name] = animation;
}

void joj::Sprite::play_animation(const std::string& animation_name)
{
    // Verifica se a animação solicitada existe.
    if (m_current_animation != nullptr && m_current_animation->name == animation_name)
        return; // Se já estamos na animação solicitada, não faz nada.

    if (m_animations.find(animation_name) != m_animations.end())
    {
        m_current_animation = &m_animations[animation_name];
        m_current_frame_index = 0;
        m_time_since_last_frame = 0.0f;
    }
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