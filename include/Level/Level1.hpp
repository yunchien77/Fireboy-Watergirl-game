#pragma once
#include "Level/Level.hpp"

class Level1 : public Level {
public:
  using Level::Level;

  bool Initialize() override;
};