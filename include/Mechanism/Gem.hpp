#ifndef GEM_HPP
#define GEM_HPP

#include "Character/Character.hpp"
#include "Mechanism/MechanismBase.hpp"
#include <SDL.h>

enum class GemType { FIRE, WATER, GREEN };

class Gem : public MechanismBase {
public:
  Gem(GemType type, const glm::vec2 &pos);

  GemType GetType() const;
  void OnCharacterEnter(Character *character);
  bool IsCollected() const;

private:
  GemType m_Type;
};

#endif // GEM_HPP
