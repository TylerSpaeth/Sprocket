#ifndef MENUSCENE_HPP
#define MENUSCENE_HPP

#include "Sprocket.h"

#include "Title.hpp"
#include "EnterToStart.hpp"

namespace Sprocket {

    class MenuScene : public Scene {

    private:
        Entity* title;
        Entity* enterToStart;

    public:
        MenuScene() {
      
            title = new Title();
            SubmitEntityToScene(*title);

            enterToStart = new EnterToStart();
            SubmitEntityToScene(*enterToStart);

        }

    };

}

#endif