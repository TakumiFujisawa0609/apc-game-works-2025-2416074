#include <DxLib.h>
#include "../Application.h"
#include "SoundManager.h"
#include"../Object/Actor/Enemey/Enemy.h"
SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager();
	}
	instance_->Init();
}

SoundManager& SoundManager::GetInstance(void)
{
	return *instance_;
}

void SoundManager::Init(void)
{

	Sound res;

	res = Sound(Sound::TYPE::SOUND_2D,"Data/BGM/Run.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::RUN, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/walk.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::WALK, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/Dash.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::DASH, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/EnemyBoice.mp3");
	res.ChengeMaxVolume(2.0);
	soundMap_.emplace(SRC::ENEMY, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/run away.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::BGM, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/KeyGet.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::KEY, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/grassWalk.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::GRASS_WALK, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/grassRun.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::GRASS_RUN, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/heartBeat.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::HERATBEAT, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/monster.mp3");
	res.ChengeMaxVolume(2.0);
	soundMap_.emplace(SRC::GAMEOVER, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/RunBGM.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::RUN_BGM, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/dontOpen.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::CLOSED, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/MENU_CARSOL.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::CARSOL, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/TITLE.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::TITLE, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/GOGAME.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::GOGAME, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/BLACE_LIMIT.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::BLACEOUT, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/CLEARD.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::CLEARD, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/DONE.mp3");
	res.ChengeMaxVolume(0.8);
	soundMap_.emplace(SRC::DONE, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/Start.mp3");
	res.ChengeMaxVolume(1.2);
	soundMap_.emplace(SRC::START, res);

	res = Sound(Sound::TYPE::SOUND_2D, "Data/BGM/ClearScene.mp3");
	res.ChengeMaxVolume(1.2);
	soundMap_.emplace(SRC::CLEARSCENE, res);
}

void SoundManager::Release(void)
{
	for (auto& p : soundMap_)
	{
		p.second.Release();  // サウンドのリソースを解放

		// p.second が動的に確保されたオブジェクトであれば、以下のようにメモリ解放
		// delete p.second; // 必要に応じて、メモリを解放する

	}

	soundMap_.clear();  // マップをクリア
}

void SoundManager::Destroy(void)
{
	Release();
	soundMap_.clear();
	delete instance_;
}

//音の再生
bool SoundManager::Play(SRC src, Sound::TIMES times, bool isForce)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())
	{
		if (!lPair->second.CheckLoad())
		{
			lPair->second.Load();
		}
		return lPair->second.Play(times, isForce);
	}
	return false;
}
bool SoundManager::Play(SRC src, Sound::TIMES times, VECTOR pos, float radius)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())
	{
		if (!lPair->second.CheckLoad())
		{
			lPair->second.Load();
		}
		return lPair->second.Play(pos, radius, times);
	}
	return false;
}

void SoundManager::Stop(SRC src)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())//指定のものが存在するか？
	{
		return lPair->second.Stop();
	}
}
void SoundManager::AllStop(void)
{
	// soundMap_ のすべての要素に対して Stop() を呼び出す
	for (auto& lPair : soundMap_)
	{
		lPair.second.Stop();
	}
}
//再生中か?
bool SoundManager::CheckMove(SRC src)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())
	{
		return lPair->second.CheckMove();
	}
	return false;
}
//音量
void SoundManager::ChengeVolume(SRC src, float per)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())
	{
		return lPair->second.ChengeVolume(per);
	}
}

bool SoundManager::IsEnded(SRC src)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())
	{
		return lPair->second.IsEnded();
	}
	return true;  // 存在しなければ再生終了
}
