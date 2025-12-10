    #include "final.h"
    #include"../Manager/SceneManager.h"
    #include"../Manager/InputManager.h"
    #include"../Manager/SoundManager.h"
    #include<string>
    final::final()
    {
    }

    final::~final()
    {
    }

    void final::Load(void)
    {
	    image_ = LoadGraph("Data/Image/UI/Black.png");

	    clearMessage_ = "Ç®ÇﬂÇ≈Ç∆Ç§\nGameClearÇæ";
	    messageTimer_ = 0;
	    messageSpeed_ = 6;

		
    }

    void final::Init(void)
    {
    }

    void final::Update(void)
    {
	    messageTimer_++;

	    auto& ins = InputManager::GetInstance();

        SoundManager::GetInstance().Play(SoundManager::SRC::CLEARD, Sound::TIMES::LOOP);

	    if (GetJoypadNum() == 0)
	    {
		    if (ins.IsTrgUp(KEY_INPUT_SPACE))
		    {
			    SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
				SoundManager::GetInstance().AllStop();
		    }
	    }
	    else
	    {
		    // ê⁄ë±Ç≥ÇÍÇƒÇ¢ÇÈÉQÅ[ÉÄÉpÉbÉhÇPÇÃèÓïÒÇéÊìæ
		    InputManager::JOYPAD_IN_STATE padState =
			    ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		    // BÉ{É^ÉìÇ™âüÇ≥ÇÍÇΩÇÁÉQÅ[ÉÄÉVÅ[ÉìÇ÷
		    if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT))
		    {
			    SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
                SoundManager::GetInstance().AllStop();
		    }
	    }

    }

    void final::Draw(void)
    {

        DrawGraph(0, 0, image_, false);

        int windowX = 500;
        int windowY = 480;
        int windowW = 900;
        int windowH = 200;

        // ògê¸ÇÃêFÅiîíÅj
        unsigned int frameColor = GetColor(255, 255, 255);
        // îwåiÇÃêFÅiîZÇ¢ê¬Åj
        unsigned int backgroundColor = GetColor(10, 10, 50);

        // ògê¸ÇÃï`âÊÅiíÜÇìhÇËÇ¬Ç‘Ç≥Ç»Ç¢ DrawBox ÇÃóòópÅj
        DrawBox(windowX - 2, windowY - 2, windowX + windowW + 2, windowY + windowH + 2, frameColor, FALSE);

        // îºìßñæÇÃîwåiÇï`âÊ
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200); // îºìßñæÉÇÅ[Éh
        DrawBox(windowX, windowY, windowX + windowW, windowY + windowH, backgroundColor, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // å≥Ç…ñﬂÇ∑

        SetFontSize(32);

        int charCount = messageTimer_ / messageSpeed_;
        if (charCount > clearMessage_.size()) {
            charCount = clearMessage_.size();
        }

     
        int x = windowX+300;  
        int y = windowY + 40; 
        int lineHeight = 40;

        
       
        std::string currentLine;
        int currentY = y;
        int drawCount = 0;

        for (size_t i = 0; i < clearMessage_.length(); ++i)
        {
            if (drawCount >= charCount) {
                break;
            }

            if (clearMessage_[i] == '\n') {
                DrawString(x, currentY, currentLine.c_str(), 0xFFFFFF);

                currentLine.clear();
                currentY += lineHeight;
            }
            else {
                currentLine += clearMessage_[i];
            }

            drawCount++;
        }

        if (!currentLine.empty()) {
            DrawString(x, currentY, currentLine.c_str(), 0xFFFFFF);
        }

        
        if (charCount == clearMessage_.size()) {
            static int blinkTimer = 0;
            blinkTimer++;
            if ((blinkTimer / 30) % 2 == 0) {
               
                DrawString(700, 730, "Press Space or X Å® Title", 0xFFFFFF);
            }
        }
    }

    void final::Release(void)
    {
    }
