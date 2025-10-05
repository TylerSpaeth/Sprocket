#ifndef GAMEOVERSOUNDEFFECT_HPP
#define GAMEOVERSOUNDEFFECT_HPP

#include "Sprocket.h"

namespace Sprocket {
    class GameOverSoundEffect : public Entity {

    public:
        GameOverSoundEffect() {
            AddComponent<SoundComponent>();
        }

        void Start() override {
            GetComponent<SoundComponent>()->SetFilepath("sounds/game-over-arcade-6435.mp3");
            GetComponent<SoundComponent>()->Play();
        }

        void Update(float deltaTime) override {
            if (!GetComponent<SoundComponent>()->IsPlaying()) {
                SceneManager::GetActiveScene()->RemoveEntityFromScene(m_Self);
            }
        }

    };
}

#endif