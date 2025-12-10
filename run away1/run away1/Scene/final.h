#pragma once
#include<Dxlib.h>
#include"../Scene/SceneBase.h"
#include<string>
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
    std::string clearMessage_;
    int messageTimer_;
    int messageSpeed_; 
};

