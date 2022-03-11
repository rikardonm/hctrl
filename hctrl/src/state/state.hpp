#pragma once

#include <cstdint>
#include <memory>


namespace state
{
    // struct Vars
    // {
    //     uint32_t counter;
    //     uint32_t timestamp;
    // };

    // class Base
    // {
    // public:
    //     /* Declare the bootstrapper */
    //     Base(Vars& vars) : _vars(vars) {}
    //     Base(const ::state::Base& other) : _vars(other._vars) {}
    //     virtual ~Base() = default;

    //     virtual std::shared_ptr<Base> Input_A() {}
    //     virtual std::shared_ptr<Base> Input_B() {}
    //     virtual std::shared_ptr<Base> Input_C() {}
    // protected:
    //     Vars& _vars;
    // };

    // extern std::shared_ptr<Base> current;

    // class Start : public Base
    // {
    // public:
    //     Start(Base& other) : Base(other) {}
    //     virtual ~Start() = default;

    //     virtual std::shared_ptr<Base> Input_A() override;
    //     virtual std::shared_ptr<Base> Input_B() override;
    // };

    // class Pause : public Base
    // {
    // public:
    //     using Base::Base;
    //     virtual ~Pause() = default;

    //     virtual std::shared_ptr<Base> Input_A() override;
    //     virtual std::shared_ptr<Base> Input_B() override;
    //     virtual std::shared_ptr<Base> Input_C() override;
    // };


    // class Run : public Base
    // {
    // public:
    //     Run(Base& other) : Base(other) {}
    //     virtual ~Run() = default;

    //     virtual std::shared_ptr<Base> Input_A() override;
    //     virtual std::shared_ptr<Base> Input_B() override;
    //     virtual std::shared_ptr<Base> Input_C() override;
    // };

}
