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

        static int LoadSound(const std::string& filepath);
        static void UnloadSound(int soundID);
        static void Play(int soundID);
        static void Stop(int soundID);
        static void Reset(int soundID);
        static bool IsSoundPlaying(int soundID);
        static void SetSoundVolume(int soundID, float volume);
        static float GetSoundVolume(int soundID);
        static void SetSoundLooping(int soundID, bool loop);
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