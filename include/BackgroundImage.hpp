#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class BackgroundImage : public Util::GameObject {

public:
  //   BackgroundImage()
  //       : GameObject(std::make_unique<Util::Image>(
  //                        RESOURCE_DIR "/material/background/cover.png"),
  //                    -10) {}
  BackgroundImage(const std::string &imagePath)
      : GameObject(std::make_unique<Util::Image>(imagePath), -10) {}

private:
};

#endif // BACKGROUND_IMAGE_HPP
