#ifndef _JOJ_CPU_ACCESS_TYPE_H
#define _JOJ_CPU_ACCESS_TYPE_H

namespace joj
{
    /**
     * @brief CPUAccessType is an enum class that holds the type of access
     * the CPU has on the buffer.
     * 
     * It is used to set the CPU access type of the buffer.
     * 
     * The CPU access type can be set to:
     * None: The CPU has no access to the buffer.
     * Write: The CPU has write access to the buffer.
     * Read: The CPU has read access to the buffer.
     */
    enum class CPUAccessType
    {
        None,
        Write,
        Read
    };
}

#endif // _JOJ_CPU_ACCESS_TYPE_H