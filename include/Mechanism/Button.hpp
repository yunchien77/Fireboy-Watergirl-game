#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Interface/ICollidable.hpp"
#include "Interface/ITriggerable.hpp"
#include "Object/Color.hpp"
#include "Util/GameObject.hpp"
#include <vector>

using ButtonColor = Color;

class Character;

class Button : public Util::GameObject, public ICollidable {
public:
  Button(ButtonColor color, const glm::vec2 &pos);

  // State update and management
  void Update(Character *fb, Character *wg);
  void Respawn();

  // Trigger linking
  void LinkTrigger(ITriggerable *target);

  // Position management
  void SetPosition(const glm::vec2 &position);
  void SetInitialState(const glm::vec2 &pos);

  // Properties
  ButtonColor GetColor() const;

  // ICollidable implementation
  const SDL_Rect &getRect() const override;

private:
  // Button properties
  ButtonColor m_Color;
  mutable SDL_Rect m_Rect;

  // Button state
  bool m_IsPressedFireboy = false;
  bool m_IsPressedWatergirl = false;

  // Linked objects
  std::vector<ITriggerable *> m_Triggers;

  // Initial state for respawn
  glm::vec2 m_InitialPosition;

  // Helper methods
  std::string GetImagePath(ButtonColor color);
};

#endif // BUTTON_HPP