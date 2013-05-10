#include <GL/glut.h>
#include "GameObj.h"
#include "Ast.h"
#include "Exp.h"
#include "Gamesession.h"
//#include "TestSession.h"
#include "utility.h"

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
	glTranslate(m_position);
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
		Point3f delta = m_position - obj->m_position;
		//ensure at least 1 distbetween, since we divide by
		//that number below
		float distanceBetweenUs = MAX(delta.Length(),1);
		float combinedSize = m_size + obj->m_size;
		//separate the objects to disallow overlap
		//and swap velocities
		if(distanceBetweenUs < combinedSize)
		{
			m_position += (delta/distanceBetweenUs)*(combinedSize - distanceBetweenUs);

			Point3f temp = m_velocity;
			m_velocity = obj->m_velocity;
			obj->m_velocity = temp;
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
		Point3f nv;
		nv.x()=randflt()*70 - 30;	
		nv.y()=randflt()*70 - 30;
		nv.z()=0;

		ast	  =new Ast(m_size/2);
		if(ast)
		{
			ast->m_angVelocity=randflt()*70;
			ast->m_axis		=Point3f(0,0,1);
			ast->m_position	=m_position;
			ast->m_velocity	=m_velocity+nv;
			Game.PostGameObj(ast);
			Game.m_numAsteroids++;
		}
        ast	  =new Ast(m_size/2);
        if(ast)
		{
			ast->m_angVelocity=randflt()*70;
			ast->m_axis		=Point3f(0,0,1);
			ast->m_position	= m_position;
			ast->m_velocity	= m_velocity-nv;
			Game.PostGameObj(ast);
			Game.m_numAsteroids++;
		}
	}
}