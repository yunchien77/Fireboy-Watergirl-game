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

  // Create and initialize the game win background if it doesn't exist
  if (!m_GameWinBackground) {
    m_GameWinBackground = std::make_shared<BackgroundImage>(
        RESOURCE_DIR "/material/background/game-win.png");
    m_Root.AddChild(m_GameWinBackground);
  }
  m_GameWinBackground->SetVisible(true);
  m_ContinueButton->SetVisible(true);

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

  // 處理遊戲結束條件
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }

  m_Root.Update();
}