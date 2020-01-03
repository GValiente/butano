#include "btn_action.h"

#include "btn_actions_manager.h"

namespace btn
{

action::action(action&& other)
{
    if(other._running)
    {
        other.stop();
        run();
    }
}

action& action::operator=(action&& other)
{
    if(other._running)
    {
        other.stop();
        run();
    }

    return *this;
}

action::~action()
{
    stop();
}

void action::run()
{
    if(! _running)
    {
        actions_manager::run(*this);
        _running = true;
    }
}

void action::stop()
{
    if(_running)
    {
        actions_manager::stop(*this);
        _running = false;
    }
}

}
