#pragma once
#include "Level/Level.hpp"

class Level3 : public Level {
public:
  using Level::Level;

  bool Initialize() override;
};