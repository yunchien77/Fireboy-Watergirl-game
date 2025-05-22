#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <SDL.h>
#include <memory>

void App::GamePause() {
  LOG_TRACE("Game Paused");

  int mouseX, mouseY;
  Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
  glm::vec2 gameCoords = ConvertToGameCoordinates(mouseX, mouseY);

  if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
    if (m_EndButton->HandleClick(gameCoords.x, gameCoords.y)) {
    }
    if (m_ResumeButton->HandleClick(gameCoords.x, gameCoords.y)) {
      m_CurrentState = State::GAME_PLAY;
    }
    if (m_RetryButton->HandleClick(gameCoords.x, gameCoords.y)) {
      m_CurrentState = State::GAME_PLAY;
    }
  }

  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }
  m_Root.Update();
}