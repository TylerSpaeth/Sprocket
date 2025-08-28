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
        Sound(void* nativeEngine, const std::string& filepath);
        ~Sound();
        void Play();
        bool IsPlaying() const;
        void Stop();

        void SetVolume(float volume); // volume should be between 0.0 and 1.0
        float GetVolume() const;

        void SetStartTime(float time); // time in seconds
        float GetStartTime() const; // time in seconds

        void SetStopTime(float time); // time in seconds
        float GetStopTime() const; // time in seconds

        void SetLooping(bool loop);
        bool IsLooping() const;
    };

}

#endif