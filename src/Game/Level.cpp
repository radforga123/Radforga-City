#include "Level.h"

#include "../Resources/ResourceManager.h"
#include "GameObjects/BrickWall.h"

#include <iostream>

unsigned int blockSize = 16;

std::shared_ptr<IGameObject> createGameObjFDesc(const char desc, const glm::vec2 &position, const glm::vec2 &size, const float rotation)
{
    switch (desc)
    {
    case '0':
        return std::make_shared<BrickWall>(ResourceManager::getSprite("brickWall_right"), position, size, rotation);
        break;
    case '1':
        return std::make_shared<BrickWall>(ResourceManager::getSprite("brickWall_bottom"), position, size, rotation);
        break;
    case '2':
        return std::make_shared<BrickWall>(ResourceManager::getSprite("brickWall_left"), position, size, rotation);
        break;
    case '3':
        return std::make_shared<BrickWall>(ResourceManager::getSprite("brickWall_top"), position, size, rotation);
        break;
    case '4':
        return std::make_shared<BrickWall>(ResourceManager::getSprite("brickWall"), position, size, rotation);
        break;
    case 'F':
        return std::make_shared<BrickWall>(ResourceManager::getSprite("brickWall_bottom_left"), position, size, rotation);
        break;
    case 'G':
        return std::make_shared<BrickWall>(ResourceManager::getSprite("brickWall_bottom_right"), position, size, rotation);
        break;
    case 'I':
        return std::make_shared<BrickWall>(ResourceManager::getSprite("brickWall_top_left"), position, size, rotation);
        break;
    case 'J':
        return std::make_shared<BrickWall>(ResourceManager::getSprite("brickWall_top_right"), position, size, rotation);
        break;
    case 'D':
        return nullptr;
        break;
    default:
        std::cerr << "UnknownGameObject" << std::endl;
        break;
    }
}

Level::Level(const std::vector<std::string> &levelDescription)
{
    if (levelDescription.empty())
    {
        std::cerr << "Empty level desc" << std::endl;
    }

    m_width = levelDescription[0].length();
    m_height = levelDescription.size();

    m_mapObject.reserve(m_width * m_height);
    unsigned int currentBottomOffset = static_cast<unsigned int>(blockSize * (m_height - 1));
    for (auto &currebtRow : levelDescription)
    {
        unsigned int currentLeftOffset = 0;
        for (auto &currentElement : currebtRow)
        {
            m_mapObject.emplace_back(createGameObjFDesc(currentElement, glm::vec2{currentLeftOffset, currentBottomOffset}, glm::vec2(blockSize, blockSize), 0.f));

            currentLeftOffset += blockSize;
        }
        currentBottomOffset -= blockSize;
    }
}

void Level::render() const
{
    for (auto &currentMapObject : m_mapObject)
    {
        if (currentMapObject)
        {
            currentMapObject->render();
        }
    }
}

void Level::update(const uint64_t delta)
{
    for (auto &currentMapObject : m_mapObject)
    {
        if (currentMapObject)
        {
            currentMapObject->update(delta);
        }
    }
}