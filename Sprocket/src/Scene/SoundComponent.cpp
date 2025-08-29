#include "SoundComponent.h"

#include "Events/AudioEvent.h"

namespace Sprocket {

    void SoundComponent::SetFilepath(const std::string& filepath) {
        m_Filepath = filepath;
        AudioNewEvent* event = new AudioNewEvent(m_Filepath);
        m_EventCallback(*event);
        m_SoundID = event->GetSoundID();
        delete event;
    }

    const std::string& SoundComponent::GetFilepath() const {
        return m_Filepath;
    }

    void SoundComponent::Play() {
        AudioActionEvent* event = new AudioActionEvent(m_SoundID, AudioActionType::PLAY);
        m_EventCallback(*event);
        delete event;
    }

    void SoundComponent::Stop() {
        AudioActionEvent* event = new AudioActionEvent(m_SoundID, AudioActionType::STOP);
        m_EventCallback(*event);
        delete event;
    }

    void SoundComponent::Reset() {
        AudioActionEvent* event = new AudioActionEvent(m_SoundID, AudioActionType::RESET);
        m_EventCallback(*event);
        delete event;
    }

    bool SoundComponent::IsPlaying() const {
        AudioGetterEvent* event = new AudioGetterEvent(m_SoundID, AudioGetterType::IS_PLAYING);
        m_EventCallback(*event);
        bool isPlaying = event->GetBoolValue();
        delete event;
        return isPlaying;
    }

    void SoundComponent::SetVolume(float volume) {
        AudioSetterEvent* event = new AudioSetterEvent(m_SoundID, AudioSetterType::VOLUME, volume);
        m_EventCallback(*event);
        delete event;
    }

    float SoundComponent::GetVolume() const {
        AudioGetterEvent* event = new AudioGetterEvent(m_SoundID, AudioGetterType::VOLUME);
        m_EventCallback(*event);
        float volume = event->GetFloatValue();
        delete event;
        return volume;
    }

    void SoundComponent::SetLooping(bool loop) {
        AudioSetterEvent* event = new AudioSetterEvent(m_SoundID, AudioSetterType::LOOPING, loop);
        m_EventCallback(*event);
        delete event;
    }

    bool SoundComponent::IsLooping() const {
        AudioGetterEvent* event = new AudioGetterEvent(m_SoundID, AudioGetterType::LOOPING);
        m_EventCallback(*event);
        bool isLooping = event->GetBoolValue();
        delete event;
        return isLooping;
    }

}