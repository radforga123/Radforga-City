#pragma once

#include "Sprite.h"
#include "Texture2D.h"
#include <vector>
#include <map>

namespace Renderer
{

    class AnimatedSprite : public Sprite
    {
    private:
        std::map<std::string, std::vector<std::pair<std::string, uint64_t>>> m_statesMape;
        size_t m_currentFrame = 0;
        uint64_t m_currerntAnimationTime = 0;
        std::map<std::string, std::vector<std::pair<std::string, uint64_t>>>::const_iterator m_pCurrentAnimationDuration;
        mutable bool m_dirty = false;

    public:
        AnimatedSprite(std::shared_ptr<Texture2D> pTexture,
                       std::string initialSubTexture,
                       std::shared_ptr<ShaderProgram> pShaderProgram,
                       const glm::vec2 &position = glm::vec2(0.f),
                       const glm::vec2 &size = glm::vec2(1.f),
                       const float &rotation = 0.f);
        void insertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTextureDuration);
        void render() const override;

        void setState(const std::string &newState);

        void update(const size_t delta);
    };

};