#ifndef IGEM_HPP
#define IGEM_HPP

#include "Character/Character.hpp"
#include "Util/GameObject.hpp"

class IGem : public Util::GameObject {
public:
  virtual ~IGem() = default;
  virtual void OnCharacterEnter(Character *character) = 0;
  virtual const SDL_Rect &getRect() const = 0;
  virtual void Respawn() = 0;
};

#endif
