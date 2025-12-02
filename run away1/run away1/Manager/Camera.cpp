#include<EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include"../Manager/InputManager.h"
#include "Camera.h"
Camera::Camera(void)
{
	
}
Camera::~Camera()
{
}
void Camera::Init(void)
{
	
	pos_ = DEFAULT_POS;
	angles_ = DEFAULT_ANGLES; 
	mode_ = MODE::FREE;
}
void Camera::Update(void)
{
	SetBeforeDraw();
}
void Camera::SetBeforeDraw(void)
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFixedPoint();
		break;
		return;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	}

	// カメラの設定(位置と角度による制御)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);

	Effekseer_Sync3DSetting();

}
void Camera::Release(void)
{
}

void Camera::ChangeMode(MODE mode)
{
	// カメラモードの変更
	mode_ = mode;
	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::FREE:
		break;
	}

}

void Camera::SetPlayer(Player* player)
{
	player_ = player;
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	if (player_ == nullptr) return;

	VECTOR playerPos = player_->GetPos();
	VECTOR playerRot = player_->GetRot();
	
	// プレイヤーの視点位置（頭の高さ）
	pos_ = VAdd(playerPos,HEAD_POS);

	// Player の回転をカメラ角度に反映
	angles_.x = playerRot.x; // 上下（Pitch）
	angles_.y = playerRot.y; // 左右（Yaw）
	angles_.z = 0.0f;        // Roll は通常 0

	// カメラの向きは SetCameraPositionAndAngle で設定
	SetCameraPositionAndAngle(pos_, angles_.x, angles_.y, angles_.z);

	// Effekseer 用に同期
	Effekseer_Sync3DSetting();
}

void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();
	// WASDでカメラの位置を変える
	float movePow = CAMERA_MOVE_SPEED;
	if (ins.IsNew(KEY_INPUT_W)) { pos_.z += movePow; }
	if (ins.IsNew(KEY_INPUT_A)) { pos_.x -= movePow; }
	if (ins.IsNew(KEY_INPUT_S)) { pos_.z -= movePow; }
	if (ins.IsNew(KEY_INPUT_D)) { pos_.x += movePow; }
	//if (ins.IsNew(KEY_INPUT_Q)) { pos_.y += movePow; }
	//if (ins.IsNew(KEY_INPUT_E)) { pos_.y -= movePow; }
	// 矢印キーでカメラの角度を変える
	float rotPow = CAMERA_ROT_SPEED;
	if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x += rotPow; }
	if (ins.IsNew(KEY_INPUT_UP)) { angles_.x -= rotPow; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }
}
