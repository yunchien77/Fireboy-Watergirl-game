#pragma once
#include "Character/Character.hpp"
#include "Mechanism/Box.hpp"
#include "Mechanism/Button.hpp"
#include "Mechanism/Door.hpp"
#include "Mechanism/Fan.hpp"
#include "Mechanism/Gate.hpp"
#include "Mechanism/Gem.hpp"
#include "Mechanism/Lever.hpp"
#include "Mechanism/LiquidTrap.hpp"
#include "Mechanism/Platform.hpp"
#include "Object/GridSystem.hpp"
#include <memory>
#include <vector>

class GameMechanicsManager {
public:
  GameMechanicsManager(GridSystem &gridSystem);

  // 設置遊戲物件
  void SetCharacters(std::shared_ptr<Character> fireboy,
                     std::shared_ptr<Character> watergirl);
  void SetDoors(std::shared_ptr<Door> fireboyDoor,
                std::shared_ptr<Door> watergirlDoor);
  void SetGameObjects(std::vector<std::shared_ptr<Gem>> &gems,
                      std::vector<std::shared_ptr<Button>> &buttons,
                      std::vector<std::shared_ptr<Gate>> &gates,
                      std::vector<std::shared_ptr<Lever>> &levers,
                      std::vector<std::shared_ptr<Platform>> &platforms,
                      std::vector<std::shared_ptr<Box>> &boxes,
                      std::vector<std::shared_ptr<Fan>> &fans,
                      std::vector<std::shared_ptr<LiquidTrap>> &traps);

  // 遊戲邏輯更新
  void UpdateInteractions(float deltaTime);
  void UpdateFans(float deltaTime);
  void UpdatePlatforms(float deltaTime);
  void UpdateBoxes();
  void UpdateButtons();
  void UpdateLevers();
  void UpdateGates(float deltaTime);

  // 碰撞檢測
  void HandleGemCollection();
  void HandleLiquidTraps();
  void HandleGateBlocking();

  // 勝利條件檢查
  bool CheckWinCondition();
  void CheckCharacterDoorInteraction();
  bool GetGemCollectionStatus();

  // 重置關卡
  void ResetLevel();

  // 檢查角色死亡
  bool IsAnyCharacterDead();

private:
  GridSystem &m_GridSystem;

  // 角色
  std::shared_ptr<Character> m_Fireboy;
  std::shared_ptr<Character> m_Watergirl;

  // 門
  std::shared_ptr<Door> m_FireboyDoor;
  std::shared_ptr<Door> m_WatergirlDoor;

  // 遊戲物件 (引用)
  std::vector<std::shared_ptr<Gem>> *m_Gems;
  std::vector<std::shared_ptr<Button>> *m_Buttons;
  std::vector<std::shared_ptr<Gate>> *m_Gates;
  std::vector<std::shared_ptr<Lever>> *m_Levers;
  std::vector<std::shared_ptr<Platform>> *m_Platforms;
  std::vector<std::shared_ptr<Box>> *m_Boxes;
  std::vector<std::shared_ptr<Fan>> *m_Fans;
  std::vector<std::shared_ptr<LiquidTrap>> *m_Traps;

  void UpdateCharacterPlatformMovement();
  void PrepareCharactersForUpdate();
};