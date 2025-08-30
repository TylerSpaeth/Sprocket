#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H

#include "Component.h"
#include "Core/Sprocket.pch"
#include "Core/Macros.h"

namespace Sprocket {
    class SPROCKET_API SoundComponent : public EventDrivenComponent { 
        friend class Entity;
    private:
        /// @brief Creates a sound component, but does not register it with the audio system.
        SoundComponent() {}
        ~SoundComponent() {}

        int m_SoundID = -1;
        std::string m_Filepath;
    public:
        /// @brief Creates a sound component and registers it with the audio system. This can
        /// only be called once. 
        void SetFilepath(const std::string& filepath);
        /// @brief Returns the filepath of the sound.
        const std::string& GetFilepath() const;
        /// @brief Plays the sound.
        void Play();
        /// @brief Stops the sound.
        void Stop();
        /// @brief Resets the sound to the beginning.
        void Reset();
        /// @brief Checks if the sound is currently playing.
        bool IsPlaying() const;
        /// @brief Sets the volume of the sound. Volume is a value between 0.0 and 1.0.
        void SetVolume(float volume);
        /// @brief Gets the volume of the sound. Volume is a value between 0.0 and 1.0.
        float GetVolume() const;
        /// @brief Sets whether the sound should loop when it reaches the end.
        void SetLooping(bool loop);
        /// @brief Gets whether the sound is set to loop when it reaches the end.
        bool IsLooping() const;
    };
}

#endif