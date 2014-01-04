#pragma once

#pragma warning(disable: 4786)
#include <list>
#include "core.h"

class Ship;
class AIOpponent;
class Bullet;
class Ast;
class GameObj;
class HumanControl;
class AIControl;

using namespace std;

#define BONUS_LIFE_SCORE 10000

#define INITIAL_WORLD_SIZE 1024

typedef std::list<GameObj*> GameObjectList;

class GameSession
{
public:
    //constructor/main functions
    GameSession();
    void Update(float dt);
    void Draw();
    void DrawLives();
    void Clip(cyclone::Vector3 &p);
    void PostGameObj(GameObj *obj);
    int  GetNumGameObj(int type);
    GameObj* GetClosestGameObj(GameObj* obj, int type);
    GameObj* GetClosestGameObj(cyclone::Vector3 &point, int type);
    void ApplyForce(int type,cyclone::Vector3 &force, float dt);
    void ApplyForce(int type,cyclone::Vector3 &p1, cyclone::Vector3 &p2, cyclone::Vector3 &force, float dt);
    
    //controls
    enum
    {
        CONTROL_THRUST_ON,
        CONTROL_THRUST_REVERSE,
        CONTROL_THRUST_OFF,
        CONTROL_RIGHT_ON,
        CONTROL_LEFT_ON,
        CONTROL_STOP_TURN,
        CONTROL_STOP,
        CONTROL_SHOOT,
        CONTROL_HYPERSPACE,
        CONTROL_PAUSE,
        CONTROL_STEP,
        CONTROL_AI_ON,
        CONTROL_AI_OFF,
        CONTROL_COUNT
    };
    void UseControl(int control);
    
    //score functions
    void IncrementScore(int inc) {m_score += inc;}
    void ResetScore() {m_score = 0;m_bonusScore = BONUS_LIFE_SCORE;}
    
    //game related fucntions
    void StartGame();
    void StartNextWave();
    void LaunchAsteroidWave();
    void WaveOver();
    void GameOver();
    void KillShip(GameObj *ship);
    
    //data
    Ship*         m_mainShip;
    HumanControl* m_humanControl;
    AIControl*    m_AIControl;
    
    int  m_bonusScore;
    int  m_screenW;
    int  m_screenH;
    float m_respawnTimer;
    float m_powerupTimer;
    int  m_state;
    int  m_score;
    int  m_numLives;
    int  m_waveNumber;
    int  m_numAsteroids;
    bool m_AIOn;
    int  m_timeScale;
    
    enum
    {
        STATE_PLAY,
        STATE_PAUSE,
        STATE_NEXTWAVE,
        STATE_GAMEOVER,
        STATE_STEP,
        STATE_STEPWAIT
    };
    
protected:
    GameObjectList m_activeObj;
};

//---------------------------------------------------------
extern GameSession Game;
