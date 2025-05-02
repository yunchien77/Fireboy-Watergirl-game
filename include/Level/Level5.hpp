#pragma once
#include "Level/Level.hpp"

class Level5 : public Level {
public:
  using Level::Level;

  bool Initialize() override;
};