#pragma once
#include"../Object/Actor/Player/Player.h"
#include"../Utility/AsoUtility.h"
#include<Dxlib.h>

class Camera
{
public:
	// Player.hから移動させた定数をこちらに定義するか、Utilityなどに定義します
#define FLASHLIGHT_OUT_ANGLE (AsoUtility::Deg2RadF(20.0f)) 
#define FLASHLIGHT_IN_ANGLE  (AsoUtility::Deg2RadF(15.0f))
#define FLASHLIGHT_RANGE     30.0f


	// カメラモード
	enum class MODE
	{
		NONE,
		FOLLOW, // 定点カメラ
		FREE, // フリーモード
	};

	// カメラの初期位置
	static constexpr VECTOR DEFAULT_POS =
	{ -78.47f, 42.43f, -5.0f };
	// カメラの初期角度	
	static constexpr VECTOR DEFAULT_ANGLES =
	{ -20.0f * DX_PI_F / 180.0f, 0.0f, 0.0f };
	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 0.5f;
	// カメラクリップ：FAR
	static constexpr float CAMERA_FAR = 15000.0f;
	//カメラの高さ
	static constexpr VECTOR HEAD_POS = { 0.0f,0.1f,0.0f };

	//カメラの進む速さ
	static constexpr float CAMERA_MOVE_SPEED = 0.1f;
	//カメラの回転速度
	static constexpr float CAMERA_ROT_SPEED = 1.0f * DX_PI_F / 180.0f;
	// コンストラクタ
	Camera(void);
	// デストラクタ
	~Camera(void);
	// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画前のカメラ設定
	void SetBeforeDraw(void);
	// 解放
	void Release(void);

	// カメラモードの変更
	void ChangeMode(MODE mode);
	//セッター
	void SetPlayer(Player* player);

private:
	// カメラモード
	MODE mode_;
	// カメラの位置
	VECTOR pos_;
	// カメラの角度
	VECTOR angles_;
	// カメラに連動したライトハンドル
	int flashlightHandle_ = -1;

	// カメラモード別の制御処理
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);

	Player* player_=nullptr;
};

