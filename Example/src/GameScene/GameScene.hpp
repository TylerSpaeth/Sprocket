#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "Sprocket.h"

#include "Camera.hpp"
#include "Player.hpp"
#include "Tiles.hpp"
#include "TestAnimation.hpp"

namespace Sprocket {

    class GameScene : public Scene {

    private: 
        Entity* player;
        Entity* camera;
        Entity* tileMap;
        Entity* testAnimation;
        Entity* title;

    public:

        GameScene() {
            player = new Player();
            SubmitEntityToScene(*player);

            camera = new Camera();
            ((Camera*)camera)->m_EntityToFollow = player;
            SubmitEntityToScene(*camera);

            tileMap = new Tiles();
            SubmitEntityToScene(*tileMap);

            testAnimation = new TestAnimation();
            SubmitEntityToScene(*testAnimation);

        }

    };

}

#endif