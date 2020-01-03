#include "btn_actions_manager.h"

#include "btn_action.h"
#include "btn_assert.h"
#include "btn_power_of_two.h"
#include "btn_hash_set.h"
#include "btn_config_actions.h"

namespace btn::actions_manager
{

namespace
{
    static_assert(BTN_CFG_ACTIONS_MAX_RUNNING > 0);
    static_assert(power_of_two(BTN_CFG_ACTIONS_MAX_RUNNING));

    class static_data
    {

    public:
        hash_set<action*, BTN_CFG_ACTIONS_MAX_RUNNING> actions;
    };

    BTN_DATA_EWRAM static_data data;
}

void run(action_type& action)
{
    BTN_ASSERT(! action.done(), "Action is done");
    BTN_ASSERT(! data.actions.full(), "Too much running actions");

    data.actions.insert(&action);
}

void stop(action_type& action)
{
    data.actions.erase(&action);
}

void update()
{
    auto it = data.actions.begin();
    auto end = data.actions.end();

    while(it != end)
    {
        action_type* action = *it;
        action->update();

        if(action->done())
        {
            it = data.actions.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

}
