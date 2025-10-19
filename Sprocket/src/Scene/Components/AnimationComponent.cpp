#include "AnimationComponent.h"

#include "Core/Global.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    AnimationComponent::AnimationComponent() { m_EventCallback = nullptr; }

    const bool AnimationComponent::SetAnimation(const Animation& animation) {

        float totalDuration = 0;

        int index = 0;
        for (auto frame : animation.animationFrames) {
            if (frame.frameDuration < 0) {
                Global::FileLogger().Warning(std::format("Failed to set animation, frame {} has a negative frame duration.", index));
                return false;
            }
            index++;

            totalDuration += frame.frameDuration;
        }

        m_Animation = animation;
        m_AnimationTotalTime = totalDuration;

        return true;

    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void AnimationComponent::RegisterEventCallback(const std::function<void(Event&)> eventCallback) {
        m_EventCallback = eventCallback;
        Register();
        m_QuadRenderer->RenderNew();
    }

    void AnimationComponent::Register() {

        m_QuadRenderer = std::make_shared<QuadRendererComponent>();

        m_QuadRenderer.get()->m_EventCallback = m_EventCallback;
   
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

    void AnimationComponent::OnActivate(OnActivateParams& onActivateParams) {
        RegisterEventCallback(onActivateParams.eventCallback);
        m_QuadRenderer->UpdateModelMatrix(onActivateParams.position, onActivateParams.rotation, onActivateParams.scale);
    }

    void AnimationComponent::OnDeactivate(OnDeactivateParams& onDeactivateParams) {
        m_QuadRenderer->RemoveRender();
        m_EventCallback = nullptr;
        m_QuadRenderer->m_EventCallback = nullptr;
        m_ElapsedTime = 0;
    }

    void AnimationComponent::OnUpdate(OnUpdateParams& onUpdateParams) {
        if (onUpdateParams.updatedTransform) {
            m_QuadRenderer->UpdateModelMatrix(onUpdateParams.position, onUpdateParams.rotation, onUpdateParams.scale);
        }
        UpdateAnimation(onUpdateParams.deltaTime);
    }

}