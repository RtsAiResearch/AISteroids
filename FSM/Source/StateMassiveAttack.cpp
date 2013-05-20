#include "StateMassiveAttack.h"
#include "Ship.h"
#include "FSMAIControl.h"
#include "utility.h"
#include "GameSession.h"

using namespace cyclone;

//---------------------------------------------------------
void StateMassiveAttack::Update(float dt)
{
	FSMAIControl* parent = (FSMAIControl*)m_parent;
    GameObj* asteroid = parent->m_nearestAsteroid;
    Ship*    ship     = parent->m_ship;
    
    if(!asteroid)
        return;

	ship->setDestination(Vector3(Game.m_screenW / 2, Game.m_screenH / 2, 0), parent->m_maxSpeed);
	ship->TurnRight();
	ship->Shoot();

    parent->m_debugTxt = "MassiveAttack";
}

//---------------------------------------------------------
int StateMassiveAttack::CheckTransitions()
{
    FSMAIControl* parent = (FSMAIControl*)m_parent;
    if(parent->m_willCollide)
        return FSM_STATE_EVADE;

    if(parent->m_powerupNear && parent->m_nearestAsteroidDist > 
       parent->m_nearestPowerupDist && parent->m_ship->GetShotLevel() < MAX_SHOT_LEVEL)
        return FSM_STATE_GETPOWERUP;
    
    if(!parent->m_nearestAsteroid || parent->m_nearestAsteroidDist > APPROACH_DIST)
        return FSM_STATE_IDLE;

    return FSM_STATE_MASSIVE_ATTACK;
}

//---------------------------------------------------------
void StateMassiveAttack::Exit()
{
    if(((FSMAIControl*)m_parent)->m_ship)
        ((FSMAIControl*)m_parent)->m_ship->StopTurn();
}