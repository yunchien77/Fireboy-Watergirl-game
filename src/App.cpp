#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <memory>
#include <SDL.h>
#include "config.hpp"

App::App() {
    LOG_DEBUG("Game Initialize");

    m_TitleBackground = std::make_shared<BackgroundImage>(
        RESOURCE_DIR "/material/background/cover.png");
    m_TitleBackground->SetVisible(false);
    m_Root.AddChild(m_TitleBackground);

    m_LevelBackground = std::make_shared<BackgroundImage>(
        RESOURCE_DIR "/material/background/level-page.png");
    m_LevelBackground->SetVisible(false);
    m_Root.AddChild(m_LevelBackground);

    m_Button1 = std::make_shared<BackgroundImage>(
    RESOURCE_DIR "/material/background/button/current-level.png");
    m_Button1->SetPosition({0, -220});
    m_Button1->SetVisible(false);
    m_Root.AddChild(m_Button1);


    m_Button2 = std::make_shared<BackgroundImage>(
    RESOURCE_DIR "/material/background/button/unlevel.png");
    m_Button2->SetPosition({0, -102});
    m_Button2->SetVisible(false);
    m_Root.AddChild(m_Button2);

    m_Button3 = std::make_shared<BackgroundImage>(
    RESOURCE_DIR "/material/background/button/unlevel.png");
    m_Button3->SetPosition({0, 16});
    m_Button3->SetVisible(false);
    m_Root.AddChild(m_Button3);

    m_Button4 = std::make_shared<BackgroundImage>(
    RESOURCE_DIR "/material/background/button/unlevel.png");
    m_Button4->SetPosition({0, 134});
    m_Button4->SetVisible(false);
    m_Root.AddChild(m_Button4);

    m_Button5 = std::make_shared<BackgroundImage>(
    RESOURCE_DIR "/material/background/button/unlevel.png");
    m_Button5->SetPosition({0, 252});
    m_Button5->SetVisible(false);
    m_Root.AddChild(m_Button5);

    m_Button1->SetZIndex(10);
    m_Button2->SetZIndex(5);
    m_Button3->SetZIndex(4);
    m_Button4->SetZIndex(3);
    m_Button5->SetZIndex(2);

    m_Level1Background = std::make_shared<BackgroundImage>(
    RESOURCE_DIR "/material/background/level1.jpg");
    m_Level1Background->SetVisible(false);
    m_Root.AddChild(m_Level1Background);

    m_Root.Update();
}

void App::Start() {
    LOG_TRACE("Start");
    m_TitleBackground->SetVisible(true);

    if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
        m_CurrentState = State::LEVEL_SELECT;
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    m_Root.Update();
}

void App::LevelSelect() {
    m_Button1->SetVisible(true);
    m_Button2->SetVisible(true);
    m_Button3->SetVisible(true);
    m_Button4->SetVisible(true);
    m_Button5->SetVisible(true);

    int buttonCenterX = 0;
    int buttonCenterY = -220;
    int buttonHalfSize = 85;

    if (m_CurrentState == State::LEVEL_SELECT) {
        m_TitleBackground->SetVisible(false);
        m_LevelBackground->SetVisible(true);
        m_Button1->SetVisible(true);

        int mouseX, mouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);


        int windowWidth = WINDOW_WIDTH;
        int windowHeight = WINDOW_HEIGHT;
        SDL_Window* window = SDL_GL_GetCurrentWindow();
        SDL_Renderer* renderer = SDL_GetRenderer(window);
        SDL_Rect viewport;
        SDL_RenderGetViewport(renderer, &viewport);
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        int relativeMouseX = mouseX - (windowWidth / 2);
        int relativeMouseY = (windowHeight / 2) - mouseY;


        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {

            if (relativeMouseX >= buttonCenterX - buttonHalfSize && relativeMouseX <= buttonCenterX + buttonHalfSize &&
                relativeMouseY >= buttonCenterY - buttonHalfSize && relativeMouseY <= buttonCenterY + buttonHalfSize) {
                m_CurrentState = State::GAME_PLAY;
                m_Button1->SetVisible(false);
                m_LevelBackground->SetVisible(false);
                return;
            }
        }

        if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
            m_CurrentState = State::END;
        }
    }
    m_Root.Update();
}


void App::GamePlay() {
    LOG_TRACE("GamePlay");

    if (m_CurrentState == State::GAME_PLAY) {
        m_TitleBackground->SetVisible(false);
        m_LevelBackground->SetVisible(false);
        m_Button1->SetVisible(false);
        m_Button2->SetVisible(false);
        m_Button3->SetVisible(false);
        m_Button4->SetVisible(false);
        m_Button5->SetVisible(false);
        m_Level1Background->SetVisible(true);

        if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
            m_CurrentState = State::END;
        }
    }

    m_Root.Update();
}

void App::GameOver() {
    LOG_TRACE("GameOver");
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
    m_Root.Update();
}

void App::GameWin() {
    LOG_TRACE("GameWin");
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
    m_Root.Update();
}

void App::End() {
    LOG_TRACE("開始 - End");
    LOG_TRACE("結束 - End");
}
