#ifndef BTN_ACTION_H
#define BTN_ACTION_H

#include "btn_common.h"

namespace btn
{

class action
{

public:
    action(const action& other) = delete;

    action& operator=(const action& other) = delete;

    action(action&& other);

    action& operator=(action&& other);

    virtual ~action();

    [[nodiscard]] bool running() const
    {
        return _running;
    }

    void run();

    void stop();

    virtual void update() = 0;

    [[nodiscard]] virtual bool done() const = 0;

protected:
    action() = default;

private:
    friend class actions_manager_impl;

    bool _running = false;
};

}

#endif
