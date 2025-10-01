#ifndef MENUSCENE_HPP
#define MENUSCENE_HPP

#include "Sprocket.h"

#include "Title.hpp"
#include "EnterToStart.hpp"

namespace Sprocket {

    class MenuScene : public Scene {

    public:
        MenuScene() {
      
            SubmitEntityToScene(std::make_shared<Title>());

            SubmitEntityToScene(std::make_shared<EnterToStart>());

        }

    };

}

#endif