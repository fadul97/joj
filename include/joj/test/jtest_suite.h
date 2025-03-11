#ifndef _JOJ_JTEST_SUITE_H
#define _JOJ_JTEST_SUITE_H

#include "core/defines.h"

#include <vector>
#include "jtest.h"

namespace joj
{
    class JAPI JTestSuite
    {
    public:
        static JTestSuite& instance();

        void add_test(const JTest& test);
        void run() const;

    private:
        JTestSuite() = default;
        ~JTestSuite() = default;

        std::vector<JTest> m_tests;
    };

    inline JTestSuite& JTestSuite::instance()
    {
        static JTestSuite instance;
        return instance;
    }

    inline void JTestSuite::add_test(const JTest& test)
    {
        m_tests.push_back(test);
    }

    inline void JTestSuite::run() const
    {
        u32 failed_tests = 0;
        for (u32 i = 0; i < m_tests.size(); ++i)
        {
            try
            {
                m_tests[i].run(i);
            }
            catch (...)
            {
                std::cout << "Unknown error occurred during test " << m_tests[i].get_name() << std::endl;
                ++failed_tests;
            }
        }

        std::cout << "Total tests failed: " << failed_tests << std::endl;
    }
}
#endif // _JOJ_JTEST_SUITE_H