#include "Mechanism/Fan.hpp"
#include "Character/Character.hpp"
#include "Util/Image.hpp"
#include "config.hpp"
#include <cmath>
#include <iostream>
#include <random>

Fan::Fan(const glm::vec2 &position, float strength, float windWidth)
    : m_InitialPosition(position), m_Strength(strength), m_WindWidth(windWidth),
      m_WindHeight(300.0f), // Default wind height effect
      m_MaxWindHeight(400.0f), m_AnimationTime(0.0f) {
  SetDrawable(std::make_shared<Util::Image>(GetImagePath()));
  SetPosition(position);
  SetPivot({0.0f, 14.0f}); // Center pivot
  SetZIndex(20);           // Ensure it's drawn at appropriate layer

  // Initialize wind particles
  m_WindParticles.reserve(30); // Reserve space for particles
  GenerateWindParticles();
}

std::string Fan::GetImagePath() const {
  return RESOURCE_DIR
      "/material/props/fan/fan-white.png"; // You'll need to create this asset
}

void Fan::Update(float deltaTime) {
  UpdateAnimation(deltaTime);
  UpdateWindParticles(deltaTime);
}

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

  bool isAboveFan = false;
  // 檢查角色是否在風扇上方
  if (charPos.y >= 0) {
    isAboveFan = charPos.y > (fanPos.y - 16.0);
  } else {
    isAboveFan = charPos.y < (fanPos.y - 16.0);
  }

  return isAboveFan && horizontalOverlap;
}

void Fan::ApplyWindForce(Character *character, float deltaTime) {
  if (!character || !IsCharacterInWindZone(character))
    return;

  glm::vec2 charPos = character->GetPosition();
  glm::vec2 fanPos = m_Transform.translation;

  // Calculate distance from fan (higher = less force)
  float distanceY = fanPos.y - charPos.y;
  float distanceFactor = std::max(0.0f, 1.0f - (distanceY / m_MaxWindHeight));

  // 計算垂直風力 - 符號修正：正值代表向上浮力
  float verticalForce = m_Strength * distanceFactor * deltaTime;

  // 應用風力到角色，將垂直力設為正值代表向上
  character->ApplyExternalForce(verticalForce);
}

void Fan::UpdateAnimation(float deltaTime) {
  m_AnimationTime += deltaTime;

  // Update fan rotation animation here
  // This would rotate the fan blade sprite
  // SetRotation(m_AnimationTime * 360.0f); // If you have rotation support
}

void Fan::UpdateWindParticles(float deltaTime) {
  // Update existing particles
  for (auto it = m_WindParticles.begin(); it != m_WindParticles.end();) {
    it->lifeTime -= deltaTime;
    it->position.y -= it->speed * deltaTime;

    // Add some horizontal wobble
    it->position.x += std::sin(it->lifeTime * 5.0f) * 0.5f * deltaTime;

    if (it->lifeTime <= 0.0f) {
      it = m_WindParticles.erase(it);
    } else {
      ++it;
    }
  }

  // Generate new particles
  if (m_WindParticles.size() < 30 &&
      m_AnimationTime - std::floor(m_AnimationTime) < 0.1f) {
    GenerateWindParticles();
  }
}

void Fan::GenerateWindParticles() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> xDist(-m_WindWidth / 2.0f,
                                              m_WindWidth / 2.0f);
  std::uniform_real_distribution<float> yDist(0.0f, 20.0f);
  std::uniform_real_distribution<float> speedDist(40.0f, 80.0f);
  std::uniform_real_distribution<float> lifeDist(1.0f, 2.5f);

  glm::vec2 fanPos = m_Transform.translation;

  // Create 3-5 new particles
  for (int i = 0; i < 5; ++i) {
    WindParticle particle;
    particle.position = fanPos + glm::vec2(xDist(gen), -yDist(gen));
    particle.speed = speedDist(gen);
    particle.maxLifeTime = lifeDist(gen);
    particle.lifeTime = particle.maxLifeTime;
    m_WindParticles.push_back(particle);
  }
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