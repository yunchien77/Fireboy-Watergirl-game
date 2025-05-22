#pragma once
#include "Character/Character.hpp"
#include "Object/GridSystem.hpp"
#include <memory>

struct CharacterInput {
  int moveX = 0;
  bool upKeyPressed = false;
};

class InputHandler {
public:
  InputHandler() = default;

  // 獲取角色輸入
  CharacterInput GetFireboyInput();
  CharacterInput GetWatergirlInput();

  // 處理角色移動
  void ProcessCharacterMovement(std::shared_ptr<Character> fireboy,
                                std::shared_ptr<Character> watergirl,
                                GridSystem &gridSystem);

  // 檢查系統輸入
  bool IsExitRequested();

private:
  // 輔助方法
  void ApplyCharacterMovement(std::shared_ptr<Character> character,
                              const CharacterInput &input,
                              GridSystem &gridSystem, bool isFireboy);
};