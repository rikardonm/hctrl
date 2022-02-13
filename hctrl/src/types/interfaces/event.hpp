#pragma once

#include <cstdint>

namespace interfaces
{
namespace Event
{

    using ProducerID = uint32_t;

    struct Message
    {
        ProducerID producer_id;
        union {
            uint32_t data;
            void* pdata;
        };
    };

    class IConsumer
    {
    public:
        virtual bool StoreIncomingMessage(Message message) = 0;
        virtual ~IConsumer() = default;
        using ptr = IConsumer*;
    };

} /* namespace Event */
} /* namespace interfaces */
