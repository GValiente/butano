#include "btn_actions_manager.h"

#include "btn_action.h"
#include "btn_hash_set.h"
#include "btn_config_actions.h"

namespace btn
{

namespace
{
    static_assert(BTN_CFG_ACTIONS_MAX_RUNNING > 0);
    static_assert(power_of_two(BTN_CFG_ACTIONS_MAX_RUNNING));
}

class actions_manager_impl
{

public:
    void run(actions_manager::action_type& action)
    {
        BTN_ASSERT(! _actions.full(), "Too much running actions");

        _actions.insert(&action);
    }

    void stop(actions_manager::action_type& action)
    {
        _actions.erase(&action);
    }

    void update()
    {
        auto it = _actions.begin();
        auto end = _actions.end();

        while(it != end)
        {
            actions_manager::action_type* action = *it;

            if(action->done())
            {
                action->_running = false;
                it = _actions.erase(it);
            }
            else
            {
                action->update();
                ++it;
            }
        }
    }

private:
    hash_set<action*, BTN_CFG_ACTIONS_MAX_RUNNING> _actions;
};

namespace actions_manager
{

namespace
{
    class static_data
    {

    public:
        actions_manager_impl impl;
    };

    BTN_DATA_EWRAM static_data data;
}

void run(action_type& action)
{
    data.impl.run(action);
}

void stop(action_type& action)
{
    data.impl.stop(action);
}

void update()
{
    data.impl.update();
}

}
}
