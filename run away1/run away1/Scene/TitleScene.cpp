#include "TitleScene.h"
#include"../Manager/SceneManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/SoundManager.h"
#include"../Application.h"
#include <DxLib.h>

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Load(void)
{
	image_ = LoadGraph("Data/Image/Scenes/Title(new).png");
	kizu_ = LoadGraph("Data/Kizuato.png");

	waitSound_ = false;

	// 初期状態は入力待ち
	state_ = STATE::WAIT;
	timer_ = 0;
	

}

void TitleScene::Init(void)
{
	
}

void TitleScene::Update(void)
{

	auto& ins = InputManager::GetInstance();
	

	// ゲームシーンへ
	SoundManager::GetInstance().Play(SoundManager::SRC::TITLE, Sound::TIMES::LOOP);
	if (GetJoypadNum() == 0)
	{
		if (ins.IsTrgDown(KEY_INPUT_SPACE))
		{
			//SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::LOAD);
			SoundManager::GetInstance().Play(SoundManager::SRC::GOGAME, Sound::TIMES::ONCE);
			waitSound_ = true;
		}
	}
	else
	{
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		// Bボタンが押されたらゲームシーンへ
		if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
		{
			//SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::LOAD);
			SoundManager::GetInstance().Play(SoundManager::SRC::GOGAME, Sound::TIMES::ONCE);
			waitSound_ = true;
		}
	}

	//GOGAMEの音が終わったらシーン遷移
	if (waitSound_ && SoundManager::GetInstance().IsEnded(SoundManager::SRC::GOGAME))
	{
		waitSound_ = false;
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::LOAD);
		SoundManager::GetInstance().AllStop();
	}
}

void TitleScene::Draw(void)
{
	DrawGraph(0, 0, image_, true);

	// "RUN AWAY" の表示と揺れ
	static int frameCounter = 0;
	frameCounter++;

	// 文字の揺れの振幅と速度を調整
	float swingX = sinf(frameCounter * 0.1f) * 5.0f; // 左右の揺れ
	float swingY = cosf(frameCounter * 0.08f) * 3.0f; // 上下の揺れ

	// 画像の中心に文字が来るように座標を調整
	// 画面サイズ1920x1080、デフォルトのフォントサイズ50で「RUN AWAY」の文字幅を仮定して調整
	// 実際には文字の幅を測って正確に調整してください
	int textX = 1500 / 2 + (int)swingX; // 1文字あたり約10ピクセル幅と仮定
	int textY = 200 + (int)swingY; // 画像のRUN AWAYがあった位置を参考に調整

	SetFontSize(120); 
	//SetFontEdgeType(DX_FONTTYPE_ANTIALIASING_8X8); // アンチエイリアスを適用して滑らかに

	// 文字の色を赤色に
	int redColor = GetColor(255, 0, 0);
	DrawString(textX, textY, "RUN AWAY", redColor);


	// Press Space Key to Startを点滅させて表示
	static int blinkTimer = 0;
	blinkTimer++;
	if ((blinkTimer / 30) % 2 == 0) {
		SetFontSize(50); // 他のテキストのフォントサイズに戻す
		//SetFontEdgeType(DX_FONTTYPE_NORMAL); // アンチエイリアスを元に戻す
		DrawString(650, 800, "<Press Space or B button>", 0xFFFFFF);
	}
}

void TitleScene::Release(void)
{
	
}
