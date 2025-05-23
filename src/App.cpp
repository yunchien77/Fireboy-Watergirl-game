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

  InitializeBackgrounds();
  InitializeLevelButtons();
  InitializeDoors();
  InitializeGameUI();
  InitializePauseMenu();

  m_Root.Update();
}

void App::InitializeBackgrounds() {
  // 初始化遊戲開始背景
  m_TitleBackground = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/cover.png");
  m_TitleBackground->SetVisible(false);
  m_Root.AddChild(m_TitleBackground);

  // 關卡選擇背景
  m_LevelSelectBackground = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/level-page.png");
  m_LevelSelectBackground->SetVisible(false);
  m_Root.AddChild(m_LevelSelectBackground);

  // 初始化各關卡背景
  InitializeLevelBackgrounds();

  // 遊戲獲勝背景
  m_GameWinBackground = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/game-win.png", 35);
  m_GameWinBackground->SetVisible(false);
  m_Root.AddChild(m_GameWinBackground);

  // 遊戲暫停背景
  m_GamePausedBackground = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/paused-page.png", 35);
  m_GamePausedBackground->SetVisible(false);
  m_Root.AddChild(m_GamePausedBackground);
}

void App::InitializeLevelBackgrounds() {
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
  m_Level4Background = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/rlevel4.png");
  m_Level4Background->SetVisible(false);
  m_Root.AddChild(m_Level4Background);

  // 第五關背景
  m_Level5Background = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/rlevel5.png");
  m_Level5Background->SetVisible(false);
  m_Root.AddChild(m_Level5Background);
}

void App::InitializeLevelButtons() {
  // 第一關按鈕
  m_Level1Button = CreateLevelButton(
      RESOURCE_DIR "/material/background/button/current-level.png",
      glm::vec2(0, -220), 1, true);
  m_Root.AddChild(m_Level1Button);

  // 第二關按鈕
  m_Level2Button =
      CreateLevelButton(RESOURCE_DIR "/material/background/button/unlevel.png",
                        glm::vec2(0, -102), 2, true);
  m_Root.AddChild(m_Level2Button);

  // 第三關按鈕
  m_Level3Button =
      CreateLevelButton(RESOURCE_DIR "/material/background/button/unlevel.png",
                        glm::vec2(0, 16), 3, true);
  m_Root.AddChild(m_Level3Button);

  // 第四關按鈕
  m_Level4Button =
      CreateLevelButton(RESOURCE_DIR "/material/background/button/unlevel.png",
                        glm::vec2(0, 134), 4, true);
  m_Root.AddChild(m_Level4Button);

  // 第五關按鈕
  m_Level5Button =
      CreateLevelButton(RESOURCE_DIR "/material/background/button/unlevel.png",
                        glm::vec2(0, 252), 5, true);
  m_Root.AddChild(m_Level5Button);

  // 返回按鈕
  m_BackButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/back-button.png",
      glm::vec2(-450, -250));
  m_BackButton->SetVisible(false);
  m_BackButton->SetOnClickCallback([this]() {
    m_CurrentState = State::START;
    SetLevelSelectVisible(false);
  });
  m_Root.AddChild(m_BackButton);
}

std::shared_ptr<Option> App::CreateLevelButton(const std::string &imagePath,
                                               const glm::vec2 &position,
                                               int levelNumber,
                                               bool interactable) {
  auto button = std::make_shared<Option>(imagePath, position);
  button->SetVisible(false);
  button->SetInteractable(interactable);

  button->SetOnClickCallback([this, levelNumber]() {
    ResetGame();
    m_pauseButton->SetVisible(true);

    SetLevelSelectVisible(false);

    // 顯示對應關卡背景
    ShowLevelBackground(levelNumber);

    m_CurrentLevel = levelNumber;
    m_IsGridLoaded = false;
  });

  return button;
}

void App::ShowLevelBackground(int levelNumber) {
  m_Level1Background->SetVisible(levelNumber == 1);
  m_Level2Background->SetVisible(levelNumber == 2);
  m_Level3Background->SetVisible(levelNumber == 3);
  m_Level4Background->SetVisible(levelNumber == 4);
  m_Level5Background->SetVisible(levelNumber == 5);
}

void App::SetLevelSelectVisible(bool visible) {
  m_Level1Button->SetVisible(visible);
  m_Level2Button->SetVisible(visible);
  m_Level3Button->SetVisible(visible);
  m_Level4Button->SetVisible(visible);
  m_Level5Button->SetVisible(visible);
  m_BackButton->SetVisible(visible);
  m_LevelSelectBackground->SetVisible(visible);
}

