#ifndef MENUSCENE_HPP
#define MENUSCENE_HPP

#include "Sprocket.h"

#include "Title.hpp"
#include "EnterToStart.hpp"

namespace Sprocket {

    class MenuScene : public Scene {

    public:
        MenuScene() {
      
            SubmitEntityToScene<Title>();
            SubmitEntityToScene<EnterToStart>();
        }

    };

}

#endif