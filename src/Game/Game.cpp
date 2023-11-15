#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include "GameObjects/Tank.h"
#include "Level.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>

Game::Game(const glm::ivec2 WindowSize)
    : m_eCurrentGameState(EGameState::Active),
      m_WindowSize(WindowSize)
{
    m_key.fill(false);
}
Game::~Game()
{
}

void Game::render()
{

    m_pTank->render();
    if (m_pLevel)
    {
        m_pLevel->render();
    }
}
void Game::update(const uint64_t delta)
{
    if (m_pLevel)
    {
        m_pLevel->update(delta);
    }

    if (m_pTank)
    {
        if (m_key[GLFW_KEY_W])
        {
            m_pTank->setOrientation(Tank::EOrientation::Top);
            m_pTank->move(true);
        }
        else if (m_key[GLFW_KEY_A])
        {
            m_pTank->setOrientation(Tank::EOrientation::Left);
            m_pTank->move(true);
        }
        else if (m_key[GLFW_KEY_S])
        {
            m_pTank->setOrientation(Tank::EOrientation::Bottom);
            m_pTank->move(true);
        }
        else if (m_key[GLFW_KEY_D])
        {
            m_pTank->setOrientation(Tank::EOrientation::Right);
            m_pTank->move(true);
        }
        else
        {
            m_pTank->move(false);
        }

        m_pTank->update(delta);
    }
}
void Game::setKey(const int key, const int action)
{
    m_key[key] = action;
}

bool Game::init()
{
    ResourceManager::loadJSONResources("res/resources.json");
    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't find shader program"
                  << "spriteShader" << std::endl;
        return false;
    }

    auto pTextureAtlas = ResourceManager::getTexture("mapTextureAtlas");
    if (!pTextureAtlas)
    {
        std::cerr << "Can't find texture atlas"
                  << "MapTextureAtlas" << std::endl;
        return false;
    }
    auto pTanksTextureAtlas = ResourceManager::getTexture("tankTextureAtlas");
    if (!pTanksTextureAtlas)
    {
        std::cerr << "Can't find texture atlas"
                  << "tankTextureAtlas" << std::endl;
        return false;
    }

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_WindowSize.x), 0.f, static_cast<float>(m_WindowSize.y), -100.f, 100.f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    auto pTanksAnimatedSprite = ResourceManager::getAnimatedSprite("tanksAnimatedSprite");

    pTanksAnimatedSprite->setState("tankTopState");
    if (!pTanksAnimatedSprite)
    {
        std::cerr << "Can't find aimated sprite"
                  << "TanksAnimatedSprite" << std::endl;
        return false;
    }

    m_pTank = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f, glm::vec2(0), glm::vec2(16.f, 16.f));

    m_pLevel = std::make_unique<Level>(ResourceManager::getLevels()[0]);

    return true;
}