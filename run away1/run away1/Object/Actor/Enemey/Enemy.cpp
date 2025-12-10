#include "Enemy.h"
#include"../../../Utility/AsoUtility.h"
#include"../../Actor/Player/Player.h"
#include"../../../Object/AnimationController.h"
#include"../../../Scene/GameScene.h"
#include"../../../Scene/GameClearScene.h"
#include"../../../Manager/SoundManager.h"
#include"../../../Application.h"
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	// Transform初期化
	InitTransform();
	// アニメーションの初期化
	InitAnimation();
	// 初期化後の個別処理
	InitPost();
	
}

void Enemy::Load()
{
	// リソースロード
	InitLoad();
}

void Enemy::LoadEnd()
{
	Init();
}

void Enemy::Update()
{
	dictanceSE = volume_ - (dictanceToPlayer_ / 10.0f);
	if (dictanceSE < 0.5f)
	{
		dictanceSE = 0.5f;
	}
	animationController_->Update();

	Search();

	auto& sm = SoundManager::GetInstance(); // SoundManagerのインスタンスを取得

	

	if (isFoundPlayer_)
	{  
		Move();
		sm.ChengeVolume(SoundManager::SRC::ENEMY, dictanceSE);
		sm.Play(SoundManager::SRC::ENEMY, Sound::TIMES::LOOP);
		sm.Play(SoundManager::SRC::RUN_BGM, Sound::TIMES::LOOP);
		

		animationController_->Play(static_cast<int>(ANIM_TYPE::RUN), true);
	}
	else
	{
		sm.Stop(SoundManager::SRC::ENEMY);
		sm.Stop(SoundManager::SRC::RUN_BGM);

		
		SlowFollowPlayer(); // SlowFollowPlayerの処理
		animationController_->Play(static_cast<int>(ANIM_TYPE::WALK), true);
	}
	

	MV1SetPosition(modelId_, pos_);
	MV1SetRotationXYZ(modelId_, rot_);
	MV1SetScale(modelId_, scale_);
}

void Enemy::Draw()
{
	ActorBase::Draw();

	//DrawFormatString(0, 60, 0xFFFFFF, "distance:(%.2f)",dictanceSE);

	if (isFoundPlayer_)
	{
		DrawString(800, 50, "来てるぞ..!", 0xFFFFFF);
	}
	else
	{
		DrawString(800, 50, "探っている...", 0xFFFFFF);
	}
	MV1DrawModel(modelId_);

}

void Enemy::Release()
{
	MV1DeleteModel(modelId_);
	// アニメーション制御の解放
	if (animationController_ != nullptr)
	{
		animationController_->Release();
		delete animationController_;
		animationController_ = nullptr;
	}
}

VECTOR Enemy::GetPos()
{
	return pos_;
}

bool Enemy::GetGameOver()
{
	return gameOver_;
}

void Enemy::InitLoad(void)
{
	modelId_ = MV1LoadModel("Data/Model/Enemy/Enemy.mv1");
}

void Enemy::InitTransform(void)
{
	pos_ = DEFAULT_POS;
	rot_ = VGet(0.0f, AsoUtility::Deg2RadF(-90.0), 0.0f);
	scale_ = DEFAULT_SCALE;
	
	activeFlag_ = true;

	// 当たり判定用カプセルの始点・終点・半径の初期化
	startCapsulePos_ = CAPSULE_START_POS;
	endCapsulePos_ = CAPSULE_END_POS;
	capsuleRadius_ =CAPSULE_RADIUS;

	MV1SetPosition(modelId_, pos_);
	MV1SetRotationXYZ(modelId_, rot_);
	MV1SetScale(modelId_, scale_);

	isEnemySoundPlayed_ = false;
	isBGMSoundPlayed_ = false;
}

