#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <SDL.h>
#include <memory>

void App::GameWin() {
  LOG_TRACE("Game Win");

  bool gemsCollected = GetGemCollectionStatus();

  // 初始化和顯示 UI 元素
  InitializeWinUI(gemsCollected);

  // 處理用戶輸入
  HandleWinScreenInput();

  // 檢查退出條件
  HandleExitConditions();

  m_Root.Update();
}

void App::InitializeWinUI(bool gemsCollected) {
  // 顯示背景
  ShowWinBackground();

  // 顯示角色指示器
  ShowCharacterIndicator();

  // 顯示寶石收集指示器
  ShowGemCollectionIndicator(gemsCollected);

  // 顯示關卡結果
  ShowLevelResult(gemsCollected);

  // 更新關卡按鈕
  UpdateCurrentLevelButton(gemsCollected);
}

void App::ShowWinBackground() {
  if (!m_GameWinBackground) {
    m_GameWinBackground = std::make_shared<BackgroundImage>(
        RESOURCE_DIR "/material/background/game-win.png");
    m_Root.AddChild(m_GameWinBackground);
  }
  m_GameWinBackground->SetVisible(true);
  m_ContinueButton->SetVisible(true);
}

void App::ShowCharacterIndicator() {
  if (!m_CharacterIndicator) {
    m_CharacterIndicator = std::make_shared<BackgroundImage>(
        RESOURCE_DIR "/material/background/icon/result-pass.png", 40);
    m_CharacterIndicator->SetPosition(glm::vec2(-100, 85));
    m_Root.AddChild(m_CharacterIndicator);
  }
  m_CharacterIndicator->SetVisible(true);
}

void App::ShowGemCollectionIndicator(bool gemsCollected) {
  const std::string imagePath =
      gemsCollected ? RESOURCE_DIR "/material/background/icon/result-pass.png"
                    : RESOURCE_DIR "/material/background/icon/result-fail.png";

  if (!m_GemCollectedIndicator) {
    m_GemCollectedIndicator = std::make_shared<BackgroundImage>(imagePath, 40);
    m_GemCollectedIndicator->SetPosition(glm::vec2(-100, -10));
    m_Root.AddChild(m_GemCollectedIndicator);
  } else {
    m_GemCollectedIndicator->SetImage(imagePath);
  }
  m_GemCollectedIndicator->SetVisible(true);
}

void App::ShowLevelResult(bool gemsCollected) {
  const std::string imagePath =
      gemsCollected ? RESOURCE_DIR "/material/background/button/win2-level.png"
                    : RESOURCE_DIR "/material/background/button/win1-level.png";

  if (!m_LevelResult) {
    m_LevelResult = std::make_shared<BackgroundImage>(imagePath, 40);
    m_LevelResult->SetPosition(glm::vec2(140, 50));
    m_Root.AddChild(m_LevelResult);
  } else {
    m_LevelResult->SetImage(imagePath);
  }
  m_LevelResult->SetVisible(true);
}

void App::UpdateCurrentLevelButton(bool gemsCollected) {
  std::shared_ptr<Option> currentLevelButton = GetCurrentLevelButton();

  if (currentLevelButton) {
    const std::string buttonImagePath =
        gemsCollected
            ? RESOURCE_DIR "/material/background/button/win2-level.png"
            : RESOURCE_DIR "/material/background/button/win1-level.png";

    currentLevelButton->SetImage(buttonImagePath);
  }
}

std::shared_ptr<Option> App::GetCurrentLevelButton() {
  switch (m_CurrentLevel) {
  case 1:
    return m_Level1Button;
  case 2:
    return m_Level2Button;
  case 3:
    return m_Level3Button;
  case 4:
    return m_Level4Button;
  case 5:
    return m_Level5Button;
  default:
    return nullptr;
  }
}

void App::HandleWinScreenInput() {
  int mouseX, mouseY;
  Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
  glm::vec2 gameCoords = ConvertToGameCoordinates(mouseX, mouseY);

  if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
    HandleContinueButtonClick(gameCoords);
  }
}

void App::HandleContinueButtonClick(const glm::vec2 &gameCoords) {
  if (m_ContinueButton->HandleClick(gameCoords.x, gameCoords.y)) {
    UnlockNextLevel();
  }
}

void App::UnlockNextLevel() {
  struct LevelConfig {
    int level;
    std::shared_ptr<Option> button;
    std::shared_ptr<BackgroundImage> background;
  };

  std::vector<LevelConfig> levels = {
      {1, m_Level2Button, nullptr},
      {2, m_Level3Button, m_Level2Background},
      {3, m_Level4Button, m_Level3Background},
      {4, m_Level5Button, m_Level4Background},
  };

  for (const auto &config : levels) {
    if (m_CurrentLevel == config.level && unlockedLevel == config.level) {
      // 隱藏當前關卡背景（如果存在）
      if (config.background) {
        config.background->SetVisible(false);
      }

      // 解鎖下一關
      unlockedLevel = config.level + 1;
      config.button->SetImage(RESOURCE_DIR
                              "/material/background/button/current-level.png");
      config.button->SetInteractable(true);
      break;
    }
  }
}

void App::HandleExitConditions() {
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }
}