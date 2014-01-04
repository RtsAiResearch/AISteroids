#include "StateAttack.h"
#include "Ship.h"
#include "FSMAIControl.h"
#include "utility.h"

using namespace cyclone;

//---------------------------------------------------------
void StateAttack::Update(float dt)
{
    //turn towards closest asteroid's future position, and then fire
    FSMAIControl* parent = (FSMAIControl*)m_parent;
    GameObj* asteroid = parent->m_nearestAsteroid;
    Ship*    ship     = parent->m_ship;
    
    if(!asteroid)
        return;
    
    Vector3 futureAstPosition = asteroid->getPosition();
    Vector3 deltaPos = futureAstPosition - ship->getPosition();
    float dist  = deltaPos.magnitude();
    float time = dist/BULLET_SPEED;
    futureAstPosition += asteroid->getVelocity() * time - ship->getVelocity() * time;
    Vector3 deltaFPos = futureAstPosition - ship->getPosition();
    deltaFPos.normalise();

    float newDir     = CALCDIR(deltaFPos);
    float angDelta   = CLAMPDIR180(ship->GetClosestGunAngle(newDir) - newDir);
    float dangerAdjAngle = ((1.0f - parent->m_nearestAsteroidDist/ APPROACH_DIST) * 5.0f) + 1.0f;

    if(angDelta >=1)//dangerAdjAngle)
        ship->TurnRight();
    else if(angDelta <= -1)//-dangerAdjAngle)
        ship->TurnLeft();
    else
    {
        ship->StopTurn();
        ship->Shoot();
    }
    
    parent->m_target->setPosition(futureAstPosition);
    parent->m_targetDir = newDir;
    parent->m_debugTxt = "Attack";
}

//---------------------------------------------------------
int StateAttack::CheckTransitions()
{
    FSMAIControl* parent = (FSMAIControl*)m_parent;
    if(parent->m_willCollide)
        return FSM_STATE_EVADE;

    if(parent->m_powerupNear && parent->m_nearestAsteroidDist > 
       parent->m_nearestPowerupDist && parent->m_ship->GetShotLevel() < MAX_SHOT_LEVEL)
        return FSM_STATE_GETPOWERUP;
    
    if(!parent->m_nearestAsteroid || parent->m_nearestAsteroidDist > APPROACH_DIST)
        return FSM_STATE_IDLE;

    return FSM_STATE_ATTACK;    
}

//---------------------------------------------------------
void StateAttack::Exit()
{
    if(((FSMAIControl*)m_parent)->m_ship)
        ((FSMAIControl*)m_parent)->m_ship->StopTurn();
}
