#ifndef LEVER_HPP
#define LEVER_HPP

#include "Character/Character.hpp"
#include "Interface/ICollidable.hpp"
#include "Interface/IInteractable.hpp"
#include "Interface/ITriggerable.hpp"
#include "Object/Color.hpp"
#include "Util/GameObject.hpp"
#include <vector>

using LeverColor = Color;

class Lever : public Util::GameObject,
              public ICollidable,
              public IInteractable {
public:
  Lever(LeverColor color, const glm::vec2 &pos);

  // ICollidable interface
  const SDL_Rect &getRect() const override;

  // IInteractable interface
  void OnCharacterEnter(Character *character) override;
  // void OnCharacterExit(Character *character) override;

  // Lever specific methods
  void update(Character *fb, Character *wg);
  void linkTrigger(ITriggerable *target);
  LeverColor GetColor() const;
  void SetPosition(const glm::vec2 &position);
  void Respawn();
  void SetInitialState(const glm::vec2 &pos, bool isOn);

private:
  bool m_IsOn = false;
  LeverColor m_Color;
  mutable SDL_Rect m_Rect{};
  glm::vec2 m_InitialPosition;
  bool m_InitialIsOn = false;
  std::vector<ITriggerable *> m_Triggers;

  // Track interacting characters
  std::vector<Character *> m_InteractingCharacters;

  void Toggle();
  std::string GetImagePath(LeverColor color, bool isOn) const;
  bool ShouldToggle(Character *character) const;
};

#endif