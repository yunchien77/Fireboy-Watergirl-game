#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <SDL.h>
#include <memory>

void App::Start() {
  LOG_TRACE("Start");
  m_TitleBackground->SetVisible(true);

  if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
    m_CurrentState = State::LEVEL_SELECT;
  }

  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }

  m_Root.Update();
}