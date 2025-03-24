#ifndef _JOJ_COLOR_H
#define _JOJ_COLOR_H

#include "defines.h"

namespace joj
{
    /**
     * @brief Helper RGB color structure.
     * @note values from 0 to 255.
     */
    struct IntColor
    {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    };

    /**
     * @brief Helper RGB color structure.
     * @note values from 0.0 to 1.0.
     */
    struct FloatColor
    {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };

    /**
     * @brief RGB color Class.
     */
    class JOJ_API Color
    {
    public:
        /**
         * @brief Construct a new Color object with a purple color.
         * 
         */
        Color();
        
        /**
         * @brief Construct a new Color object.
         * 
         * @param r The red color value.
         * @param g The green color value.
         * @param b The blue color value.
         */
        Color(const f32 r, const f32 g, const f32 b, const f32 a = 1.0f);

        /**
         * @brief Convert the color to IntColor.
         * 
         * @return The IntColor object.
         */
        IntColor to_IntColor() const;

        /**
         * @brief Convert the color to FloatColor.
         * 
         * @return The FloatColor object.
         */
        FloatColor to_FloatColor() const;

        /**
         * @brief Get the Red channel value.
         * 
         * @return The red channel value.
         */
        f32 get_r() const;

        /**
         * @brief Get the Green channel value.
         * 
         * @return The green channel value.
         */
        f32 get_g() const;

        /**
         * @brief Get the Blue channel value.
         * 
         * @return The blue channel value.
         */
        f32 get_b() const;

        /**
         * @brief Get the Alpha channel value.
         * 
         * @return The alpha channel value.
         */
        f32 get_a() const;

        static const Color White;
        static const Color Black;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        
    private:
        /** @brief The Red channel value. */
        f32 m_r;

        /** @brief The Green channel value. */
        f32 m_g;

        /** @brief The Blue channel value. */
        f32 m_b;

        /** @brief The Alpha channel value. */
        f32 m_a;
    };
}

#endif // _JOJ_COLOR_H