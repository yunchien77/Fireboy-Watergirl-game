#ifndef DOOR_HPP
#define DOOR_HPP

#include "Mechanism/MechanismBase.hpp"
#include <map>
#include <string>

namespace Util {
  class Image;
}

enum class DoorState { CLOSED, HALF_OPEN, FULLY_OPEN };

class Door : public MechanismBase {
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

  void OpenDoor();
  void CloseDoor();
  void UpdateAnimation();

  void Update() override;
  void Respawn() override;

private:
  void UpdateDoorImage();

  std::map<DoorState, std::string> m_DoorImages;
  bool m_IsFireDoor;
  DoorState m_CurrentState;
  bool m_IsCharacterAtDoor;
  int m_AnimationCounter;
};

#endif // DOOR_HPP