#ifndef SOUND_H
#define SOUND_H

#include "Core/Sprocket.pch"

namespace Sprocket {

    class Sound {
    
    private:
        void* m_NativeSound = nullptr;
        void* m_NativeEngine = nullptr;
        float m_StartTime = 0.0f;
        float m_StopTime = std::numeric_limits<float>::max();
    public:
        /// @Brief Creates a sound object from a file path.
        /// @Param nativeEngine A pointer to the native audio engine
        /// @Param filepath The file path to the sound file
        Sound(void* nativeEngine, const std::string& filepath);
        ~Sound();
        /// @Brief Plays the sound from the beginning or from the last stopped position.
        void Play();
        /// @Brief Checks if the sound is currently playing.
        bool IsPlaying() const;
        /// @Brief Stops the sound.
        void Stop();
        /// @Brief Resets the sound to the beginning.
        void Reset();

        /// @Brief Sets the volume of the sound.
        /// @Param volume The volume level (0.0 to 1.0). Will be clamped if out of range.
        void SetVolume(float volume); // volume should be between 0.0 and 1.0
        /// @Brief Gets the current volume of the sound.
        float GetVolume() const;

        /// @Brief Sets the pitch of the sound.
        void SetLooping(bool loop);
        /// @Brief Checks if the sound is set to loop.
        bool IsLooping() const;
    };

}

#endif