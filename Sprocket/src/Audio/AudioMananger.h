#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "Sound.h"

#include "Events/Event.h"

#include "Core/Sprocket.pch"

namespace Sprocket {

    class AudioManager {
    private:
        // Singleton Components
        static AudioManager* s_Instance;
        AudioManager() {}
        AudioManager(const AudioManager&) = delete;
        AudioManager operator=(const AudioManager&) = delete;

        void* m_NativeAudioEngine = nullptr;

        std::vector<Sound*> m_Sounds;
        std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_FreeSoundIndexes;

        /// @brief Loads a sound from a file and returns its ID.
        static int LoadSound(const std::string& filepath);
        /// @brief Unloads a sound from memory.
        static void UnloadSound(int soundID);
        /// @brief Plays a sound.
        static void Play(int soundID);
        /// @brief Stops a sound.
        static void Stop(int soundID);
        /// @brief Resets a sound to the beginning.
        static void Reset(int soundID);
        /// @brief Checks if a sound is currently playing.
        static bool IsSoundPlaying(int soundID);
        /// @brief Sets the volume of a sound. Volume is a float between 0.0 and 1.0.
        static void SetSoundVolume(int soundID, float volume);
        /// @brief Gets the volume of a sound. Volume is a float between 0.0 and 1.0.
        static float GetSoundVolume(int soundID);
        /// @brief Sets whether a sound should loop or not.
        static void SetSoundLooping(int soundID, bool loop);
        /// @brief Checks if a sound is set to loop.
        static bool IsSoundLooping(int soundID);

        static void OnShutdown();

    public:
        /// @brief Initializes the singleton and sets the appropriate parameters. This must be called
        /// before any other AudioManager function.
        static void Init();
        /// @brief Handles incoming events. Should be registered as a callback to recieve events.
        /// @param event The event the should be handled.
        static void OnEvent(Event& event);
    };

}

#endif