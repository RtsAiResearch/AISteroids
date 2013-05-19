#pragma once

#include <math.h>
#include "core.h"
#include "particle.h"
#include "Sphere3.h"
#pragma warning(disable: 4786)
#include <vector>
#include <list>

#define NO_LIFE_TIMER 99999
#define AI_MAX_SPEED_TRY  80.0f

class GameObj
{
public:
	//constructors/functions
	GameObj(float _size = 1);
    GameObj(const cyclone::Vector3 &_p, const float _angle);
    GameObj(const cyclone::Vector3 &_p, const float _angle, const cyclone::Vector3 &_v);
    ~GameObj();
	virtual void Draw(){}
    virtual void Init();
	virtual void Update(float dt);
	virtual bool IsColliding(GameObj *obj);
	virtual void DoCollision(GameObj *obj) {Explode();m_active = false;}
	virtual void Explode();
	cyclone::Vector3 UnitVectorFacing();//unit vector in facing direction
	cyclone::Vector3 UnitVectorVelocity();//unit vector in velocity direction
	void setVelocity(cyclone::Vector3 velocity) { particle.setVelocity(velocity); }
	cyclone::Vector3 getVelocity() { return particle.getVelocity(); }
	void setAcceleration(cyclone::Vector3 acceleration) { particle.setAcceleration(acceleration); }
	cyclone::Vector3 getAcceleration() { return particle.getAcceleration(); }
	void setPosition(cyclone::Vector3 position) { particle.setPosition(position); }
	cyclone::Vector3 getPosition() { return particle.getPosition(); }

	enum//collision flags/object types
	{
		OBJ_NONE		= 0x00000001,
		OBJ_ASTEROID	= 0x00000002,
        OBJ_SHIP		= 0x00000004,
		OBJ_BULLET		= 0x00000008,
		OBJ_EXP			= 0x00000010,
		OBJ_POWERUP		= 0x00000020,
		OBJ_TARGET      = 0x00000040,
        OBJ_SAUCER		= 0x00000080
    };
	
	//data
	cyclone::Vector3		m_axis;	  
	float		m_angle;  
    float		m_angVelocity;
    bool		m_active; 
	bool		m_explodes; 
	float		m_size;
	Sphere3f	m_boundSphere;
	int			m_type;
	unsigned int m_collisionFlags;
	float		m_lifeTimer;

private:
	cyclone::Particle particle;
};
