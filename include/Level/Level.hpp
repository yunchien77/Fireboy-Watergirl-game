#pragma once

#include "Object/GridSystem.hpp"
#include "Util/Renderer.hpp"
#include <memory>
#include <vector>

class IGem;
class Button;
class ITrap;
class Gate;
class Character;
class Door;

class Level {
protected:
  GridSystem &m_GridSystem;
  Util::Renderer &m_Root;
  std::vector<std::shared_ptr<IGem>> &m_Gems;
  std::vector<std::shared_ptr<Gate>> &m_Triggers;
  std::vector<std::shared_ptr<Button>> &m_Buttons;
  std::vector<std::shared_ptr<ITrap>> &m_Traps;
  std::shared_ptr<Character> &m_Fireboy;
  std::shared_ptr<Character> &m_Watergirl;
  std::shared_ptr<Door> &m_Fireboy_Door;
  std::shared_ptr<Door> &m_Watergirl_Door;

public:
  Level(GridSystem &gridSystem, Util::Renderer &root,
        std::vector<std::shared_ptr<IGem>> &gems,
        std::vector<std::shared_ptr<Gate>> &triggers,
        std::vector<std::shared_ptr<Button>> &buttons,
        std::vector<std::shared_ptr<ITrap>> &traps,
        std::shared_ptr<Character> &fireboy,
        std::shared_ptr<Character> &watergirl,
        std::shared_ptr<Door> &fireboyDoor,
        std::shared_ptr<Door> &watergirlDoor);

  virtual ~Level() = default;

  virtual bool Initialize() = 0;

  virtual void Cleanup();

  virtual bool LoadGrid(int levelNumber);
};