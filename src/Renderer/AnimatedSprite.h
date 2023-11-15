#pragma once

#include "Sprite.h"
#include "Texture2D.h"
#include <vector>
#include <map>

namespace RenderEngine
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
                       std::shared_ptr<ShaderProgram> pShaderProgram);
        void insertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTextureDuration);
        void render(const glm::vec2 &position, const glm::vec2 size, const float rotation) const override;

        void setState(const std::string &newState);

        void update(const size_t delta);
    };

};