#ifndef GATE_HPP
#define GATE_HPP

#include "Interface/ICollidable.hpp"
#include "Interface/ITriggerable.hpp"
#include "Object/Color.hpp"
#include "Util/GameObject.hpp"
#include <glm/glm.hpp>
#include <string>


using GateColor = Color;

class Gate : public Util::GameObject, public ITriggerable, public ICollidable {
public:
  Gate(GateColor color, const glm::vec2 &pos);

  // ITriggerable interface
  void OnTriggered() override;
  void OnReleased() override;

  // ICollidable interface
  const SDL_Rect &getRect() const override;

  // Gate specific methods
  void SetOpen(bool open);
  void SetPosition(const glm::vec2 &position);
  bool IsOpen() const;
  bool IsBlocking() const;
  GateColor GetColor() const;
  void UpdateAnimation(float deltaTime);
  void SetScale(const glm::vec2 &scale);
  void SetInitialState(const glm::vec2 &pos, bool isOpen);
  void Respawn();

private:
  GateColor m_Color;
  bool m_IsOpen = false;
  glm::vec2 m_InitialPosition;
  mutable SDL_Rect m_Rect;
  bool m_IsAnimating = false;
  bool m_ShouldOpen = false;
  bool m_InitialIsOpen = false;
  int m_ActiveTriggerCount = 0;

  std::string GetImagePath(GateColor color);
};

#endif // GATE_HPP