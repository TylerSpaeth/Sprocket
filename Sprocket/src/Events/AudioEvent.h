#ifndef AUDIOEVENT_H
#define AUDIOEVENT_H

namespace Sprocket {

    class AudioEvent : public Event {
        friend class AudioManager;
    private:
        int m_SoundID;
    public:
        AudioEvent(EventType type, int soundID) : m_SoundID(soundID) {
            this->SetType(type);
        }
        int GetSoundID() const { return m_SoundID; }
    };

    class AudioNewEvent : public AudioEvent {
        friend class AudioManager;
    private:
        std::string m_Filepath;
    public:
        AudioNewEvent(const std::string& filepath) : AudioEvent(EventType::AUDIO_NEW, -1), m_Filepath(filepath) {}

        const std::string& GetFilepath() const { return m_Filepath; }
    };

    class AudioDeleteEvent : public AudioEvent {
        friend class AudioManager;
    public:
        AudioDeleteEvent(int soundID) : AudioEvent(EventType::AUDIO_DELETE, soundID) {}
    };

    enum class AudioSetterType {
        VOLUME,
        LOOPING,
    };

    class AudioSetterEvent : public AudioEvent {
        friend class AudioManager;
    private:
        AudioSetterType m_SetterType;
        float m_FloatValue = 0.0f;
        bool m_BoolValue = false;
    public:
        AudioSetterEvent(int soundID, AudioSetterType setterType, float value) 
            : AudioEvent(EventType::AUDIO_SETTER, soundID), m_SetterType(setterType), m_FloatValue(value) {}
        AudioSetterEvent(int soundID, AudioSetterType setterType, bool value) 
            : AudioEvent(EventType::AUDIO_SETTER, soundID), m_SetterType(setterType), m_BoolValue(value) {}

        AudioSetterType GetSetterType() const { return m_SetterType; }
        float GetFloatValue() const { return m_FloatValue; }
        bool GetBoolValue() const { return m_BoolValue; }
    };

    enum class AudioGetterType {
        VOLUME,
        LOOPING,
        IS_PLAYING
    };

    class AudioGetterEvent : public AudioEvent {
        friend class AudioManager;
    private:
        AudioGetterType m_GetterType;
        float m_FloatValue = 0.0f;
        bool m_BoolValue = false;
    public:
        AudioGetterEvent(int soundID, AudioGetterType getterType) 
            : AudioEvent(EventType::AUDIO_GETTER, soundID), m_GetterType(getterType) {}

        AudioGetterType GetGetterType() const { return m_GetterType; }
        float GetFloatValue() const { return m_FloatValue; }
        bool GetBoolValue() const { return m_BoolValue; }
    };

    enum class AudioActionType {
        PLAY,
        STOP,
        RESET
    };

    class AudioActionEvent : public AudioEvent {
        friend class AudioManager;
    private:
        AudioActionType m_ActionType;
    public:
        AudioActionEvent(int soundID, AudioActionType actionType) 
            : AudioEvent(EventType::AUDIO_ACTION, soundID), m_ActionType(actionType) {}

        AudioActionType GetActionType() const { return m_ActionType; }
    };
}

#endif