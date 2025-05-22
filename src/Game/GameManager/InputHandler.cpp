#include "Game/InputHandler.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

CharacterInput InputHandler::GetFireboyInput() {
  CharacterInput input;

  if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
    input.moveX = -5;
  }
  if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
    input.moveX = 5;
  }
  if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
    input.upKeyPressed = true;
  }

  return input;
}

CharacterInput InputHandler::GetWatergirlInput() {
  CharacterInput input;

  if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
    input.moveX = -5;
  }
  if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
    input.moveX = 5;
  }
  if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
    input.upKeyPressed = true;
  }

  return input;
}

void InputHandler::ProcessCharacterMovement(
    std::shared_ptr<Character> fireboy, std::shared_ptr<Character> watergirl,
    GridSystem &gridSystem) {
  if (!fireboy || !watergirl)
    return;

  // 獲取輸入
  CharacterInput fireboyInput = GetFireboyInput();
  CharacterInput watergirlInput = GetWatergirlInput();

  // 應用移動
  ApplyCharacterMovement(fireboy, fireboyInput, gridSystem, true);
  ApplyCharacterMovement(watergirl, watergirlInput, gridSystem, false);
}

void InputHandler::ApplyCharacterMovement(std::shared_ptr<Character> character,
                                          const CharacterInput &input,
                                          GridSystem &gridSystem,
                                          bool isFireboy) {
  if (!character)
    return;

  // 執行移動、跳躍、重力
  character->Move(input.moveX, input.upKeyPressed, gridSystem, isFireboy);
  character->UpdateJump(gridSystem);
  character->ApplyGravity(gridSystem);
}

bool InputHandler::IsExitRequested() {
  return Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit();
}