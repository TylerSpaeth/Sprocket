#include <iostream>

#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"
#include "Window.hpp"
#include "GLValidate.hpp"

#include "ThirdParty/glad/glad.h"
#include "ThirdParty/GLFW/glfw3.h"
#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"

// Processes user input
void ProcessInput(GLFWwindow* window) {
  // Binds the escape key to closing the window if pressed
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window,true);
  }
}

int main() {

  /////////////////////////////////////////////////////////
  /////////////////// INITITIALIZATION ////////////////////
  /////////////////////////////////////////////////////////

  const unsigned int xDimension = 1066;
  const unsigned int yDimension = 600;

  const unsigned int maxQuads = 100000;
  const unsigned int maxVertex = 4 * maxQuads;
  const unsigned int maxIndex = 6 * maxQuads;

  GLFWwindow* window = InitGLFWwindow(xDimension, yDimension);

  // Initialize GLAD
  // MUST BE DONE BEFORE ANY OPENGL CALLS INCLUDING INTIALIZING RENDERER
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return -1;
  }

  Renderer renderer("../res/shaders/Basic.vert", "../res/shaders/Basic.frag", maxQuads);
 
  // Load Textures
  Texture texture("../res/textures/BiggerBetterTree.png", 1);
  Texture texture1("../res/textures/CowBoyAiming.png", 2);
  Texture texture2("../res/textures/Bandit-Idle96x96.png", 3);
  // Update the textures in the uniform of the renderer's shader
  renderer.UpdateTextureUniform(3);

  // Set projection matrix
  // (0,0) is the center of our orthographic projection matrix
  renderer.SetProjectionMatrix( glm::ortho(-(float)xDimension/2, (float)xDimension/2, -(float) yDimension/2, (float)yDimension/2));

  
  // LOOKHERE test code for rendering
  // Set model matricies
  glm::mat4 modelMatrix0 = glm::translate(glm::mat4(1.0f), glm::vec3(100, 200, 0));
  glm::mat4 modelMatrix1 = glm::translate(glm::mat4(1.0f), glm::vec3(200, 100, 0));
  glm::mat4 modelMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
  glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -1.57f, glm::vec3(0,0,1));

  unsigned int index1 = renderer.AddQuad(100,1);
  unsigned int index2 = renderer.AddQuad(100,3);
  unsigned int index3 = renderer.AddQuad(100,2);
  renderer.SetQuadModelMatrix(index1, modelMatrix0*scale);
  renderer.SetQuadModelMatrix(index2, modelMatrix1);
  renderer.SetQuadTextureCoords(index2, glm::vec4(0.25f, 0.25f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  renderer.SetQuadColor(index3, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
  renderer.SetQuadTextureID(index3, 0);

  // Stress Test
  for(int i = 0; i < 100000; i++) {
    unsigned int index = renderer.AddQuad(100,0);
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-300, -300+i, 0));
    renderer.SetQuadModelMatrix(index, modelMatrix);
  }
  // LOOKHERE

  /////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////
  ////////////////////// RENDER LOOP //////////////////////
  /////////////////////////////////////////////////////////
  while(!glfwWindowShouldClose(window)) {

    // Print fps
    std::cout << 1000000 / renderer.GetTimeSinceLastChecked() << "FPS\n";

    ProcessInput(window);

    renderer.Clear();
    
    renderer.Draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clear out used resources
  glfwTerminate();	
  return 0;

}


