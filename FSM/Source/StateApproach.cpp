#include "StateApproach.h"
#include "Ship.h"
#include "FSMAIControl.h"
#include "utility.h"
#include "GameSession.h"


//---------------------------------------------------------
void StateApproach::Update(float dt)
{
    //turn and then thrust towards closest asteroid
    FSMAIControl* parent = (FSMAIControl*)m_parent;
    GameObj* asteroid = parent->m_nearestAsteroid;
    Ship*    ship     = parent->m_ship;

	Point3f deltaPos = asteroid->m_position - ship->m_position;
	Point3f astVelNormalized = asteroid->m_velocity;
	astVelNormalized.Normalize();
    
	Point3f futureAstPosition;

	//use braking vector if you're going too fast
	bool needToBrake = false;
    float speed		 = ship->m_velocity.Length();
    if(speed > parent->m_maxSpeed)
	{
		needToBrake = true;
		deltaPos	= -ship->m_velocity;
	}
	else
	{
		float dotVelocity = DOT(ship->UnitVectorVelocity(),asteroid->UnitVectorVelocity());

		//if the other guy is "to my front" and we're moving towards each other...
		Point3f targetPos = asteroid->m_position;
		if ((DOT(deltaPos,ship->UnitVectorVelocity()) < 0) || (dotVelocity > -0.93))//magic number == about 21 degrees
		{
			Point3f shipVel = ship->m_velocity;
			shipVel = shipVel.Normalize() * parent->m_maxSpeed;
			float combinedSpeed	  = (shipVel + asteroid->m_velocity).Length();
			float predictionTime  = deltaPos.Length() / combinedSpeed;
			targetPos = asteroid->m_position + (asteroid->m_velocity*predictionTime);
			deltaPos  = targetPos - ship->m_position;

			Game.Clip(futureAstPosition);
		}
	}
	//sub off our current velocity, to get direction of wanted velocity
	deltaPos -= ship->m_velocity;
  
    //find new direction, and head to it
    float newDir	 = CALCDIR(deltaPos);
    float angDelta	 = CLAMPDIR180(newDir - ship->m_angle);
	bool canApproachInReverse = needToBrake || ship->GetShotLevel()!=0;
	
    if(fabsf(angDelta) <3 || (fabsf(angDelta)> 177 && canApproachInReverse) )
    {
        //thrust
        ship->StopTurn();
        if(parent->m_nearestAsteroidDist > parent->m_nearestAsteroid->m_size + 20)
            fabsf(angDelta)<3? ship->ThrustOn() : ship->ThrustReverse();
        else
            ship->ThrustOff();
    }
	else if(fabsf(angDelta)<=90 || !canApproachInReverse)
    {
        //turn when facing forwards
		if(angDelta<0)
			ship->TurnRight();
		else if(angDelta>0)
			ship->TurnLeft();
    }
    else
    {
        //turn when facing rear
        if(angDelta>0)
            ship->TurnRight();
        else if(angDelta<0)
            ship->TurnLeft();
    }
    parent->m_target->m_position = asteroid->m_position;
    parent->m_targetDir = newDir;
    parent->m_debugTxt = "Approach";
}

//---------------------------------------------------------
int StateApproach::CheckTransitions()
{
    FSMAIControl* parent = (FSMAIControl*)m_parent;
    if(parent->m_willCollide)
        return FSM_STATE_EVADE;

    if(parent->m_powerupNear && parent->m_nearestAsteroidDist > 
	   parent->m_nearestPowerupDist && parent->m_ship->GetShotLevel() < MAX_SHOT_LEVEL)
        return FSM_STATE_GETPOWERUP;

    if(!parent->m_nearestAsteroid || parent->m_nearestAsteroidDist < APPROACH_DIST)
        return FSM_STATE_IDLE;

    return FSM_STATE_APPROACH;  
}

//---------------------------------------------------------
void StateApproach::Exit()
{
    if(((FSMAIControl*)m_parent)->m_ship)
    {
        ((FSMAIControl*)m_parent)->m_ship->ThrustOff();
        ((FSMAIControl*)m_parent)->m_ship->StopTurn();
    }
}