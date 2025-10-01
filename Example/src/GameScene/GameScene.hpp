#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "Sprocket.h"

#include "Entities/Camera.hpp"
#include "Player.hpp"
#include "Tiles.hpp"
#include "TestAnimation.hpp"

namespace Sprocket {

    class GameScene : public Scene {

    private: 
        std::shared_ptr<Entity> player;
        std::shared_ptr<Entity> camera;

    public:

        GameScene() {
            player = SubmitEntityToScene<Player>();
            camera = SubmitEntityToScene<Camera>();
            ((Camera*)(camera.get()))->m_EntityToFollow = player;
            SubmitEntityToScene<Tiles>();
            SubmitEntityToScene<TestAnimation>();

        }

    };

}

#endif