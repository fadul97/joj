#ifndef _JOJ_JTEST_MACROS_H
#define _JOJ_JTEST_MACROS_H

#include "jtest_suite.h"

#define JTEST_SUITE(name)                                                             \
    namespace name {                                                                  \
        void runTests();                                                              \
        static struct Register_##name {                                               \
            Register_##name() {                                                       \
                joj::JTestSuite::instance().add_test(joj::JTest(#name, runTests));    \
            }                                                                         \
        } register_##name;                                                            \
    }                                                                                 \
    void name::runTests()

#define JOJ_TEST(name)                                                                \
    void name();                                                                      \
    static struct Register_##name {                                                   \
        Register_##name() {                                                           \
            joj::JTestSuite::instance().add_test(joj::JTest(#name, name));            \
        }                                                                             \
    } register_##name;                                                                \
    void name()

#endif // _JOJ_JTEST_MACROS_H