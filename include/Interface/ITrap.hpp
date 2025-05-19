#ifndef ITRAP_HPP
#define ITRAP_HPP

#include "Character/Character.hpp"
#include "Util/GameObject.hpp"

class ITrap : public Util::GameObject {
public:
  virtual ~ITrap() = default;
  virtual void OnCharacterEnter(Character *character) = 0;
};

#endif