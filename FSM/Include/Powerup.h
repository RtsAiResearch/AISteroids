#pragma once

#include "GameObj.h"

#define POWERUP_SIZE 8
#define POW_MAX_LIFE 6.5f

class Powerup : public GameObj
{
public:
    //constructors/functions
    Powerup():GameObj(POWERUP_SIZE),m_powerType(POWERUP_SHOT) {Init();}
    Powerup(const cyclone::Vector3 &_p, const float _angle, const cyclone::Vector3 &_v);
    virtual void Draw();
    void Init();
    void DoCollision(GameObj *obj);
    void SetType(int type) {m_powerType = type;}

    enum
    {
        POWERUP_NONE,
        POWERUP_SHOT
    };

private:
    //data
    int m_powerType;
};
