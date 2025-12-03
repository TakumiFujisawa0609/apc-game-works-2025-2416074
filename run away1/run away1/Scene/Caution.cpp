#include "Caution.h"
#include"../Manager/SceneManager.h"
#include"../Manager/InputManager.h"

Caution::Caution()
{
}

Caution::~Caution()
{
}

void Caution::Load(void)
{
	image_ = LoadGraph("Data/Image/UI/Caution.png");
	counter_ = 0.0f;
}

void Caution::Init(void)
{
}

void Caution::Update(void)
{
	counter_++;
	if (counter_ >= 180.0f)
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
