#pragma once
#include<DxLib.h>
#include"../ActorBase.h"
#include"../../../Utility/AsoUtility.h"

class Camera;
class Player : public ActorBase
{
public:

	//定数定義
	const VECTOR INITPOS = VGet(-78.47f, 42.50f, -5.0f);
	const float MOVE_SPEED = 0.22f;
	const float MOUSE_SENSITIVITY = 0.0005f;
	const float JUMP_POW = 0.5f;
	static constexpr int STAMINA_BAR_X = 1000;    // 画面右上のX座標
	static constexpr int STAMINA_BAR_Y = 20;     // 画面上部からのY座標
	static constexpr int STAMINA_BAR_WIDTH = 250; // ゲージの最大幅
	static constexpr int STAMINA_BAR_HEIGHT = 20; // ゲージの高さ
	static constexpr int STAMINA_BAR_THICKNESS = 2; // ゲージの枠の太さ


	// 色の定義 (DxLibのカラーコード)
	static constexpr unsigned int COLOR_GREEN = 0x00FF00;
	static constexpr unsigned int COLOR_YELLOW = 0x00FFFF00;
	static constexpr unsigned int COLOR_RED = 0xFF0000;
	static constexpr unsigned int COLOR_BLACK = 0x000000;
	static constexpr unsigned int COLOR_WHITE = 0xFFFFFF;

	Player();
	~Player()override;
	void Load();
	void LoadEnd();
	void Init();
	void Update()override;
	void Draw()override;
	void Release()override;

	void OnGetItem(); 
	bool HasItem() const { return hasItem_; }

	//ゲッター
	VECTOR GetPos();
	VECTOR GetRot();
	bool GetClear();
	virtual ACTOR_TYPE GetType() const { return ACTOR_TYPE::PLAYER; }

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

	//VECTOR GetModelBoundingBox(int modelHandle);

private:

	VECTOR rot_;
	VECTOR moveDir_;      // 移動方向
	float moveSpeed_;     // 移動速度
	float mouseSensitivity_; // マウス感度

	bool isGrounded_;   // 地面に接地しているか
	float verticalSpeed_ = 0.0f;  // 上下方向の速度
	const float gravity_ = 0.03f; // 重力加速度

	//ゲームクリア判定用変数
	VECTOR gameClearPos_;
	bool gameClear_;

	bool hasItem_ = false;
	//カメラ
	Camera* camera_;
	int countDown_;

	//ダッシュ用
	float currentStamina_;     // 現在のスタミナ
	float maxStamina_;         // 最大スタミナ
	float staminaConsumptionRate_; // 1秒あたりのスタミナ消費量 
	float staminaRecoveryRate_;    // 1秒あたりのスタミナ回復量 
	float dashSpeedMultiplier_; // ダッシュ時の速度倍率
	float baseMoveSpeed_;      // 基本の移動速度を保持
	bool isDashing_;           // ダッシュ中フラグ
	bool wasDashingLastFrame_ = false;
	float staminaCoolDownTimer_ = 0.0f;
	static constexpr float STAMINA_COOLDOWN_DURATION = 1.0f; // 回復が始まるまでのクールタイム(秒)
	//UI用
	bool announce;

};

