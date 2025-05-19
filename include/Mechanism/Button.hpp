#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Interface/ITriggerable.hpp"
#include "Object/Color.hpp"
#include "Util/GameObject.hpp"
#include <vector>

using ButtonColor = Color;

class Character;

class Button : public Util::GameObject {
public:
  Button(ButtonColor color, const glm::vec2 &pos);

  void Update(Character *fb, Character *wg);
  void LinkTrigger(ITriggerable *target);
  void SetPosition(const glm::vec2 &position);
  const SDL_Rect &getRect() const;
  void SetInitialPosition(const glm::vec2 &pos);
  void Respawn();
  ButtonColor GetColor() const;

private:
  ButtonColor m_Color;
  mutable SDL_Rect m_Rect;
  bool m_IsPressedFireboy = false;
  bool m_IsPressedWatergirl = false;
  std::vector<ITriggerable *> m_Triggers;
  glm::vec2 m_InitialPosition;
  std::string GetImagePath(ButtonColor color);
};

#endif // BUTTON_HPP