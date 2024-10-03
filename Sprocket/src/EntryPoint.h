#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "Application.h"
#include <iostream>

extern Sprocket::Application* Sprocket::CreateApplication();

int main(int argc, char** argv) {

  std::cout << "Sprocket Startup\n";
  auto app = Sprocket::CreateApplication();
  app->Run();
  delete app;

}


#endif