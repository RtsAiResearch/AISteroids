#include "StateGetPowerup.h"
#include "Ship.h"
#include "FSMAIControl.h"
#include "utility.h"
#include "GameSession.h"

using namespace cyclone;

//---------------------------------------------------------
void StateGetPowerup::Update(float dt)
{
    FSMAIControl* parent = (FSMAIControl*)m_parent;
    GameObj* powerup = parent->m_nearestPowerup;
    Ship*    ship     = parent->m_ship;

    if(!powerup || !ship)
        return;
    
    //find future position of powerup
    Vector3 futurePowPosition = powerup->getPosition();
    Vector3 deltaPos = futurePowPosition - ship->getPosition();
    float dist  = deltaPos.magnitude();
    float speed = parent->m_maxSpeed;
    float time = dist/speed;
    futurePowPosition += powerup->getVelocity() * time - ship->getVelocity() * time;
	Game.Clip(futurePowPosition);
    Vector3 deltaFPos = futurePowPosition - ship->getPosition();
    deltaFPos.normalise();
    
    //add braking vec if you're going too fast
    speed = ship->getVelocity().magnitude();
    if(speed > parent->m_maxSpeed)
        deltaFPos += -ship->UnitVectorVelocity();

    //DOT out my velocity
    Vector3 shpUnitVel = ship->UnitVectorVelocity();
    float dotVel = DOT(shpUnitVel,deltaFPos);
    float proj = 1-dotVel;
    deltaFPos -= shpUnitVel * proj;
    deltaFPos.normalise();

    float newDir = CALCDIR(deltaFPos);
    float angDelta = CLAMPDIR180(ship->m_angle - newDir);
    if(fabsf(angDelta) <3 || fabsf(angDelta)> 177)//thrust
    {
        ship->StopTurn();
        if(speed < parent->m_maxSpeed || parent->m_nearestPowerupDist > 20)
            fabsf(angDelta)<3? ship->ThrustOn() : ship->ThrustReverse();
        else
            ship->ThrustOff();
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
    
    parent->m_target->setPosition(futurePowPosition + ship->getVelocity() * time);
    parent->m_targetDir = newDir;
    parent->m_debugTxt = "GetPowerup";
}

//---------------------------------------------------------
int StateGetPowerup::CheckTransitions()
{
    FSMAIControl* parent = (FSMAIControl*)m_parent;

    if(parent->m_willCollide)
        return FSM_STATE_EVADE;

	if (parent->m_asteroidCount < 4)
	{
		return FSM_STATE_GETPOWERUP;
	}

    if(!parent->m_nearestPowerup || parent->m_nearestAsteroidDist < parent->m_nearestPowerupDist)
        return FSM_STATE_IDLE;

    return FSM_STATE_GETPOWERUP;
}

//---------------------------------------------------------
void StateGetPowerup::Exit()
{
    if(((FSMAIControl*)m_parent)->m_ship)
    {
        ((FSMAIControl*)m_parent)->m_ship->ThrustOff();
        ((FSMAIControl*)m_parent)->m_ship->StopTurn();
    }
}