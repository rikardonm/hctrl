
#pragma once

#include <cstdint>
#include <cstddef>


namespace types
{

    using CallbackId_t = uint32_t;
    using IsrCallback_t = void (*)(CallbackId_t id);
    using FreeFunctionIsr = void (*)(void);



    /**
     * @brief
     *
     */
    using ReturnCode_t = uint32_t;
    enum class ReturnCode : ReturnCode_t
    {
        /* General Errors */
        Success                 = 0x00000000,
        UnknownError            = 0x00000001,
        Timeout                 = 0x00000002,
        /* Argument Errors */
        InvalidArguments        = 0x00000100,
        OutOfRange              = 0x00000101,
        /* Resource Errors */
        ResourceUnavailable     = 0x00000200,
    };


} /* namespace types */
