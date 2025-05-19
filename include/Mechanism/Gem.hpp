#ifndef GEM_HPP
#define GEM_HPP

#include "Interface/ICollidable.hpp"
#include "Interface/IGem.hpp"
#include "Interface/IInteractable.hpp"

class Character;

enum class GemType { FIRE, WATER, GREEN };

class Gem : public IGem, public ICollidable, public IInteractable {
public:
  explicit Gem(GemType type);

  // IGem interface
  void OnCharacterEnter(Character *character) override;
  const SDL_Rect &getRect() const override;
  bool IsCollected() const override;
  void Respawn() override;

  // IInteractable interface
  // void
  // OnCharacterExit(Character *character) override{}; // Optional
  // implementation

  // Gem specific methods
  void SetPosition(const glm::vec2 &position);
  GemType GetType() const;
  void SetInitialPosition(const glm::vec2 &pos);

private:
  GemType m_Type;
  mutable SDL_Rect m_Rect;
  glm::vec2 m_InitialPosition;
};

#endif // GEM_HPP