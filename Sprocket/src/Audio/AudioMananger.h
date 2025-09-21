#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "Sound.h"

#include "Events/Event.h"

#include "Core/Sprocket.pch"

namespace Sprocket {

    /// @brief Class for managing the audio of an application
    class AudioManager {
    private:

        void* m_NativeAudioEngine = nullptr;

        std::vector<Sound*> m_Sounds;
        std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_FreeSoundIndexes;

    public:

        /// @brief Handles incoming events. Should be registered as a callback to recieve events.
        /// @param event The event the should be handled.
        void OnEvent(Event& event);

    private:

        /// @brief Loads a sound from a file and returns its ID.
        int LoadSound(const std::string& filepath);
        /// @brief Unloads a sound from memory.
        void UnloadSound(int soundID);
        /// @brief Plays a sound.
        void Play(int soundID);
        /// @brief Stops a sound.
        void Stop(int soundID);
        /// @brief Resets a sound to the beginning.
        void Reset(int soundID);
        /// @brief Checks if a sound is currently playing.
        bool IsSoundPlaying(int soundID);
        /// @brief Sets the volume of a sound. Volume is a float between 0.0 and 1.0.
        void SetSoundVolume(int soundID, float volume);
        /// @brief Gets the volume of a sound. Volume is a float between 0.0 and 1.0.
        float GetSoundVolume(int soundID);
        /// @brief Sets whether a sound should loop or not.
        void SetSoundLooping(int soundID, bool loop);
        /// @brief Checks if a sound is set to loop.
        bool IsSoundLooping(int soundID);
        
        /// @brief Logic that occurs on application startup
        void OnStart();
        /// @brief Logic that occurs on application shutdown
        void OnShutdown();

    };

}

#endif