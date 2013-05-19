#include "FSMAIControl.h"
#include "FSMMachine.h"
#include "GameSession.h"
#include "utility.h"
#include "StateApproach.h"
#include "StateAttack.h"
#include "StateMassiveAttack.h"
#include "StateEvade.h"
#include "StateGetPowerup.h"
#include "StateIdle.h"
#include "Target.h"

using namespace cyclone;

//---------------------------------------------------------
FSMAIControl::FSMAIControl(Ship* ship):
AIControl(ship)
{
    //construct the state machine and add the necessary states
    m_machine = new FSMMachine(FSM_MACH_MAINSHIP,this);
    StateApproach* approach = new StateApproach(this);
    m_machine->AddState(approach);
    m_machine->AddState(new StateAttack(this));
    m_machine->AddState(new StateMassiveAttack(this));
    m_machine->AddState(new StateEvade(this));
    m_machine->AddState(new StateGetPowerup(this));
    StateIdle* idle = new StateIdle(this);
    m_machine->AddState(idle);
//  m_machine->SetDefaultState(idle);
    m_machine->SetDefaultState(approach);
    m_machine->Reset();
}

//---------------------------------------------------------
void FSMAIControl::Init()
{
    m_willCollide  = false;
    m_powerupNear  = false;
    m_nearestAsteroid = NULL;
    m_nearestPowerup  = NULL;
    m_safetyRadius    = 15.0f;
    m_maxSpeed        = AI_MAX_SPEED_TRY;///Game.m_timeScale;
	m_asteroidCount   = INT_MAX;
    
    if(!m_target)
    {
        m_target = new Target;
        m_target->m_size = 1;
        Game.PostGameObj(m_target);
    }
}

//---------------------------------------------------------
void FSMAIControl::Update(float dt)
{
    if(!m_ship)
    {
        m_machine->Reset();
        return;
    }
    
    UpdatePerceptions(dt);
    m_machine->UpdateMachine(dt);
}

//---------------------------------------------------------
void FSMAIControl::UpdatePerceptions(float dt)
{
    if(m_willCollide)
        m_safetyRadius = 30.0f;
    else
        m_safetyRadius = 15.0f;
    
    //store closest asteroid and powerup
    m_nearestAsteroid = Game.GetClosestGameObj(m_ship,GameObj::OBJ_ASTEROID);
    m_nearestPowerup  = Game.GetClosestGameObj(m_ship,GameObj::OBJ_POWERUP);

	//count number of asteroids in the scene
	m_asteroidCount = Game.GetNumGameObj(GameObj::OBJ_ASTEROID);
    
    //asteroid collision determination
    m_willCollide = false;
    if(m_nearestAsteroid)
    {
        float speed = m_ship->getVelocity().magnitude();
        m_nearestAsteroidDist = m_nearestAsteroid->getPosition().distance(m_ship->getPosition());
        Vector3 normDelta = m_nearestAsteroid->getPosition() - m_ship->getPosition();
        normDelta.normalise();
        float astSpeed = m_nearestAsteroid->getVelocity().magnitude();
        float shpSpeedAdj = DOT(m_ship->UnitVectorVelocity(),normDelta)*speed;
        float astSpeedAdj = astSpeed * DOT(m_nearestAsteroid->UnitVectorVelocity(), -normDelta);
        speed = shpSpeedAdj+astSpeedAdj;

//        if(speed > astSpeed)
//            dotVel  = DOT(m_ship->UnitVectorVelocity(),normDelta);
//        else 
//        {
//            speed = astSpeed;
//            dotVel = DOT(m_nearestAsteroid->UnitVectorVelocity(),-normDelta);
//        }
        float spdAdj = LERP(speed/m_maxSpeed,0.0f,90.0f);
        float adjSafetyRadius = m_safetyRadius+spdAdj+m_nearestAsteroid->m_size;
        
        //if you're too close, and I'm heading somewhat towards you,
        //flag a collision
        if(m_nearestAsteroidDist <= adjSafetyRadius && speed > 0)
            m_willCollide = true;
    }

    //powerup near determination
    m_powerupNear = false;
    if(m_nearestPowerup)
    {
        m_nearestPowerupDist = m_nearestPowerup->getPosition().distance(m_ship->getPosition());
        if(m_nearestPowerupDist <= POWERUP_SCAN_DIST)
        {
            m_powerupNear     = true;
        }
    }
    
}