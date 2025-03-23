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

  // 處理遊戲結束條件
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }

  m_Root.Update();
}