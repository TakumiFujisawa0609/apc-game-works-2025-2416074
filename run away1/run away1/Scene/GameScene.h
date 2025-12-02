#pragma once

#include <vector>

#include "../Scene/SceneBase.h"
#include<Dxlib.h>
class NightDoom;
class Enemy;
class Door;
class Item;
class Stage;
class Player;
class Camera;
class ActorBase;


class GameScene : public SceneBase
{
public:
    //定数定義
    static constexpr int ENEMY_STATCK_TIME = 30;
    GameScene();
    ~GameScene() override;

    void Load(void) override;
    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Release(void) override;

    // プレイヤー取得関数（ActorBaseを走査して見つける）
    Player* GetPlayer();

    Enemy* GetEnemy();

    //ゲームクリア判定
    bool CheckGameClear();



private:
   

    Stage* stage_ = nullptr;
    Player* player_ = nullptr;
    Camera* camera_ = nullptr;
    Item* item_ = nullptr;
	Door* door_ = nullptr;
	Enemy* enemy_ = nullptr;
    NightDoom* nDoom = nullptr;

    int startTime_;

    //デバッグ用当たり判定
	bool OkCollision_ = true;

    bool enemyStack_ = false;

    bool clear_;

    //カウントダウン
    int countDown_;

    //すべてのアクター
    std::vector<ActorBase*>allActor_;

    // 衝突判定(床)
    void FieldCollision(ActorBase* actor);

    //当たり判定
    void WallCollision(ActorBase* actor);

    //当たり判定(ドア)
	void DoorCollision(ActorBase* actor);

	// アイテム取得判定
	void ItemCollision(ActorBase* actor);
    void ItemCollision2(ActorBase* actor);
    void ItemCollision3(ActorBase* actor);
    void ItemCollision4(ActorBase* actor);

    //敵のスタック回避用関数
    void EnemyCollision(ActorBase* actor);

    //敵のスタック検知用
    void EnemyStack(ActorBase* actor);

    // 敵のスタック解除関連のフラグとカウンタ
    bool isEnemyStuck_ = false; // 敵が壁にスタックしているかを検知するフラグ
    int stuckTimer_ = 0;        // スタック開始からの時間
    int noCollisionTimer_ = 0;  // 当たり判定無効期間のタイマー
    bool isNoCollisionMode_ = false; // 当たり判定無効モードかどうかのフラグ

    bool isPaused_ = false;        // ゲームポーズ状態 (メニュー表示) フラグ
    int menuSelection_ = 0;        // メニュー選択肢 (0: 続ける, 1: 終了)

    void DrawMenu();               // メニュー画面の描画処理
};
