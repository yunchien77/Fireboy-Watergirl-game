#include "App.hpp"
#include "Core/Context.hpp"
#include <SDL.h>

int main(int, char **) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    return 1;
  }

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

    case App::State::GAME_PAUSE:
      app.GamePause();
      break;

    case App::State::GAME_WIN:
      app.GameWin();
      break;

    case App::State::LOADING:
      app.Loading(0.016f); // 16毫秒（~60FPS）
      break;

    case App::State::END:
      app.End();
      context->SetExit(true);
      break;
    }

    context->Update();
  }

  SDL_Quit();
  return 0;
}
