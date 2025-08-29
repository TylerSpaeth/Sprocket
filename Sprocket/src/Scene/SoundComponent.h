#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H

#include "Component.h"
#include "Core/Sprocket.pch"
#include "Core/Macros.h"
#include "Events/Event.h"

namespace Sprocket {
    class SPROCKET_API SoundComponent : public Component { 
        friend class Entity;
    private:
        SoundComponent() {}
        ~SoundComponent() {}

        std::function<void(Event&)> m_EventCallback;
        int m_SoundID = -1;
        std::string m_Filepath;
    public:
        void SetFilepath(const std::string& filepath); // Can only be set once
        const std::string& GetFilepath() const;
        void Play();
        void Stop();
        void Reset();
        bool IsPlaying() const;
        void SetVolume(float volume);
        float GetVolume() const;
        void SetLooping(bool loop);
        bool IsLooping() const;
    };
}

#endif