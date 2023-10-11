#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include "Tank.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>

    Game::Game(const glm::ivec2 WindowSize)
    :m_eCurrentGameState(EGameState::Active),
    m_WindowSize(WindowSize)
    {
        m_key.fill(false);
    }
    Game::~Game(){

    }

    void Game::render(){
        ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();

        m_pTank->render();
    }
    void Game::update(const uint64_t delta){
        ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);

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
            else{
                m_pTank->move(false);
            }
            
            
            m_pTank->update(delta);
        }
    }
    void Game::setKey(const int key,const int action){
        m_key[key]=action;
    }

    bool Game::init(){
auto pDefaultShaderProgram = ResourceManager::loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "Can't create program: DefaultShader" << std::endl;
            return false;
        }

        auto pSpriteShaderProgram = ResourceManager::loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram)
        {
            std::cerr << "Can't create program: SpriteShader" << std::endl;
            return -false;
        }

        auto tex = ResourceManager::loadTextures("DefaultTexture", "res/textures/map_16x16.png");

        std::vector<std::string> SubTexturesName = {"BrickWall",
                                                    "topBrick",
                                                    "botBrick",
                                                    "leftBrick",
                                                    "rightBrick",
                                                    "topLeftBrick",
                                                    "topRightBrick",
                                                    "BotLeftBrick",
                                                    "botRightBrick",
                                                    "IronWall",
                                                    "topIronWall",
                                                    "bottomIronWall",
                                                    "leftIronWall",
                                                    "rightIronWall",
                                                    "topLeftIronWall",
                                                    "topRightIronWall",

                                                    "bottomLeftIronWall",
                                                    "bottomRightIronWall",
                                                    "water1",
                                                    "water2",
                                                    "water3",
                                                    "trees",
                                                    "ice",
                                                    "wall",

                                                    "eagle",
                                                    "deadEagle",
                                                    "nothing",
                                                    "respawn1",
                                                    "respawn2",
                                                    "respawn3",
                                                    "respawn4"};
        auto pTanksTextureAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(SubTexturesName), 16, 16);

        auto pSprite = ResourceManager::loadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, 0, "IronWall");
        pSprite->setPosition(glm::vec2(250, 100));

        auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, 0, "IronWall");
        pAnimatedSprite->setPosition(glm::vec2(300, 300));
        std::vector<std::pair<std::string, uint64_t>> waterState;
        waterState.emplace_back(std::pair<std::string, uint64_t>("water1", 500000000));
        waterState.emplace_back(std::pair<std::string, uint64_t>("water2", 500000000));
        waterState.emplace_back(std::pair<std::string, uint64_t>("water3", 500000000));

        std::vector<std::pair<std::string, uint64_t>> eagleState;
        eagleState.emplace_back(std::pair<std::string, uint64_t>("eagle", 1000000000));
        eagleState.emplace_back(std::pair<std::string, uint64_t>("deadEagle", 1000000000));

        pAnimatedSprite->insertState("waterState", std::move(waterState));
        pAnimatedSprite->insertState("eagleState", std::move(eagleState));

        pAnimatedSprite->setState("waterState");


        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        glm::mat4 modelMatrix_1 = glm::mat4(1.f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 200.f, 0.f));

        glm::mat4 modelMatrix_2 = glm::mat4(1.f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(540.f, 200.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_WindowSize.x), 0.f, static_cast<float>(m_WindowSize.y), -100.f, 100.f);

        pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

         std::vector<std::string> TankSubTexturesName = {
                                                    "TankTop1",
                                                    "TankTop2",
                                                    "TankLeft1",
                                                    "TankLeft2",
                                                    "TankBot1",
                                                    "TankBot2",
                                                    "TankRight1",
                                                    "TankRight2"
                                                                };

        auto pTextureAtlas = ResourceManager::loadTextureAtlas("TankTextureAtlas", "res/textures/tanks.png", std::move(TankSubTexturesName), 16, 16);

        auto pTanksAnimatedSprite = ResourceManager::loadAnimatedSprite("TanksAnimatedSprite", "TankTextureAtlas", "SpriteShader", 100, 100, 0, "TankTop1");

        std::vector<std::pair<std::string, uint64_t>> tankTopState;
        tankTopState.emplace_back(std::pair<std::string, uint64_t>("TankTop1", 500000000));
        tankTopState.emplace_back(std::pair<std::string, uint64_t>("TankTop2", 500000000));

        std::vector<std::pair<std::string, uint64_t>> tankLeftState;
        tankLeftState.emplace_back(std::pair<std::string, uint64_t>("TankLeft1", 500000000));
        tankLeftState.emplace_back(std::pair<std::string, uint64_t>("TankLeft2", 500000000));

        std::vector<std::pair<std::string, uint64_t>> tankBotState;
        tankBotState.emplace_back(std::pair<std::string, uint64_t>("TankBot1", 500000000));
        tankBotState.emplace_back(std::pair<std::string, uint64_t>("TankBot2", 500000000));

        std::vector<std::pair<std::string, uint64_t>> tankRightState;
        tankRightState.emplace_back(std::pair<std::string, uint64_t>("TankRight1", 500000000));
        tankRightState.emplace_back(std::pair<std::string, uint64_t>("TankRight2", 500000000));

        pTanksAnimatedSprite->insertState("tankTopState", std::move(tankTopState));
        pTanksAnimatedSprite->insertState("tankLeftState", std::move(tankLeftState));
        pTanksAnimatedSprite->insertState("tankBotState", std::move(tankBotState));
        pTanksAnimatedSprite->insertState("tankRightState", std::move(tankRightState));

        pTanksAnimatedSprite->setState("tankTopState");

        m_pTank = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f,glm::vec2(100.f, 100.f));


        return true;
    }