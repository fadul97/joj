#ifndef _JOJ_JTEST_H
#define _JOJ_JTEST_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include <string>
#include <functional>
#include <iostream>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <stdexcept>


namespace joj
{
    class JAPI JTest
    {
    public:
        JTest(const std::string& name, const std::function<void()>& func)
            : m_name(name), m_func(func) {}
        ~JTest() = default;

        void run() const;
        void run(u32 index) const;

        std::string get_name() const;

    private:
        std::string m_name;
        std::function<void()> m_func;
    };

    inline void JTest::run() const
    {
        try
        {
            m_func(); // Run test
            std::cout << "\033[32m" << m_name << " PASSED\033[0m\n"; // Green
        }
        catch (const std::exception& e)
        {
            std::cout << "\033[31m" << m_name << " FAILED: " << e.what() << "\033[0m\n"; // Red
        }
    }

    inline void JTest::run(u32 index) const
    {
        // Formatted test name
        std::string formatted_name = "[" + std::to_string(index + 1) + "] " + m_name;

        // Define the minimum width for the test name
        const size_t name_width = 50;
        const size_t status_width = 10;

        // Display the test name aligned to the left
        std::cout << std::left << std::setw(name_width) << formatted_name;

        try
        {
            m_func(); // Run test
            // Display "PASSED" aligned to the right
            std::cout << std::right << std::setw(status_width) << "\033[32mPASSED\033[0m" << "\n";
        }
        catch (const std::exception& e)
        {
            // Display failure with the exception message
            std::cout << std::right << std::setw(status_width) << "\033[31mFAILED: " << e.what() << "\n";
        }
        // Unknown exceptions
        catch (...)
        {
            // Display "FAILED: Unkown exception" aligned to the right
            std::cout << std::right << std::setw(status_width) << "\033[31mFAILED: Unkown exception\033[0m" << "\n";
        }
    }

    inline std::string JTest::get_name() const
    { return m_name; }

}

#endif // _JOJ_JTEST_H