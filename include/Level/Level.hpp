#pragma once

#include "Interface/ITriggerable.hpp"
#include "Object/GridSystem.hpp"
#include "Util/Renderer.hpp"
#include <Mechanism/Box.hpp>
#include <Mechanism/Button.hpp>
#include <Mechanism/Door.hpp>
#include <Mechanism/Fan.hpp>
#include <Mechanism/Gate.hpp>
#include <Mechanism/Gem.hpp>
#include <Mechanism/Lever.hpp>
#include <Mechanism/LiquidTrap.hpp>
#include <Mechanism/Platform.hpp>
#include <memory>
#include <vector>

class IGem;
class Button;
class ITrap;
class Gate;
class Character;
class Door;
class Lever;
class Platform;
class Box;
class Fan;

class Level {
protected:
  GridSystem &m_GridSystem;
  Util::Renderer &m_Root;
  std::vector<std::shared_ptr<IGem>> &m_Gems;
  std::vector<std::shared_ptr<Gate>> &m_Triggers;
  std::vector<std::shared_ptr<Button>> &m_Buttons;
  std::vector<std::shared_ptr<ITrap>> &m_Traps;
  std::vector<std::shared_ptr<Lever>> &m_Levers;
  std::vector<std::shared_ptr<Platform>> &m_Platforms;
  std::vector<std::shared_ptr<Box>> &m_Boxes;
  std::vector<std::shared_ptr<Fan>> &m_Fans;
  std::shared_ptr<Character> &m_Fireboy;
  std::shared_ptr<Character> &m_Watergirl;
  std::shared_ptr<Door> &m_Fireboy_Door;
  std::shared_ptr<Door> &m_Watergirl_Door;

public:
  Level(GridSystem &gridSystem, Util::Renderer &root,
        std::vector<std::shared_ptr<IGem>> &gems,
        std::vector<std::shared_ptr<Gate>> &gates,
        std::vector<std::shared_ptr<Button>> &buttons,
        std::vector<std::shared_ptr<ITrap>> &traps,
        std::vector<std::shared_ptr<Lever>> &levers,
        std::vector<std::shared_ptr<Platform>> &platforms,
        std::vector<std::shared_ptr<Box>> &boxes,
        std::vector<std::shared_ptr<Fan>> &fans,
        std::shared_ptr<Character> &fireboy,
        std::shared_ptr<Character> &watergirl,
        std::shared_ptr<Door> &fireboyDoor,
        std::shared_ptr<Door> &watergirlDoor);

  virtual ~Level() = default;

  virtual bool Initialize() = 0;

  virtual bool LoadGrid(int levelNumber);

  virtual void Cleanup();

protected:
  void InitCharacter(std::shared_ptr<Character> &character, const glm::ivec2 &gridPos);
  void InitDoor(std::shared_ptr<Door> &door, const glm::ivec2 &gridPos);
  void InitGems(GemType type, const std::vector<glm::ivec2> &coords);
  void InitLiquidTraps(CellType type, SizeType size, const std::vector<glm::ivec2> &coords);
  void InitBoxes(const std::vector<glm::ivec2> &coords);
  void InitFans(const std::vector<std::tuple<int, int, float, FanColor>> &fanInfos);
  std::shared_ptr<Gate> InitGate(GateColor color, const glm::ivec2 &cellPos);
  std::shared_ptr<Button> InitButton(ButtonColor color, const glm::ivec2 &cellPos, ITriggerable *target);
  std::shared_ptr<Lever> InitLever(LeverColor color, const glm::ivec2 &cellPos, ITriggerable *target);
  std::shared_ptr<Platform> InitPlatform(PlatformColor color, const glm::ivec2 &cellPos, const glm::vec2 &offset);
};