#ifndef LEVER_HPP
#define LEVER_HPP

#include "Character/Character.hpp"
#include "Interface/ITriggerable.hpp"
#include "Mechanism/MechanismBase.hpp"
#include "Object/Color.hpp"
#include <vector>

using LeverColor = Color;

class Lever : public MechanismBase {
public:
  Lever(LeverColor color, const glm::vec2 &pos);

  using MechanismBase::Update;
  void Update(Character *fb, Character *wg);

  void LinkTrigger(ITriggerable *target);
  const SDL_Rect &getRect() const override;

  void SetInitialState(const glm::vec2 &pos, bool isOn);
  void Respawn() override;

private:
  void Toggle();
  std::string GetImagePath(LeverColor color, bool isOn) const;

  bool m_IsOn = false;
  bool m_InitialIsOn = false;
  mutable SDL_Rect m_Rect{};
  std::vector<ITriggerable *> m_Triggers;
};

#endif // LEVER_HPP