void App::InitializeDoors() {
  // 火男的門
  if (!m_Fireboy_Door) {
    m_Fireboy_Door = std::make_shared<Door>(
        RESOURCE_DIR "/material/props/door/door-fireboy-closed.png",
        RESOURCE_DIR "/material/props/door/door-fireboy-half.png",
        RESOURCE_DIR "/material/props/door/door-fireboy-open.png", true, 10);
    m_Fireboy_Door->SetVisible(false);
    m_Root.AddChild(m_Fireboy_Door);
  }

  // 水女的門
  if (!m_Watergirl_Door) {
    m_Watergirl_Door = std::make_shared<Door>(
        RESOURCE_DIR "/material/props/door/door-watergirl-closed.png",
        RESOURCE_DIR "/material/props/door/door-watergirl-half.png",
        RESOURCE_DIR "/material/props/door/door-watergirl-open.png", false, 10);
    m_Watergirl_Door->SetVisible(false);
    m_Root.AddChild(m_Watergirl_Door);
  }
}

void App::InitializeGameUI() {
  // 繼續按鈕 (遊戲獲勝時)
  m_ContinueButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/continue-button.png",
      glm::vec2(0, -150));
  m_ContinueButton->SetVisible(false);
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

    HideAllLevelBackgrounds();
  });
  m_Root.AddChild(m_ContinueButton);

  // 暫停遊戲按鍵
  m_pauseButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/pause-button.png",
      glm::vec2(440, 320));
  m_pauseButton->SetVisible(false);
  m_pauseButton->SetOnClickCallback([this]() { SetPauseMenuVisible(true); });
  m_Root.AddChild(m_pauseButton);
}

void App::HideAllLevelBackgrounds() {
  m_Level1Background->SetVisible(false);
  m_Level2Background->SetVisible(false);
  m_Level3Background->SetVisible(false);
  m_Level4Background->SetVisible(false);
  m_Level5Background->SetVisible(false);
}

void App::InitializePauseMenu() {
  // 結束按鈕
  m_EndButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/end-button.png",
      glm::vec2(-130, -50));
  m_EndButton->SetVisible(false);
  m_EndButton->SetOnClickCallback([this]() {
    ResetGame();

    SetPauseMenuVisible(false);
    m_pauseButton->SetVisible(false);

    m_CurrentState = State::LOADING;
    // Reset loading animation
    m_LoadingFrameIndex = 0;
    m_LoadingAnimationTimer = 0.0f;

    m_LevelSelectBackground->SetVisible(true);
    HideAllLevelBackgrounds();
  });
  m_Root.AddChild(m_EndButton);

  // 重新開始按鈕
  m_RetryButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/retry-button.png",
      glm::vec2(130, -50));
  m_RetryButton->SetVisible(false);
  m_RetryButton->SetOnClickCallback([this]() {
    // 重置當前關卡
    ResetGameLevel();
    SetPauseMenuVisible(false);
  });
  m_Root.AddChild(m_RetryButton);

  // 繼續按鈕
  m_ResumeButton = std::make_shared<Option>(
      RESOURCE_DIR "/material/background/button/resume-button.png",
      glm::vec2(0, -150));
  m_ResumeButton->SetVisible(false);
  m_ResumeButton->SetOnClickCallback([this]() { SetPauseMenuVisible(false); });
  m_Root.AddChild(m_ResumeButton);
}

void App::SetPauseMenuVisible(bool visible) {
  m_GamePausedBackground->SetVisible(visible);
  m_EndButton->SetVisible(visible);
  m_RetryButton->SetVisible(visible);
  m_ResumeButton->SetVisible(visible);
}

void App::End() { LOG_TRACE("Game End"); }

void App::ActivateEndlessMode() {
    int width = m_GridSystem.GetWidth();
    int height = m_GridSystem.GetHeight();

    // 建立備份（僅第一次進入時儲存）
    if (m_OriginalTrapMap.empty()) {
        m_OriginalTrapMap.resize(height, std::vector<CellType>(width));

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                m_OriginalTrapMap[y][x] = m_GridSystem.GetCellType(x, y);
            }
        }
    }

    // 修改地圖陷阱 → FLOOR
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            CellType type = m_GridSystem.GetCellType(x, y);
            if (type == CellType::LAVA || type == CellType::WATER || type == CellType::POISON) {
                m_GridSystem.SetCellType(x, y, CellType::FLOOR);
            }
        }
    }

    std::cout << "[Endless Mode] Traps replaced with FLOOR.\n";
}


void App::RestoreTrapMap() {
    if (m_OriginalTrapMap.empty()) return;

    int width = m_GridSystem.GetWidth();
    int height = m_GridSystem.GetHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            m_GridSystem.SetCellType(x, y, m_OriginalTrapMap[y][x]);
        }
    }

    std::cout << "[Endless Mode] Traps restored.\n";
}
