#include "AnimationComponent.h"

#include "Core/Global.h"

namespace Sprocket {

    AnimationComponent::AnimationComponent() {m_EventCallback = nullptr;}
    AnimationComponent::~AnimationComponent() {
        if (m_QuadRenderer) {
            delete m_QuadRenderer;
        }
    }

    void AnimationComponent::RegisterEventCallback(const std::function<void(Event&)> eventCallback) {
        m_EventCallback = eventCallback;
        Register();
        m_QuadRenderer->RenderNew();
    }

    void AnimationComponent::Register() {

        m_QuadRenderer = new QuadRendererComponent();

        m_QuadRenderer->m_EventCallback = m_EventCallback;
   
    }

    void AnimationComponent::UpdateAnimation(float deltaTime) {

        if (!m_EventCallback) {
            return;
        }

        m_ElapsedTime += deltaTime;

        float timeInCurrentAnimationLoop = fmod(m_ElapsedTime, m_AnimationTotalTime);

        float totalTime = 0;
        for (auto frame : m_Animation.animationFrames) {
            totalTime += frame.frameDuration;

            if (totalTime > timeInCurrentAnimationLoop) {
                m_QuadRenderer->SetSprite(frame.sprite);
                break;
            }
        }

    }

    bool AnimationComponent::SetAnimation(const Animation& animation) {

        float totalDuration = 0;

        int index = 0;
        for (auto frame : animation.animationFrames) {
            if (frame.frameDuration < 0) {
                Global::fileLogger.Warning(std::format("Failed to set animation, frame {} has a negative frame duration.", index));
                return false;
            }
            if (frame.sprite.texturePath.empty()) {
                Global::fileLogger.Warning(std::format("Failed to set animation, frame {} has an empty texturePath.", index));
                return false;
            }
            index++;

            totalDuration += frame.frameDuration;
        }

        m_Animation = animation;
        m_AnimationTotalTime = totalDuration;

        return true;

    }

}