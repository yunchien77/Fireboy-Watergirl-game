#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <SDL.h>
#include <memory>

App::App() {
  LOG_TRACE("Game Initialize");

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
  // m_Level1Button->SetOnClickCallback([this]() {
  //   m_CurrentState = State::GAME_PLAY;
  //   m_Level1Button->SetVisible(false);
  //   m_Level2Button->SetVisible(false);
  //   m_Level3Button->SetVisible(false);
  //   m_Level4Button->SetVisible(false);
  //   m_Level5Button->SetVisible(false);
  //   m_BackButton->SetVisible(false);
  //   m_LevelSelectBackground->SetVisible(false);
  //   m_Level1Background->SetVisible(true);
  // });
  m_Level1Button->SetOnClickCallback([this]() {
    ResetGame();

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
    m_CurrentState = State::GAME_PLAY;
    m_Level1Button->SetVisible(false);
    m_Level2Button->SetVisible(false);
    m_Level3Button->SetVisible(false);
    m_Level4Button->SetVisible(false);
    m_Level5Button->SetVisible(false);
    m_BackButton->SetVisible(false);
    m_LevelSelectBackground->SetVisible(false);
    m_Level2Background->SetVisible(true);
  });

  m_Root.AddChild(m_Level2Button);

  // 第三關按鈕
  m_Level3Button = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/unlevel.png", glm::vec2(0, 16));
  m_Level3Button->SetVisible(false);
  m_Level3Button->SetInteractable(false); // 第三關不可互動

  // 設置按鈕點擊事件回調
  m_Level3Button->SetOnClickCallback([this]() {
    m_CurrentState = State::GAME_PLAY;
    m_Level1Button->SetVisible(false);
    m_Level2Button->SetVisible(false);
    m_Level3Button->SetVisible(false);
    m_Level4Button->SetVisible(false);
    m_Level5Button->SetVisible(false);
    m_BackButton->SetVisible(false);
    m_LevelSelectBackground->SetVisible(false);
    m_Level3Background->SetVisible(true);
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
    m_CurrentState = State::GAME_PLAY;
    m_Level1Button->SetVisible(false);
    m_Level2Button->SetVisible(false);
    m_Level3Button->SetVisible(false);
    m_Level4Button->SetVisible(false);
    m_Level5Button->SetVisible(false);
    m_BackButton->SetVisible(false);
    m_LevelSelectBackground->SetVisible(false);
    m_Level4Background->SetVisible(true);
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
    m_CurrentState = State::GAME_PLAY;
    m_Level1Button->SetVisible(false);
    m_Level2Button->SetVisible(false);
    m_Level3Button->SetVisible(false);
    m_Level4Button->SetVisible(false);
    m_Level5Button->SetVisible(false);
    m_LevelSelectBackground->SetVisible(false);
    m_Level5Background->SetVisible(true);
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
  // 第三關背景
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
  m_GameWinBackground->SetVisible(false); // Hide initially
  m_Root.AddChild(m_GameWinBackground);

  // 切換到關卡選擇按鈕
  m_ContinueButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/continue-button.png",
      glm::vec2(0, -150));
  m_ContinueButton->SetVisible(false);

  // 設置按鈕點擊事件回調
  m_ContinueButton->SetOnClickCallback([this]() {
    ResetGame();

    m_CurrentState = State::LEVEL_SELECT;
    m_Level1Button->SetVisible(true);
    m_Level2Button->SetVisible(true);
    m_Level3Button->SetVisible(true);
    m_Level4Button->SetVisible(true);
    m_Level5Button->SetVisible(true);
    m_BackButton->SetVisible(true);
    m_LevelSelectBackground->SetVisible(true);

    m_Level1Background->SetVisible(false);
    // m_Level2Background->SetVisible(false);
    // m_Level3Background->SetVisible(false);
    // m_Level4Background->SetVisible(false);
    // m_Level5Background->SetVisible(false);
  });

  m_Root.AddChild(m_ContinueButton);

  m_Root.Update();
}

void App::End() { LOG_TRACE("Game End"); }