#include "AudioMananger.h"

#include "Core/Global.h"

#include "Events/AudioEvent.h"

#include "ThirdParty/miniaudio/miniaudio.h"

namespace Sprocket {

    void AudioManager::OnEvent(Event& event) {
        switch(event.GetEventType()) {
            case EventType::AUDIO_NEW: {
                AudioNewEvent& newEvent = static_cast<AudioNewEvent&>(event);
                newEvent.m_SoundID = LoadSound(newEvent.GetFilepath());
                break;
            }
            case EventType::AUDIO_DELETE: {
                AudioDeleteEvent& deleteEvent = static_cast<AudioDeleteEvent&>(event);
                UnloadSound(deleteEvent.GetSoundID());
                break;
            }
            case EventType::AUDIO_ACTION: {
                AudioActionEvent& actionEvent = static_cast<AudioActionEvent&>(event);
                if (actionEvent.GetActionType() == AudioActionType::PLAY) {
                    Play(actionEvent.GetSoundID());
                }
                else if (actionEvent.GetActionType() == AudioActionType::STOP) {
                    Stop(actionEvent.GetSoundID());
                }
                else if(actionEvent.GetActionType() == AudioActionType::RESET) {
                    Reset(actionEvent.GetSoundID());
                }
                break;
            }
            case EventType::AUDIO_SETTER: {
                AudioSetterEvent& setterEvent = static_cast<AudioSetterEvent&>(event);
                switch(setterEvent.GetSetterType()) {
                    case AudioSetterType::VOLUME:
                        SetSoundVolume(setterEvent.GetSoundID(), setterEvent.GetFloatValue());
                        break;
                    case AudioSetterType::LOOPING:
                        SetSoundLooping(setterEvent.GetSoundID(), setterEvent.GetBoolValue());
                        break;
                }
                break;
            }
            case EventType::AUDIO_GETTER: {
                AudioGetterEvent& getterEvent = static_cast<AudioGetterEvent&>(event);
                switch(getterEvent.GetGetterType()) {
                    case AudioGetterType::VOLUME:
                        getterEvent.m_FloatValue = GetSoundVolume(getterEvent.GetSoundID());
                        break;
                    case AudioGetterType::LOOPING:
                        getterEvent.m_BoolValue = IsSoundLooping(getterEvent.GetSoundID());
                        break;
                    case AudioGetterType::IS_PLAYING:
                        getterEvent.m_BoolValue = IsSoundPlaying(getterEvent.GetSoundID());
                        break;
                }
                break;
            }
            case EventType::APP_START:
                OnStart();
                break;
            case EventType::APP_SHUTDOWN:
                OnShutdown();
                break;
        }
    }

    int AudioManager::LoadSound(const std::string& filepath) {
        Sound* newSound = new Sound(m_NativeAudioEngine, filepath);

        unsigned int index = m_FreeSoundIndexes.top();
        m_FreeSoundIndexes.pop();

        if(index == m_Sounds.size()) {
            m_Sounds.push_back(newSound);
            m_FreeSoundIndexes.push(index + 1);
        }
        else {
            m_Sounds[index] = newSound;
        }
        

        return index;
    }

    void AudioManager::UnloadSound(int soundID) {
        if(soundID < 0 || soundID >= m_Sounds.size() || m_Sounds[soundID] == nullptr) {
            Global::fileLogger.Warning(std::format("Attempted to unload invalid sound ID: {}", soundID));
            return;
        }
        delete m_Sounds[soundID];
        m_Sounds[soundID] = nullptr;
        m_FreeSoundIndexes.push(soundID);
    }

    void AudioManager::Play(int soundID) {
        m_Sounds[soundID]->Play();
    }

    void AudioManager::Stop(int soundID) {
        m_Sounds[soundID]->Stop();
    }

    void AudioManager::Reset(int soundID) {
        m_Sounds[soundID]->Reset();
    }

    bool AudioManager::IsSoundPlaying(int soundID) {
        return m_Sounds[soundID]->IsPlaying();
    }

    void AudioManager::SetSoundVolume(int soundID, float volume) {
        m_Sounds[soundID]->SetVolume(volume);
    }

    float AudioManager::GetSoundVolume(int soundID) {
        return m_Sounds[soundID]->GetVolume();
    }

    void AudioManager::SetSoundLooping(int soundID, bool loop) {
        m_Sounds[soundID]->SetLooping(loop);
    }

    bool AudioManager::IsSoundLooping(int soundID) {
        return m_Sounds[soundID]->IsLooping();
    }

    void AudioManager::OnStart() {
        m_FreeSoundIndexes.push(0);

        // Initialize the native audio engine
        m_NativeAudioEngine = malloc(sizeof(ma_engine));
        auto startupStatus = ma_engine_init(NULL, (ma_engine*)m_NativeAudioEngine);

        if (startupStatus != MA_SUCCESS) {
            Global::fileLogger.Error(std::format("Failed to initialize native audio engine. Error code: {}", (void*)startupStatus));
            free(m_NativeAudioEngine);
            delete this;
        }
    }

    void AudioManager::OnShutdown() {

        for (auto sound : m_Sounds) {
            delete sound;
        }
        m_Sounds.clear();

        // Clean up the native audio engine
        ma_engine_uninit((ma_engine*)m_NativeAudioEngine);
        free(m_NativeAudioEngine);

        delete this;
    }
}