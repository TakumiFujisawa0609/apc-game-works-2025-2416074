#pragma once
#include<Dxlib.h>
#include"../Scene/SceneBase.h"
class final :public SceneBase
{
public:
    final();
    ~final();
    void Load(void) override;
    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Release(void) override;
private:
    int image_;

};

