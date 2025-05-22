#pragma once
#include "Level/Level.hpp"

class Level4 : public Level {
public:
    using Level::Level;

    bool Initialize() override;
};