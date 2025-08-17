#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "Application.h"

extern Sprocket::Application* Sprocket::CreateApplication();

int main(int argc, char** argv) {

    auto app = Sprocket::CreateApplication();
    app->Run();
    delete app;

}


#endif