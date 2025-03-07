#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <memory>

App::App() {
  LOG_DEBUG("Game object initialize");
  // 初始化遊戲資源
  m_TitleBackground = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/cover.png");
  m_TitleBackground->SetVisible(false);
  m_Root.AddChild(m_TitleBackground);

  m_LevelBackground = std::make_shared<BackgroundImage>(
      RESOURCE_DIR "/material/background/level-page-show.png");
  m_LevelBackground->SetVisible(false);
  m_Root.AddChild(m_LevelBackground);
}

void App::Start() {
  LOG_TRACE("Game Start");
  m_TitleBackground->SetVisible(true);

  // 檢查是否點擊開始按鈕
  if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
    m_CurrentState = State::LEVEL_SELECT;
    LOG_TRACE("Entering Level Select");
  }

  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }

  m_Root.Update();
}

void App::LevelSelect() {
  LOG_TRACE("Level Select");

  m_TitleBackground->SetVisible(false);
  m_LevelBackground->SetVisible(true);
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }
  m_Root.Update();
}

void App::GamePlay() {
  LOG_TRACE("Game Play");
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }
  m_Root.Update();
}

void App::GameOver() {
  LOG_TRACE("Game Over");
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }
  m_Root.Update();
}

void App::GameWin() {
  LOG_TRACE("Game Win");
  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }
  m_Root.Update();
}

void App::End() { // NOLINT(this method will mutate members in the future)
  LOG_TRACE("End");
}
