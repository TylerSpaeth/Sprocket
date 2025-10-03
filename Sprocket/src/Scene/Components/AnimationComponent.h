#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include "Component.h"
#include "QuadRendererComponent.h"
#include "Subcomponents/Animation.h"

#include "Core/Sprocket.pch"

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket {

    class SPROCKET_API AnimationComponent : public EventDrivenComponent {

        friend class Entity;

    private:

        // Animation components store a quad renderer component that they use for actually 
        // displaying sprites.
        std::shared_ptr<QuadRendererComponent> m_QuadRenderer = nullptr;
        Animation m_Animation;

        float m_AnimationTotalTime = 0;
        float m_ElapsedTime = 0;

    public:

        AnimationComponent();

        /// @brief Sets the Animation that should be playing.
        const bool SetAnimation(const Animation& animation);

    private:

        void RegisterEventCallback(const std::function<void(Event&)> eventCallback) override;

        /// @brief Registers the component in the event system
        void Register();

        /// @brief Updates the animation. 
        /// Should be called every frame so the animation clock stays in sync.
        /// @param deltaTime - The elapsed time between frames
        void UpdateAnimation(float deltaTime);

    };

}

#endif