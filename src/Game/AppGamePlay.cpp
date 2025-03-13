#include "App.hpp"
#include "Character/Fireboy.hpp"
#include "Character/Watergirl.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "config.hpp"
#include <memory>


const int LEVEL_MIN_X = -430;
const int LEVEL_MAX_X = 425;
const int LEVEL_MIN_Y = -288;
const int LEVEL_MAX_Y = 315;

void RestrictPlayerPosition(Character &player) {
  glm::vec2 pos = player.GetPosition();

  if (pos.x < LEVEL_MIN_X)
    pos.x = LEVEL_MIN_X;
  if (pos.x > LEVEL_MAX_X)
    pos.x = LEVEL_MAX_X;
  if (pos.y < LEVEL_MIN_Y)
    pos.y = LEVEL_MIN_Y;
  if (pos.y > LEVEL_MAX_Y)
    pos.y = LEVEL_MAX_Y;

  player.SetPosition(pos);
}

void App::GamePlay() {
  LOG_TRACE("Game Play");

  if (!m_Fireboy) {
    m_Fireboy = std::make_shared<Fireboy>();
    m_Fireboy->SetPosition({-420, -288});
    m_Root.AddChild(m_Fireboy);
  }

  if (!m_Watergirl) {
    m_Watergirl = std::make_shared<Watergirl>();
    m_Watergirl->SetPosition({-420, -193});
    m_Root.AddChild(m_Watergirl);
  }

  int fireboyMoveX = 0;
  bool fireboyUpKeyPressed = false;
  if (Util::Input::IsKeyPressed(Util::Keycode::LEFT))
    fireboyMoveX = -5;
  if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT))
    fireboyMoveX = 5;
  if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
    fireboyUpKeyPressed = true;
  }

  m_Fireboy->Move(fireboyMoveX, fireboyUpKeyPressed);
  m_Fireboy->UpdateJump();
  RestrictPlayerPosition(*m_Fireboy);

  int watergirlMoveX = 0;
  bool watergirlUpKeyPressed = false;
  if (Util::Input::IsKeyPressed(Util::Keycode::A))
    watergirlMoveX = -5;
  if (Util::Input::IsKeyPressed(Util::Keycode::D))
    watergirlMoveX = 5;
  if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
    watergirlUpKeyPressed = true;
  }

  m_Watergirl->Move(watergirlMoveX, watergirlUpKeyPressed);
  m_Watergirl->UpdateJump();
  RestrictPlayerPosition(*m_Watergirl);

  if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
    m_CurrentState = State::END;
  }

  m_Root.Update();
}
