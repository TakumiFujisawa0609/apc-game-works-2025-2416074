#pragma once
#include "SceneBase.h"

class SceneManager;
class TitleScene : public SceneBase
{
public:
	

	// コンストラクタ
	TitleScene(void);
	// デストラクタ
	~TitleScene(void)override;

	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;
private:
	// シーンの状態
	enum class STATE
	{
		WAIT,   // 入力待ち
		SHAKE,  // 揺れ演出中
		END     // 終了（次のシーンへ遷移）
	};

	STATE state_;
	int timer_;

	int image_;
	int kizu_;

	bool waitSound_;
};

