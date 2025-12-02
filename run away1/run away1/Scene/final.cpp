#include "final.h"
#include"../Manager/SceneManager.h"
#include"../Manager/InputManager.h"

final::final()
{
}

final::~final()
{
}

void final::Load(void)
{
	image_ = LoadGraph("Data/escaped.png");
}

void final::Init(void)
{
}

void final::Update(void)
{
	auto& ins = InputManager::GetInstance();

	if (GetJoypadNum() == 0)
	{
		if (ins.IsTrgUp(KEY_INPUT_SPACE))
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
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
		}
	}

}

void final::Draw(void)
{
	DrawGraph(0, 0, image_, false);  
	SetFontSize(50);
	static int blinkTimer = 0;
	blinkTimer++;
	if ((blinkTimer / 30) % 2 == 0)
	{
		DrawString(800, 500, "GameClear！", 0xFFFFFF);
	}
	if ((blinkTimer / 30) % 2 == 0)
	{
		DrawString(800, 610, "Space or X Title", 0xFFFFFF);
	}
}

void final::Release(void)
{
}
