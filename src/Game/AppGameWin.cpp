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

  // Create and initialize the game win background if it doesn't exist
  if (!m_GameWinBackground) {
    m_GameWinBackground = std::make_shared<BackgroundImage>(
        RESOURCE_DIR "/material/background/game-win.png");
    m_Root.AddChild(m_GameWinBackground);
  }
  m_GameWinBackground->SetVisible(true);
  m_ContinueButton->SetVisible(true);

  if (!m_CharacterIndicator) {
    m_CharacterIndicator = std::make_shared<BackgroundImage>(
        RESOURCE_DIR "/material/background/icon/result-pass.png", 40);
    m_CharacterIndicator->SetPosition(glm::vec2(-100, 110));
    m_Root.AddChild(m_CharacterIndicator);
  }
  m_CharacterIndicator->SetVisible(true);

  if (!m_GemCollectedIndicator) {
    m_GemCollectedIndicator = std::make_shared<BackgroundImage>(
        gemsCollected ? RESOURCE_DIR "/material/background/icon/result-pass.png"
                      : RESOURCE_DIR
            "/material/background/icon/result-fail.png",
        40);
    m_GemCollectedIndicator->SetPosition(glm::vec2(-100, 50));
    m_Root.AddChild(m_GemCollectedIndicator);
  } else {
    // Update existing image
    m_GemCollectedIndicator->SetImage(
        gemsCollected ? RESOURCE_DIR "/material/background/icon/result-pass.png"
                      : RESOURCE_DIR
            "/material/background/icon/result-fail.png");
  }
  m_GemCollectedIndicator->SetVisible(true);

  // Do the same for the level result
  if (!m_LevelResult) {
    m_LevelResult = std::make_shared<BackgroundImage>(
        gemsCollected
            ? RESOURCE_DIR "/material/background/button/win2-level.png"
            : RESOURCE_DIR "/material/background/button/win1-level.png",
        40);
    m_LevelResult->SetPosition(glm::vec2(140, 50));
    m_Root.AddChild(m_LevelResult);
  } else {
    // Update existing image
    m_LevelResult->SetImage(gemsCollected ? RESOURCE_DIR
                                "/material/background/button/win2-level.png"
                                          : RESOURCE_DIR
                                "/material/background/button/win1-level.png");
  }
  m_LevelResult->SetVisible(true);

  // Update the current level button based on gem collection status
  std::shared_ptr<Option> currentLevelButton = nullptr;

  switch (m_CurrentLevel) {
  case 1:
    currentLevelButton = m_Level1Button;
    break;
  case 2:
    currentLevelButton = m_Level2Button;
    break;
  case 3:
    currentLevelButton = m_Level3Button;
    break;
  case 4:
    currentLevelButton = m_Level4Button;
    break;
  case 5:
    currentLevelButton = m_Level5Button;
    break;
  default:
    break;
  }

  // Update the button image if we have a valid button
  if (currentLevelButton) {
    std::string buttonImagePath =
        gemsCollected
            ? RESOURCE_DIR "/material/background/button/win2-level.png"
            : RESOURCE_DIR "/material/background/button/win1-level.png";

    currentLevelButton->SetImage(buttonImagePath);
  }

  int mouseX, mouseY;
  Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
  glm::vec2 gameCoords = ConvertToGameCoordinates(mouseX, mouseY);

  if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
    if (m_ContinueButton->HandleClick(gameCoords.x, gameCoords.y)) {
      if (m_CurrentLevel == 1) {
        unlockedLevel = 2;
        m_Level2Button->SetImage(
            RESOURCE_DIR "/material/background/button/current-level.png");
        m_Level2Button->SetInteractable(true);
      }
      if (m_CurrentLevel == 2) {
        m_Level2Background->SetVisible(false);
        unlockedLevel = 3;
        m_Level3Button->SetImage(
            RESOURCE_DIR "/material/background/button/current-level.png");
        m_Level3Button->SetInteractable(true);
      }
    }
  }

  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }

  m_Root.Update();
}