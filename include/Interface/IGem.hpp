#ifndef IGEM_HPP
#define IGEM_HPP

#include "Character/Character.hpp"

class IGem {
public:
    virtual ~IGem() = default;
    virtual void OnCharacterEnter(Character* character) = 0;
    virtual const SDL_Rect& getRect() const = 0;

    virtual void Respawn() = 0;
};


#endif