void Enemy::InitAnimation(void)
{
	// アニメーション制御の初期化

	animationController_ = new AnimationController(modelId_);

	// アニメーションの追加
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 25.0f, "Data/Model/Enemy/EnemyIdle.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::RUN), 25.0f, "Data/Model/Enemy/EnemyrRun.mv1");
	animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), 25.0f, "Data/Model/Enemy/Walk.mv1");

	//初期アニメーションは待機状態

	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

void Enemy::InitPost(void)
{
}

void Enemy::Move()
{
	FollowPlayer();
}

void Enemy::FollowPlayer()
{
	if (gameScene_ == nullptr) return;
	Player* player = gameScene_->GetPlayer();
	if (player == nullptr) return;

	VECTOR playerPos = player->GetPos();
	VECTOR toPlayer = VSub(playerPos, pos_);

	float distance = sqrtf(toPlayer.x * toPlayer.x + toPlayer.z * toPlayer.z);
	if (distance < GAMEOVER_DISTANCE)
	{
		gameOver_ = true;
	}
	// 正規化（方向ベクトル）
	toPlayer = VNorm(toPlayer);

	// Y回転だけ求める
	float targetRotY = atan2f(toPlayer.x, toPlayer.z);

	//モデルの正面補正
	targetRotY += DX_PI; 
	//スムーズ回転（補間）
	float diff = targetRotY - rot_.y;
	if (diff > DX_PI) diff -= DX_TWO_PI;
	if (diff < -DX_PI) diff += DX_TWO_PI;
	rot_.y += diff * 0.1f; // ← 徐々に向きを合わせる

	// ★ゆっくり移動（1フレームごとに少しずつ）
	pos_.x += toPlayer.x * moveSpeed_;
	pos_.z += toPlayer.z * moveSpeed_;
}

void Enemy::SlowFollowPlayer()
{
	if (gameScene_ == nullptr) return;
	Player* player = gameScene_->GetPlayer();
	if (player == nullptr) return;

	VECTOR playerPos = player->GetPos();
	VECTOR toPlayer = VSub(playerPos, pos_);

	// Low Speed Move Factor
	const float SLOW_FACTOR = SLOW_SPEED; // 1/5の速度

	float distance = sqrtf(toPlayer.x * toPlayer.x + toPlayer.z * toPlayer.z);

	// 低速移動中でも、十分に近づいたらゲームオーバーにする
	if (distance < GAMEOVER_DISTANCE)
	{
		gameOver_ = true;
	}

	// ゲームオーバーになったらそれ以上移動しない
	if (gameOver_) return;

	// 正規化（方向ベクトル）
	toPlayer = VNorm(toPlayer);

	// Y回転だけ求める
	float targetRotY = atan2f(toPlayer.x, toPlayer.z);

	// モデルの正面補正
	targetRotY += DX_PI; // 180度反転

	// スムーズ回転（補間）
	float diff = targetRotY - rot_.y;
	if (diff > DX_PI) diff -= DX_TWO_PI;
	if (diff < -DX_PI) diff += DX_TWO_PI;
	rot_.y += diff * 0.1f; // ← 徐々に向きを合わせる

	// ゆっくり移動（元の速度の 1/5）
	float slowSpeed = moveSpeed_ * SLOW_FACTOR;
	pos_.x += toPlayer.x * slowSpeed;
	pos_.z += toPlayer.z * slowSpeed;
}


void Enemy::Search()
{
	if (gameScene_ == nullptr) return;

	Player* player = gameScene_->GetPlayer();
	if (player == nullptr) return;

	VECTOR playerPos = player->GetPos();
	VECTOR toPlayer = VSub(playerPos, pos_);

	dictanceToPlayer_ = VSize(toPlayer);

	if (dictanceToPlayer_ <= searchRange_)
	{
		// 発見
		isFoundPlayer_ = true;
	}
	else
	{
		// 範囲外なら見失う
		isFoundPlayer_ = false;
	}
}

void Enemy::SetPos(VECTOR pos)
{
	pos_ = pos;
	MV1SetPosition(modelId_, pos_);
}
 