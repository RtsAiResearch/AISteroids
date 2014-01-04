#include <GL/glut.h>
#include "GameObj.h"
#include "Powerup.h"
#include "Ship.h"
//#include "Testsession.h"
#include "Gamesession.h"
#include "utility.h"

using namespace cyclone;

//---------------------------------------------------------
Powerup::Powerup(const Vector3 &_p, const float _angle, const Vector3 &_v):
GameObj(_p,_angle,_v)
{
    Init();
}

//---------------------------------------------------------
void Powerup::Init()
{
    m_size = POWERUP_SIZE;
    m_type = GameObj::OBJ_POWERUP;
    m_collisionFlags = GameObj::OBJ_SHIP | GameObj::OBJ_SAUCER;
    m_lifeTimer = POW_MAX_LIFE*Game.m_timeScale;
}

//---------------------------------------------------------
void Powerup::Draw()
{
    //just a square
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glTranslatef(getPosition().x, getPosition().y, getPosition().z);
    glRotatef(m_angle,0,0,1);
    glScalef(m_size,m_size,m_size);
    
    glBegin(GL_LINE_LOOP);
    glColor3f(.7,.6,.6);
    glVertex3f( 1.0, 1.0,0);
    glVertex3f(-1.0, 1.0,0);
    glVertex3f(-1.0,-1.0,0);
    glVertex3f( 1.0,-1.0,0);
    glEnd();
    
    glEnable(GL_LIGHTING);
    glPopMatrix();

    //draw any special case delineation for different powerup types
    switch(m_powerType)
    {
        case POWERUP_SHOT://nothing for right now
        default:
            break;
    }
}

//---------------------------------------------------------
void Powerup::DoCollision(GameObj *obj)
{
    //take me out, and give power to the ship
    m_active = false;
    if(obj->m_type == OBJ_SHIP || obj->m_type == OBJ_SAUCER)
        ((Ship*)obj)->GetPowerup(m_powerType);
}
