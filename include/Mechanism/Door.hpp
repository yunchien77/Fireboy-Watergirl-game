#ifndef DOOR_HPP
#define DOOR_HPP

#include "Interface/ICollidable.hpp"
#include "Interface/IInteractable.hpp"
#include "Interface/ITriggerable.hpp"
#include "Util/GameObject.hpp"
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <string>

namespace Util {
class Image;
}

enum class DoorState { CLOSED, HALF_OPEN, FULLY_OPEN };

class Door : public Util::GameObject,
             public ICollidable,
             public IInteractable,
             public ITriggerable {
public:
  Door(const std::string &closedImagePath, const std::string &halfOpenImagePath,
       const std::string &fullyOpenImagePath, bool isFireDoor,
       float zIndex = 20.0f);

  Door(const Door &) = delete;
  Door(Door &&) = delete;
  Door &operator=(const Door &) = delete;
  Door &operator=(Door &&) = delete;
  virtual ~Door() = default;

  // ICollidable interface
  const SDL_Rect &getRect() const override;

  // IInteractable interface
  void OnCharacterEnter(Character *character) override;
  // void OnCharacterExit(Character *character) override;

  // ITriggerable interface
  void OnTriggered() override;
  void OnReleased() override;

  // Door-specific methods
  [[nodiscard]] bool IsOpen() const;
  [[nodiscard]] bool IsFireDoor() const;

  void SetOpen(bool isOpen);
  bool IsCharacterAtDoor() const;

  void SetPosition(const glm::vec2 &position);
  glm::vec2 GetPosition() const;

  void Update(float deltaTime);

  void SetCharacterAtDoor(bool isAtDoor);

  void UpdateAnimation();

private:
  void OpenDoor();
  void CloseDoor();
  void UpdateDoorImage();

  std::map<DoorState, std::string> m_DoorImages;
  bool m_IsFireDoor;
  DoorState m_CurrentState;
  bool m_IsCharacterAtDoor;
  int m_AnimationCounter;
  mutable SDL_Rect m_Rect{};
};

#endif // DOOR_HPP