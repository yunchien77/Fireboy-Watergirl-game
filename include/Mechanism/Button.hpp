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
  Button(Color color, const glm::vec2 &pos);

  // State update and management
  using MechanismBase::Update;
  void Update(Character *fb, Character *wg);
  void Respawn() override;

  // Trigger linking
  void LinkTrigger(ITriggerable *target);

  // Position management
  void SetPosition(const glm::vec2 &position) override;

private:
  bool m_IsPressedFireboy = false;
  bool m_IsPressedWatergirl = false;
  std::vector<ITriggerable *> m_Triggers;
  std::string GetImagePath(ButtonColor color);
};

#endif // BUTTON_HPP
