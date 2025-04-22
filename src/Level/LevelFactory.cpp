// LevelFactory.cpp
#include "Level/LevelFactory.hpp"
#include "Level/Level1.hpp"
#include "Level/Level2.hpp"
#include "Level/Level3.hpp"

std::unique_ptr<Level> LevelFactory::CreateLevel(
    int levelNumber, GridSystem &gridSystem, Util::Renderer &root,
    std::vector<std::shared_ptr<IGem>> &gems,
    std::vector<std::shared_ptr<Gate>> &triggers,
    std::vector<std::shared_ptr<Button>> &buttons,
    std::vector<std::shared_ptr<ITrap>> &traps,
    std::vector<std::shared_ptr<Lever>> &levers,
    std::vector<std::shared_ptr<Platform>> &platforms,
    std::shared_ptr<Character> &fireboy, std::shared_ptr<Character> &watergirl,
    std::shared_ptr<Door> &fireboyDoor, std::shared_ptr<Door> &watergirlDoor) {

  switch (levelNumber) {
    case 1:
      return std::make_unique<Level1>(gridSystem, root, gems, triggers, buttons,
                                      traps, levers, platforms, fireboy, watergirl,
                                      fireboyDoor, watergirlDoor);
    case 2:
      return std::make_unique<Level2>(gridSystem, root, gems, triggers, buttons,
                                      traps, levers, platforms, fireboy, watergirl,
                                      fireboyDoor, watergirlDoor);
    case 3:
      return std::make_unique<Level3>(gridSystem, root, gems, triggers, buttons,
                                      traps, levers, platforms, fireboy, watergirl,
                                      fireboyDoor, watergirlDoor);
  default:
    return nullptr;
  }
}