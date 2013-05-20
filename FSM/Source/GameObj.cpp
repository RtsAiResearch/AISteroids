#include <GL/glut.h>
#include "Sphere3.h"
#include "GameObj.h"
#include "Exp.h"
#include "GameSession.h"
#include "utility.h"
#include <vector>
#include <list>

using namespace cyclone;

//---------------------------------------------------------
GameObj::GameObj(float _size)
{
	body.setVelocity(10.0f, 10.0f, 0.0f); // 5m/s
    Init();
	m_size = _size;
	m_boundSphere.r = m_size;
}

//---------------------------------------------------------
GameObj::GameObj(const Vector3 &_p, const float _angle, const Vector3 &_v)
{
	setPosition(_p);
    setVelocity(_v);
	m_angle = _angle;
    Init();
}

//---------------------------------------------------------
GameObj::GameObj(const Vector3 &_p, const float _angle)
{
    setPosition(_p);
    setVelocity(Vector3::zero());
    m_angle = _angle;
    Init();
}

//---------------------------------------------------------
GameObj::~GameObj()
{
}

//---------------------------------------------------------
void GameObj::Init()
{
	body.setAcceleration(Vector3::zero());
	body.setMass(2.0f);
	body.setDamping(1.0f, 1.0f);
	//cyclone::Matrix3 it;
	//it.setBlockInertiaTensor(cyclone::Vector3(2,1,1), 1);
	//body.setInertiaTensor(it);
	body.setAwake();
	body.setCanSleep(false);
    m_angVelocity	 = 0;
    m_axis			 = Vector3(0, 0, 1);
    m_active		 = true;
    m_size			 = 1;
    m_boundSphere.r	 = m_size;
    m_collisionFlags = OBJ_NONE;
    m_type			 = OBJ_NONE;
    m_lifeTimer		 = NO_LIFE_TIMER;
}

//---------------------------------------------------------
bool GameObj::IsColliding(GameObj *obj)
{
	m_boundSphere.c		 = getPosition();
	obj->m_boundSphere.c = obj->getPosition();
	return m_boundSphere.Intersect(obj->m_boundSphere);
}

//---------------------------------------------------------
void GameObj::Update(float dt)
{
 //   m_velocity  += m_acceleration * dt;
	////don't clamp bullets
	//if(!(m_type & OBJ_BULLET))
	//	CLAMPVECTORLENGTH(m_velocity,0.0f,AI_MAX_SPEED_TRY);

 //   m_position  += dt*m_velocity;
	//Game.Clip(m_position );

    //m_angle     += dt*m_angVelocity;
    //m_angle      = CLAMPDIR180(m_angle);

 //   if(m_position.z !=0.0f)
 //   {
 //       m_position.z = 0.0f;
 //   }
	
	body.integrate(dt);
    
    if(m_lifeTimer != NO_LIFE_TIMER)
    {
        m_lifeTimer -= dt;
        if(m_lifeTimer < 0.0f) 
            m_active = false;
    }
};

//---------------------------------------------------------
void GameObj::Explode()
{
	Exp *e = NULL;
	switch(m_type)
	{
		case OBJ_ASTEROID:
        case OBJ_SHIP:
        case OBJ_SAUCER:
			e = new Exp(this);
			e->setPosition(getPosition());
			Game.PostGameObj(e);
			break;		

		case OBJ_NONE:
		case OBJ_BULLET:
		case OBJ_EXP:
		case OBJ_POWERUP:
		default:
			break;
	}
}
//---------------------------------------------------------
Vector3 GameObj::UnitVectorFacing()
{
	return UNITFROMANGLE(m_angle);
}; 

//---------------------------------------------------------
Vector3 GameObj::UnitVectorVelocity()
{
	return getVelocity().unit();
};