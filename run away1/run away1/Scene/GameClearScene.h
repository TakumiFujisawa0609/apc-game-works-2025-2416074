#pragma once
#include <vector>
#include<Dxlib.h>
#include"../Scene/SceneBase.h"
class Player;
class Camera;
class NightDoom;
class ActorBase;


class GameClearScene :public SceneBase
{
public:
    GameClearScene();
    ~GameClearScene()override;
    void Load(void) override;
    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Release(void) override;

    Player* GetPlayer();

    void Cleared();
private:

    int image_;
    int exit_;
    int forest_;
    int forest2_;
    bool cleared_;
    bool gameOver_;
    VECTOR pos_1;
    VECTOR pos_2;
    VECTOR pos_3;
    VECTOR pPos_;

    VECTOR clearPos_;

    VECTOR scl_;
    VECTOR rot_;


    Player* player_ = nullptr;
    NightDoom* nDoom = nullptr;
    Camera* camera_ = nullptr;
    //すべてのアクター
    std::vector<ActorBase*>allActor_;

    //当たり判定
    void WallCollision(ActorBase* actor);
    //当たり判定
    void ForestCollision(ActorBase* actor);
    ////敵のスタック回避用関数
    //void EnemyCollision(ActorBase* actor);

    ////敵のスタック検知用
    //void EnemyStack(ActorBase* actor);


};

