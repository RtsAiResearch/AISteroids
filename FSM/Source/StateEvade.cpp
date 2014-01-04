#include "StateEvade.h"
#include "Ship.h"
#include "FSMAIControl.h"
#include "utility.h"

using namespace cyclone;

//---------------------------------------------------------
void StateEvade::Update(float dt)
{
    //evade by going to the quad opposite as the asteroid
    //is moving, add in a deflection, and cancel out your movement
    FSMAIControl* parent = (FSMAIControl*)m_parent;
    GameObj* asteroid = parent->m_nearestAsteroid;
    Ship*    ship     = parent->m_ship;
    Vector3 vecSteer = CROSS(ship->getPosition(), asteroid->getPosition());
    Vector3 vecBrake = ship->getPosition() - asteroid->getPosition();
    vecSteer += vecBrake;
    
    float newDir = CALCDIR(vecSteer);
    float angDelta = CLAMPDIR180(ship->m_angle - newDir);
    float dangerAdjAngle = ((1.0f - parent->m_nearestAsteroidDist/ APPROACH_DIST)*10.0f) + 1.0f;
    if(fabsf(angDelta) <dangerAdjAngle || fabsf(angDelta)> 180-dangerAdjAngle)//thrust
    {
        ship->StopTurn();
        if(ship->getVelocity().magnitude() < parent->m_maxSpeed || parent->m_nearestAsteroidDist < 20 + asteroid->m_size)
            fabsf(angDelta)<dangerAdjAngle? ship->ThrustOn() : ship->ThrustReverse();
        else
            ship->ThrustOff();

        //if I'm pointed right at the asteroid, shoot
        ship->Shoot();
    }
    else if(fabsf(angDelta)<=90)//turn front
    {
        if(angDelta >0)
            ship->TurnRight();
        else
            ship->TurnLeft();
    }
    else//turn rear
    {
        if(angDelta<0)
            ship->TurnRight();
        else
            ship->TurnLeft();
    }

    parent->m_target->setPosition(parent->m_nearestAsteroid->getPosition());
    parent->m_targetDir = newDir;
    parent->m_debugTxt = "Evade";
}

//---------------------------------------------------------
int StateEvade::CheckTransitions()
{
    FSMAIControl* parent = (FSMAIControl*)m_parent;

    if(!parent->m_willCollide)
        return FSM_STATE_IDLE;

    return FSM_STATE_EVADE;
}

//---------------------------------------------------------
void StateEvade::Exit()
{
    if(((FSMAIControl*)m_parent)->m_ship)
    {
        ((FSMAIControl*)m_parent)->m_ship->ThrustOff();
        ((FSMAIControl*)m_parent)->m_ship->StopTurn();
    }
}
