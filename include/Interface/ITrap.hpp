#ifndef ITRAP_HPP
#define ITRAP_HPP

#include "Character/Character.hpp"

class ITrap {
public:
    virtual ~ITrap() = default;

    // 當角色碰到這個陷阱時觸發
    virtual void OnCharacterEnter(Character* character) = 0;
};

#endif
