#pragma once

#include <array>
#include <memory>
#include <glm/vec2.hpp>

class Tank;
class Level;

class Game
{
public:
    Game(const glm::ivec2 WindowSize);
    ~Game();

    void render();
    void update(const uint64_t delta);
    void setKey(const int key, const int action);

    bool init();

private:
    std::array<bool, 349> m_key;

    enum EGameState
    {
        Active,
        Pause
    };

    glm::ivec2 m_WindowSize;

    EGameState m_eCurrentGameState;

    std::unique_ptr<Tank> m_pTank;
    std::unique_ptr<Level> m_pLevel;
};
