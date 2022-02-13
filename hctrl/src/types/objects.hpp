#pragma once

namespace types
{

    class Owned
    {
    public:
        Owned() : _last_id(nullptr) {}
    private:
        void* _last_id;
    };

} /* namespace types */