#pragma once
#include "Level/Level.hpp"
#include <memory>

class LevelFactory {
public:
  static std::unique_ptr<Level>
  CreateLevel(int levelNumber, GridSystem &gridSystem, Util::Renderer &root,
              std::vector<std::shared_ptr<IGem>> &gems,
              std::vector<std::shared_ptr<Gate>> &triggers,
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
};