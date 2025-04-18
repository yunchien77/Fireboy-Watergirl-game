#pragma once
#include "Level/Level.hpp"

class Level2 : public Level {
public:
  using Level::Level;

  bool Initialize() override;
};