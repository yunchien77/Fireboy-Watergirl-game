#ifndef GATE_HPP
#define GATE_HPP

#include "Interface/ITriggerable.hpp"
#include "Mechanism/MechanismBase.hpp"
#include "Object/Color.hpp"
#include <glm/glm.hpp>
#include <string>

using GateColor = Color;

class Gate : public MechanismBase, public ITriggerable {
public:
  Gate(GateColor color, const glm::vec2 &pos);

  void OnTriggered() override;
  void OnReleased() override;
  void Respawn() override;

  void SetOpen(bool open);
  void SetScale(const glm::vec2 &scale);
  void SetInitialState(const glm::vec2 &pos, bool isOpen);

  void UpdateAnimation(float deltaTime);

  GateColor GetColor() const;
  bool IsOpen() const;
  bool IsBlocking() const;

private:
  std::string GetImagePath(GateColor color);

  GateColor m_Color;
  bool m_IsOpen = false;
  bool m_IsAnimating = false;
  bool m_ShouldOpen = false;
  bool m_InitialIsOpen = false;
  int m_ActiveTriggerCount = 0;
};

#endif // GATE_HPP