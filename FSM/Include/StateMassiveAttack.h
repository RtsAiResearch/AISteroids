#pragma once

#include "FSMState.h"

class StateMassiveAttack : public FSMState
{
public:
    StateMassiveAttack(Control* parent) : FSMState(FSM_STATE_MASSIVE_ATTACK, parent) {}

    void Update(float dt);
    
    int CheckTransitions();
    
    void Exit();
};
