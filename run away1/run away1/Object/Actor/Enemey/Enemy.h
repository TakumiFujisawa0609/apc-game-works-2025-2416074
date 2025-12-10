#pragma once
#include<DxLib.h>
#include "../ActorBase.h"

class GameClearScene;
class GameScene;
class Player;
class Enemy : public ActorBase
{
public:
	//列挙
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		WALK,
		MAX
	};
	//定数定義
	//デフォルト座標
	static constexpr VECTOR DEFAULT_POS = { -125.18f,40.0f,-20.60f };
	//デフォルトスケール
	static constexpr VECTOR DEFAULT_SCALE = { 0.025f,0.025f,0.025f };

	//当たり判定用カプセルの始点
	static constexpr VECTOR CAPSULE_START_POS = { 0.0f,3.0f,0.0f };
	//当たり判定用カプセルの終点
	static constexpr VECTOR CAPSULE_END_POS = { 0.0f,0.5f,0.0f };
	//当たり判定用カプセルの半径
	static constexpr float CAPSULE_RADIUS = 0.5f;

	//ゲームオーバー判定距離
	static constexpr float GAMEOVER_DISTANCE = 2.0f;

	//スロースピード
	static constexpr float SLOW_SPEED = 0.2f;

	Enemy();
	~Enemy() override;
	void Init();
	void Load();
	void LoadEnd();
	void Update() override;
	void Draw() override;
	void Release() override;

	VECTOR GetPos();

	bool GetGameOver();
	virtual ACTOR_TYPE GetType() const { return ACTOR_TYPE::ENEMY; }

	void SetGameScene(GameScene* scene) { gameScene_ = scene; }

	void SetGameClearScene(GameClearScene* scene) { clearScene_ = scene; }

	//セッター
	void SetPos(VECTOR pos);

private:
	// リソースロード
	void InitLoad(void) override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;

	//アニメーション初期化
	void InitAnimation(void) override;

	// 初期化後の個別処理
	void InitPost(void) override;

	// 移動処理
	void Move()override;// 入力処理まとめ

	//プレイヤーを追従する
	void FollowPlayer();

	void SlowFollowPlayer();

	//Playerをみつける
	void Search();




private:
	int modelId_;
	bool gameOver_ = false;
	VECTOR rot_;
	VECTOR scale_;
	bool activeFlag_;

	bool isFoundPlayer_ = true; 

	float moveSpeed_ = 0.25f;    
	float searchRange_ = 30.0f; 
	float dictanceToPlayer_ = 0.0f;
	float dictanceSE = 0.0f;
	float volume_ = 3.0f;

	//アニメーション制御
	AnimationController* animationController_;
	//ゲームシーン
	GameScene* gameScene_;
	//クリアシーン
	GameClearScene* clearScene_;

	bool isEnemySoundPlayed_; // SRC::ENEMY の再生開始フラグ
	bool isBGMSoundPlayed_;   // SRC::BGM の再生開始フラグ

};

