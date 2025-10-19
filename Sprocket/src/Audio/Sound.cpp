#include "Sound.h"

#include "Core/Global.h"

#define MINIAUDIO_IMPLEMENTATION
#include "ThirdParty/miniaudio/miniaudio.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    Sound::Sound(void* nativeEngine, const std::string& filepath) : m_NativeEngine(nativeEngine) {
        if(!m_NativeEngine) {
            Global::FileLogger().Error("Native audio engine is null. Cannot create sound.");
            throw std::runtime_error("Native audio engine is null. Cannot create sound.");
        }
        
        m_NativeSound = malloc(sizeof(ma_sound));
        auto result = ma_sound_init_from_file((ma_engine*)m_NativeEngine, filepath.c_str(), MA_SOUND_FLAG_DECODE, NULL, NULL, (ma_sound*)m_NativeSound);
        if(result != MA_SUCCESS) {
            Global::FileLogger().Error(std::format("Failed to load sound from file: {}. Error code: {}", filepath, (void*)result));
            free(m_NativeSound);
            m_NativeSound = nullptr;
            throw std::runtime_error("Failed to load sound from file: " + filepath);
        }
    }

    Sound::~Sound() {
        ma_sound_uninit((ma_sound*)m_NativeSound);
        free(m_NativeSound);
    }

    void Sound::Play() {
        ma_sound_start((ma_sound*)m_NativeSound);
    }

    bool Sound::IsPlaying() const {
        return ma_sound_is_playing((ma_sound*)m_NativeSound);
    }

    void Sound::Stop() {
        ma_sound_stop((ma_sound*)m_NativeSound);
    }

    void Sound::Reset() {
        ma_sound_seek_to_pcm_frame((ma_sound*)m_NativeSound, 0);
    }

    void Sound::SetVolume(float volume) {
        // Clamp volume between 0.0 and 1.0
        if (volume < 0.0f) volume = 0.0f;
        if (volume > 1.0f) volume = 1.0f;
        ma_sound_set_volume((ma_sound*)m_NativeSound, volume);
    }

    float Sound::GetVolume() const {
        return ma_sound_get_volume((ma_sound*)m_NativeSound);
    }

    void Sound::SetLooping(bool loop) {
        ma_sound_set_looping((ma_sound*)m_NativeSound, loop);
    }

    bool Sound::IsLooping() const {
        return ma_sound_is_looping((ma_sound*)m_NativeSound);
    }

}