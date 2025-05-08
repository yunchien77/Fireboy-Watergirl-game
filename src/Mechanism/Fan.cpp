#include "Mechanism/Fan.hpp"
#include "Character/Character.hpp"
#include "Util/Image.hpp"
#include "config.hpp"
#include <cmath>
#include <iostream>
#include <map>
#include <random>

Fan::Fan(const glm::vec2 &position, float strength, FanColor color)
    : m_InitialPosition(position), m_Strength(strength),
      m_MaxWindHeight(250.0f), m_AnimationTime(0.0f), m_Color(color) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath()));
  SetPosition(position);
  SetPivot({0.0f, 14.0f});
  SetZIndex(20);
}

std::string Fan::GetImagePath() const { return GetImagePath(m_Color); }

std::string Fan::GetImagePath(FanColor color) {
  std::string base = RESOURCE_DIR "/material/props/fan/fan-";
  switch (color) {
  case FanColor::BLUE:
    base += "blue";
    break;
  case FanColor::GREEN:
    base += "green";
    break;
  case FanColor::PINK:
    base += "pink";
    break;
  case FanColor::RED:
    base += "red";
    break;
  case FanColor::WHITE:
    base += "white";
    break;
  case FanColor::YELLOW:
    base += "yellow";
    break;
  }
  base += ".png";
  return base;
}

FanColor Fan::GetColor() const { return m_Color; }

bool Fan::IsCharacterInWindZone(Character *character) const {
  if (!character)
    return false;

  glm::vec2 charPos = character->GetPosition();
  glm::vec2 fanPos = m_Transform.translation;
  glm::vec2 fanSize = GetScaledSize();

  // 風扇的左右邊界
  float fanLeft = fanPos.x - (fanSize.x / 2.0f);
  float fanRight = fanPos.x + (fanSize.x / 2.0f);

  // 角色的左右邊界
  glm::vec2 charSize = character->GetSize();
  float charLeft = charPos.x - (charSize.x / 2.0f);
  float charRight = charPos.x + (charSize.x / 2.0f);

  // 檢查角色是否在風扇正上方（水平重疊）
  bool horizontalOverlap = (charRight > fanLeft) && (charLeft < fanRight);

  // 檢查角色是否在風扇上方
  bool isAboveFan = charPos.y > (fanPos.y - 28.0);

  return isAboveFan && horizontalOverlap;
}

void Fan::ApplyWindForce(Character *character) {
  if (!IsCharacterInWindZone(character)) {
    if (character->IsAffectedByWind()) {
      // Gradually decrease wind effect when character leaves wind zone
      character->ApplyExternalForce(-0.15f);
    } else {
      character->SetAffectedByWind(false);
    }
    return;
  }

  character->SetAffectedByWind(true);
  glm::vec2 charPos = character->GetPosition();
  glm::vec2 fanPos = m_Transform.translation;

  float distanceFromFan = std::abs(charPos.y - (fanPos.y - 16.0f));
  if (distanceFromFan > m_MaxWindHeight) {
    character->SetAffectedByWind(false);
    return;
  }

  float distanceFactor =
      1.0f - std::pow(distanceFromFan / m_MaxWindHeight, 0.3f);

  float minForceFactor = 0.6f;
  distanceFactor = std::max(minForceFactor, distanceFactor);

  float windVariation = std::sin(m_AnimationTime * 0.003f) * 0.15f + 1.0f;

  float idealHeight = fanPos.y + m_MaxWindHeight * 0.8f;
  float heightDifference = idealHeight - charPos.y;

  float heightAdjustFactor = 1.2f;

  if (std::abs(heightDifference) > 20.0f) {
    if (heightDifference > 0) {
      heightAdjustFactor = 1.8f;
    } else {
      heightAdjustFactor = 0.9f;
    }
  }

  float baseForce = m_Strength * 0.15f;

  static std::map<Character *, float> lastForces;
  float lastForce = lastForces.count(character) ? lastForces[character] : 0.0f;

  float newForce =
      baseForce * distanceFactor * windVariation * heightAdjustFactor;

  float smoothedForce = lastForce * 0.7f + newForce * 0.3f;

  smoothedForce = std::min(smoothedForce, 0.65f);
  smoothedForce = std::max(smoothedForce, 0.12f);

  lastForces[character] = smoothedForce;
  character->ApplyExternalForce(smoothedForce);
}

void Fan::Update(float deltaTime) {
  m_AnimationTime += deltaTime * 1000.0f; // Update animation time
}

void Fan::SetPosition(const glm::vec2 &position) {
  m_Transform.translation = position;
}

const SDL_Rect &Fan::getRect() const {
  glm::vec2 pos = m_Transform.translation;
  glm::vec2 size = GetScaledSize();

  m_Rect.x = static_cast<int>(pos.x - size.x / 2);
  m_Rect.y = static_cast<int>(pos.y - size.y / 2);
  m_Rect.w = static_cast<int>(size.x);
  m_Rect.h = static_cast<int>(size.y);

  return m_Rect;
}

void Fan::SetInitialPosition(const glm::vec2 &pos) { m_InitialPosition = pos; }

void Fan::Respawn() { SetPosition(m_InitialPosition); }