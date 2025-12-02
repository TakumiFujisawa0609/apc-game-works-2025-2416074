#pragma once
#include <DxLib.h>

class Stage
{
public:


	Stage(void);
	~Stage(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);	

	int GetModelId()const;


private:

	int modelId;
	VECTOR pos_;
	VECTOR scale_;
};

