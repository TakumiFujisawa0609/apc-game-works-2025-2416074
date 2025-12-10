#include "GameOverScene.h"
#include"../Manager/SceneManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/InputManager.h"
#include"../Application.h"
GameOverScene::GameOverScene()
{
	horror_message_ = "心肺停止を確認...";
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Load(void)
{
	image_ = LoadGraph("Data/Image/UI/Black.png");

	text_ = strlen("心肺停止を確認...");

	counter_ = 0;

	// シーン初期化時にタイマーをリセット
	text_anim_timer_ = 0;
	// 文字サイズを設定し、メッセージの最大幅を取得
	SetFontSize(50);
	// メッセージの最大描画幅を計算しておきます
	message_max_width_ = GetDrawStringWidth(horror_message_,text_);

	isText_ = false;

	overText_ = false;
}

void GameOverScene::Init(void)
{
}

void GameOverScene::Update(void)
{
	auto& ins = InputManager::GetInstance();

	//最初の一度だけ再生
	auto& sm = SoundManager::GetInstance();

	sm.Play(SoundManager::SRC::HERATBEAT, Sound::TIMES::ONCE);

	counter_++;

	if (counter_ > 100)
	{
		isText_ = true;
	}

	// 恐怖メッセージのアニメーションを進行させる
	if (text_anim_timer_ < 180)
	{
		text_anim_timer_++;
	}

	
	if (GetJoypadNum() == 0)
	{
		if (ins.IsTrgUp(KEY_INPUT_SPACE))
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			sm.AllStop();
		}
	}
	else
	{
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		// Bボタンが押されたらゲームシーンへ
		if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT))
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			sm.AllStop();
		}
	}

	if (SceneManager::GetInstance().GetSpecialGameOver() == true)
	{
		overText_ = true;
	}
	else
	{
		overText_ = false;
	}
}

void GameOverScene::Draw(void)
{
	SetFontSize(50);

	DrawGraph(0.0, 0.0, image_, true);

	
	int current_width = (message_max_width_ * text_anim_timer_) / 180; // アニメーション進行度に応じて幅を計算
	if (current_width > message_max_width_) {
		current_width = message_max_width_; // 念のため最大幅でクランプ
	}

	RECT draw_area;
	GetDrawArea(&draw_area); // 引数は一つ

	int text_x = 0; // 画面左寄りから開始
	int text_y = 0;

	// 描画範囲を制限（文字が表示されるはずの領域の左端から、現在の表示幅まで）
	SetDrawArea(text_x, text_y, text_x + current_width, text_y + GetFontSize());

	// 制限された描画範囲内で、メッセージ全体を描画する
	DrawString(text_x, text_y, horror_message_, 0xFF0000); // 赤色で強調

	// 描画領域を元に戻す
	SetDrawArea(draw_area.left, draw_area.top, draw_area.right, draw_area.bottom);

	
	if (isText_&&!overText_)
	{
		static int blinkTimer = 0;
		blinkTimer++;
		if ((blinkTimer / 30) % 2 == 0)
		{
			// Title誘導
			DrawString(700, 530, "Space or X Title", 0xFFFFFF);
		}
	}

	if (overText_)
	{
		DrawString(700, 530, "余計な詮索はしないことだな...", 0xFFFFFF);
	}
}

void GameOverScene::Release(void)
{
}
