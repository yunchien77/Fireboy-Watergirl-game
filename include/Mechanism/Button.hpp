#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Interface/ITriggerable.hpp"
#include "Mechanism/MechanismBase.hpp"
#include "Object/Color.hpp"
#include <vector>

using ButtonColor = Color;

class Character;

class Button : public MechanismBase {
public:
  Button(ButtonColor color, const glm::vec2 &pos);

  using MechanismBase::Update;
  void Update(Character *fb, Character *wg);
  void LinkTrigger(ITriggerable *target);
  ButtonColor GetColor() const;

private:
  std::string GetImagePath(ButtonColor color);

  ButtonColor m_Color;
  bool m_IsPressedFireboy = false;
  bool m_IsPressedWatergirl = false;
  std::vector<ITriggerable *> m_Triggers;
};

#endif // BUTTON_HPP
