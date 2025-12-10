#include "Caution.h"
#include"../Manager/SceneManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/SoundManager.h"
Caution::Caution()
{
}

Caution::~Caution()
{
}

void Caution::Load(void)
{
	image_ = LoadGraph("Data/Image/UI/Caution.png");
	sound_ = true;
}

void Caution::Init(void)
{
}

void Caution::Update(void)
{
	if (sound_)
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::DONE, Sound::TIMES::ONCE);
		sound_ = false;
	}
	if (SoundManager::GetInstance().IsEnded(SoundManager::SRC::DONE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

}

void Caution::Draw(void)
{
	DrawGraph(0, 0, image_, false);
}

void Caution::Release(void)
{
}
