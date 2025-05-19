#ifndef DOOR_HPP
#define DOOR_HPP

#include "Util/GameObject.hpp"
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <string>

namespace Util {
  class Image;
}

enum class DoorState { CLOSED, HALF_OPEN, FULLY_OPEN };

class Door : public Util::GameObject {
public:
  Door(const std::string &closedImagePath,
       const std::string &halfOpenImagePath,
       const std::string &fullyOpenImagePath,
       bool isFireDoor,
       float zIndex = 20.0f);

  Door(const Door &) = delete;
  Door(Door &&) = delete;
  Door &operator=(const Door &) = delete;
  Door &operator=(Door &&) = delete;

  [[nodiscard]] bool IsOpen() const;
  [[nodiscard]] bool IsFireDoor() const;

  void SetOpen(bool isOpen);
  void SetCharacterAtDoor(bool isAtDoor);
  bool IsCharacterAtDoor() const;

  void SetPosition(const glm::vec2 &position);
  glm::vec2 GetPosition() const;

  void OpenDoor();
  void CloseDoor();
  void UpdateAnimation();

private:
  void UpdateDoorImage();

  std::map<DoorState, std::string> m_DoorImages;
  bool m_IsFireDoor;
  DoorState m_CurrentState;
  bool m_IsCharacterAtDoor;
  int m_AnimationCounter;
};

#endif // DOOR_HPP