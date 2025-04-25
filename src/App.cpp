#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <SDL.h>
#include <memory>

App::App() {
  LOG_INFO("Game Initialize");

  // 初始化背景
  m_TitleBackground = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/cover.png");
  m_TitleBackground->SetVisible(false);
  m_Root.AddChild(m_TitleBackground);

  m_LevelSelectBackground = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/level-page.png");
  m_LevelSelectBackground->SetVisible(false);
  m_Root.AddChild(m_LevelSelectBackground);

  // 創建關卡按鈕
  // 第一關按鈕
  m_Level1Button = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/current-level.png",
      glm::vec2(0, -220));
  m_Level1Button->SetVisible(false);

  // 設置按鈕點擊事件回調
  m_Level1Button->SetOnClickCallback([this]() {
    ResetGame();
    m_pauseButton->SetVisible(true);

    m_Level1Button->SetVisible(false);
    m_Level2Button->SetVisible(false);
    m_Level3Button->SetVisible(false);
    m_Level4Button->SetVisible(false);
    m_Level5Button->SetVisible(false);
    m_BackButton->SetVisible(false);
    m_LevelSelectBackground->SetVisible(false);

    m_Level1Background->SetVisible(true);
    m_CurrentLevel = 1;
    m_IsGridLoaded = false;
  });

  m_Root.AddChild(m_Level1Button);

  // 第二關按鈕
  m_Level2Button = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/unlevel.png",
      glm::vec2(0, -102));
  m_Level2Button->SetVisible(false);
  m_Level2Button->SetInteractable(false); // 第二關不可互動

  // 設置按鈕點擊事件回調
  m_Level2Button->SetOnClickCallback([this]() {
    ResetGame();
    m_pauseButton->SetVisible(true);

    m_Level1Button->SetVisible(false);
    m_Level2Button->SetVisible(false);
    m_Level3Button->SetVisible(false);
    m_Level4Button->SetVisible(false);
    m_Level5Button->SetVisible(false);
    m_BackButton->SetVisible(false);
    m_LevelSelectBackground->SetVisible(false);

    m_Level2Background->SetVisible(true);
    m_CurrentLevel = 2;
    m_IsGridLoaded = false;
  });

  m_Root.AddChild(m_Level2Button);

  // 第三關按鈕
  m_Level3Button = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/unlevel.png", glm::vec2(0, 16));
  m_Level3Button->SetVisible(false);
  m_Level3Button->SetInteractable(true); // 第三關不可互動

  // 設置按鈕點擊事件回調
  m_Level3Button->SetOnClickCallback([this]() {
    ResetGame();
    m_pauseButton->SetVisible(true);

    m_Level1Button->SetVisible(false);
    m_Level2Button->SetVisible(false);
    m_Level3Button->SetVisible(false);
    m_Level4Button->SetVisible(false);
    m_Level5Button->SetVisible(false);
    m_BackButton->SetVisible(false);
    m_LevelSelectBackground->SetVisible(false);

    m_Level3Background->SetVisible(true);
    m_CurrentLevel = 3;
    m_IsGridLoaded = false;
  });

  m_Root.AddChild(m_Level3Button);

  // 第四關按鈕
  m_Level4Button = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/unlevel.png",
      glm::vec2(0, 134));
  m_Level4Button->SetVisible(false);
  m_Level4Button->SetInteractable(false); // 第四關不可互動

  // 設置按鈕點擊事件回調
  m_Level4Button->SetOnClickCallback([this]() {
    ResetGame();
    m_pauseButton->SetVisible(true);

    m_Level1Button->SetVisible(false);
    m_Level2Button->SetVisible(false);
    m_Level3Button->SetVisible(false);
    m_Level4Button->SetVisible(false);
    m_Level5Button->SetVisible(false);
    m_BackButton->SetVisible(false);
    m_LevelSelectBackground->SetVisible(false);

    m_Level4Background->SetVisible(true);
    m_CurrentLevel = 4;
    m_IsGridLoaded = false;
  });

  m_Root.AddChild(m_Level4Button);

  // 第五關按鈕
  m_Level5Button = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/unlevel.png",
      glm::vec2(0, 252));
  m_Level5Button->SetVisible(false);
  m_Level5Button->SetInteractable(false); // 第五關不可互動

  // 設置按鈕點擊事件回調
  m_Level5Button->SetOnClickCallback([this]() {
    ResetGame();
    m_pauseButton->SetVisible(true);

    m_Level1Button->SetVisible(false);
    m_Level2Button->SetVisible(false);
    m_Level3Button->SetVisible(false);
    m_Level4Button->SetVisible(false);
    m_Level5Button->SetVisible(false);
    m_BackButton->SetVisible(false);
    m_LevelSelectBackground->SetVisible(false);

    m_Level5Background->SetVisible(true);
    m_CurrentLevel = 5;
    m_IsGridLoaded = false;
  });

  m_Root.AddChild(m_Level5Button);

  // 返回按鈕
  m_BackButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/back-button.png",
      glm::vec2(-450, -250));
  m_BackButton->SetVisible(false);

  // 設置按鈕點擊事件回調
  m_BackButton->SetOnClickCallback([this]() {
    m_CurrentState = State::START;
    m_Level1Button->SetVisible(false);
    m_Level2Button->SetVisible(false);
    m_Level3Button->SetVisible(false);
    m_Level4Button->SetVisible(false);
    m_Level5Button->SetVisible(false);
    m_BackButton->SetVisible(false);
    m_LevelSelectBackground->SetVisible(false);
  });

  m_Root.AddChild(m_BackButton);

  // 創建關卡背景
  // 第一關背景
  m_Level1Background = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/rlevel1.png");
  m_Level1Background->SetVisible(false);
  m_Root.AddChild(m_Level1Background);

  // 第二關背景
  m_Level2Background = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/rlevel2.png");
  m_Level2Background->SetVisible(false);
  m_Root.AddChild(m_Level2Background);

  // 第三關背景
  m_Level3Background = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/rlevel3.png");
  m_Level3Background->SetVisible(false);
  m_Root.AddChild(m_Level3Background);

  // 第四關背景
  // 第五關背景

  // Door
  if (!m_Fireboy_Door) {
    m_Fireboy_Door = std::make_shared<Door>(
        RESOURCE_DIR "/material/props/door/door-fireboy-closed.png",
        RESOURCE_DIR "/material/props/door/door-fireboy-half.png",
        RESOURCE_DIR "/material/props/door/door-fireboy-open.png", true, 10);
    m_Fireboy_Door->SetVisible(false);
    m_Root.AddChild(m_Fireboy_Door);
  }

  if (!m_Watergirl_Door) {
    m_Watergirl_Door = std::make_shared<Door>(
        RESOURCE_DIR "/material/props/door/door-watergirl-closed.png",
        RESOURCE_DIR "/material/props/door/door-watergirl-half.png",
        RESOURCE_DIR "/material/props/door/door-watergirl-open.png", false, 10);
    m_Watergirl_Door->SetVisible(false);
    m_Root.AddChild(m_Watergirl_Door);
  }

  m_GameWinBackground = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/game-win.png", 35);
  m_GameWinBackground->SetVisible(false);
  m_Root.AddChild(m_GameWinBackground);

  // 切換到關卡選擇按鈕
  m_ContinueButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/continue-button.png",
      glm::vec2(0, -150));
  m_ContinueButton->SetVisible(false);

  // 設置按鈕點擊事件回調
  m_ContinueButton->SetOnClickCallback([this]() {
    ResetGame();

    m_GameWinBackground->SetVisible(false);
    m_ContinueButton->SetVisible(false);
    m_GemCollectedIndicator->SetVisible(false);
    m_CharacterIndicator->SetVisible(false);
    m_LevelResult->SetVisible(false);
    m_pauseButton->SetVisible(false);

    m_CurrentState = State::LOADING;

    // Reset loading animation
    m_LoadingFrameIndex = 0;
    m_LoadingAnimationTimer = 0.0f;

    m_Level1Background->SetVisible(false);
    m_Level2Background->SetVisible(false);
    m_Level3Background->SetVisible(false);
  });
  m_Root.AddChild(m_ContinueButton);

  // 暫停遊戲按鍵
  m_pauseButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/pause-button.png",
      glm::vec2(440, 320));
  m_pauseButton->SetVisible(false);
  m_Root.AddChild(m_pauseButton);

  m_pauseButton->SetOnClickCallback([this]() {
    m_GamePausedBackground->SetVisible(true);
    m_EndButton->SetVisible(true);
    m_RetryButton->SetVisible(true);
    m_ResumeButton->SetVisible(true);
  });

  m_GamePausedBackground = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/paused-page.png", 35);
  m_GamePausedBackground->SetVisible(false);
  m_Root.AddChild(m_GamePausedBackground);

  // 結束按鈕
  m_EndButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/end-button.png",
      glm::vec2(-130, -50));
  m_EndButton->SetVisible(false);
  m_Root.AddChild(m_EndButton);

  m_EndButton->SetOnClickCallback([this]() {
    ResetGame();

    m_GamePausedBackground->SetVisible(false);
    m_EndButton->SetVisible(false);
    m_RetryButton->SetVisible(false);
    m_ResumeButton->SetVisible(false);
    m_pauseButton->SetVisible(false);

    m_CurrentState = State::LOADING;
    // Reset loading animation
    m_LoadingFrameIndex = 0;
    m_LoadingAnimationTimer = 0.0f;

    m_LevelSelectBackground->SetVisible(true);

    m_Level1Background->SetVisible(false);
    m_Level2Background->SetVisible(false);
    m_Level3Background->SetVisible(false);
  });

  // 重新開始按鈕
  m_RetryButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/retry-button.png",
      glm::vec2(130, -50));
  m_RetryButton->SetVisible(false);
  m_Root.AddChild(m_RetryButton);

  m_RetryButton->SetOnClickCallback([this]() {
    // 重置當前關卡
    ResetGameLevel();

    m_GamePausedBackground->SetVisible(false);
    m_EndButton->SetVisible(false);
    m_RetryButton->SetVisible(false);
    m_ResumeButton->SetVisible(false);
  });

  // 繼續按鈕
  m_ResumeButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/resume-button.png",
      glm::vec2(0, -150));
  m_ResumeButton->SetVisible(false);
  m_Root.AddChild(m_ResumeButton);

  m_ResumeButton->SetOnClickCallback([this]() {
    m_GamePausedBackground->SetVisible(false);
    m_EndButton->SetVisible(false);
    m_RetryButton->SetVisible(false);
    m_ResumeButton->SetVisible(false);
  });

  m_Root.Update();
}

void App::End() { LOG_TRACE("Game End"); }