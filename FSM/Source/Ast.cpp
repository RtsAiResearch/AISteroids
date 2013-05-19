#include <GL/glut.h>
#include "GameObj.h"
#include "Ast.h"
#include "Exp.h"
#include "Gamesession.h"
#include "utility.h"

using namespace cyclone;


//---------------------------------------------------------
void Ast::Init()
{
	m_type = GameObj::OBJ_ASTEROID;
	m_collisionFlags = GameObj::OBJ_ASTEROID;
}

//---------------------------------------------------------
void Ast::Draw()
{
	//just a circle
	glPushMatrix();
	glDisable(GL_LIGHTING);
	Vector3 position = getPosition();
	glTranslatef(position.x, position.y, position.z);
	glRotatef(m_angle,0,0,1);
	glScalef(m_size,m_size,m_size);
	
	glBegin(GL_LINE_LOOP);
	glColor3f(.7,.6,.6);
	const int nside=18;
	for(int i=0;i<=nside;i++)
		glVertex3f(cos(float(i)*M_PI*2.0/nside),sin(float(i)*M_PI*2.0/nside),0.0);
	glEnd();
	
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

//---------------------------------------------------------
void Ast::DoCollision(GameObj *obj)
{
	//if I'm colliding with another asteroid, just
	//do "pool ball" collision
	if(obj->m_type == GameObj::OBJ_ASTEROID)
	{
		Vector3 delta = getPosition() - obj->getPosition();
		//ensure at least 1 dist between, since we divide by
		//that number below
		float distanceBetweenUs = MAX(delta.magnitude(), 1);
		float combinedSize = m_size + obj->m_size;
		//separate the objects to disallow overlap
		//and swap velocities
		if(distanceBetweenUs < combinedSize)
		{
			setPosition(getPosition() + (delta/distanceBetweenUs)*(combinedSize - distanceBetweenUs));

			Vector3 temp = getVelocity();
			setVelocity(obj->getVelocity());
			obj->setVelocity(temp);
		}
		return;
	}

	//get rid of myself
	GameObj::DoCollision(obj);
	Game.m_numAsteroids--;

	//split into two
	if(m_size>16)
	{
		Ast *ast;
		Vector3 nv;
		nv.x=randflt()*70 - 30;	
		nv.y=randflt()*70 - 30;
		nv.z=0;

		ast	  =new Ast(m_size/2);
		if(ast)
		{
			ast->m_angVelocity = randflt()*70;
			ast->m_axis		   = Vector3(0,0,1);
			ast->setPosition(getPosition());
			ast->setVelocity(getVelocity() + nv);
			Game.PostGameObj(ast);
			Game.m_numAsteroids++;
		}
        ast	  =new Ast(m_size/2);
        if(ast)
		{
			ast->m_angVelocity = randflt()*70;
			ast->m_axis		   = Vector3(0, 0, 1);
			ast->setPosition(getPosition());
			ast->setVelocity(getVelocity() - nv);
			Game.PostGameObj(ast);
			Game.m_numAsteroids++;
		}
	}
}