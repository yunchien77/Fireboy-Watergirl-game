#include "App.hpp"

#include "Core/Context.hpp"

int main(int, char **) {
  auto context = Core::Context::GetInstance();
  App app;

  while (!context->GetExit()) {
    switch (app.GetCurrentState()) {
    case App::State::START:
      app.Start();
      break;

    case App::State::LEVEL_SELECT:
      app.LevelSelect();
      break;

    case App::State::GAME_PLAY:
      app.GamePlay();
      break;

    case App::State::GAME_OVER:
      app.GameOver();
      break;

    case App::State::GAME_WIN:
      app.GameWin();
      break;

    case App::State::END:
      app.End();
      context->SetExit(true);
      break;
    }
    context->Update();
  }
  return 0;
}
