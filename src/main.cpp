#include <iostream>

#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"
#include "Window.hpp"
#include "GLValidate.hpp"
#include "Input.h"

#include "ThirdParty/glad/glad.h"
#include "ThirdParty/GLFW/glfw3.h"
#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"

// This vector stores the keys that have been pressed and need to be cleaned between frames
std::vector<int> clearKeys;
std::vector<int> pressedKeys;

// This updates the key states in the input object that is stored in the glfw user pointer
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod) {
  Input* input = (Input*)glfwGetWindowUserPointer(window);
  input->UpdateKeyState(key, action);
  if(action == GLFW_RELEASE) {
    clearKeys.push_back(key);
  }
  else if(action == GLFW_PRESS) {
    pressedKeys.push_back(key);
  }
}

// This clears all key for the keys that are stored in the keys vector
static void ClearKeys(GLFWwindow* window) {
  Input* input = (Input*)glfwGetWindowUserPointer(window);
  for(int i : clearKeys) {
    input->UpdateKeyState(i, -1);
  }
  clearKeys.clear();
  for(int i : pressedKeys) {
    input->UpdateKeyState(i, GLFW_REPEAT);
  }
  pressedKeys.clear();
}

static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
  Input* input = (Input*)glfwGetWindowUserPointer(window);
  input->UpdateCursorPosition(xpos, ypos);
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  Input* input = (Input*)glfwGetWindowUserPointer(window);
  input->UpdateMouseButtonState(button, action);
}

static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  Input* input = (Input*)glfwGetWindowUserPointer(window);
  input->UpdateMouseScrollOffset(xoffset, yoffset);
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
  // Create an input object and store it in the glfw user pointer so that it can
  // be accessed in the callback
  Input input(window);
  glfwSetWindowUserPointer(window, &input);
  glfwSetKeyCallback(window, KeyCallback);
  glfwSetCursorPosCallback(window, CursorPositionCallback);
  glfwSetMouseButtonCallback(window, MouseButtonCallback);
  glfwSetScrollCallback(window, ScrollCallback);

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
  /*for(int i = 0; i < 100000; i++) {
    unsigned int index = renderer.AddQuad(100,0);
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-300, -300+i, 0));
    renderer.SetQuadModelMatrix(index, modelMatrix);
  }*/
  // LOOKHERE

  /////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////


  glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
  /////////////////////////////////////////////////////////
  ////////////////////// RENDER LOOP //////////////////////
  /////////////////////////////////////////////////////////
  while(!glfwWindowShouldClose(window)) {

    // In microseconds
    double frameDelay = renderer.GetTimeSinceLastChecked();

    // Print fps
    //std::cout << 1000000 / frameDelay << "FPS\n";

    if(input.IsKeyPressed(GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window,true);
    }

    if(input.IsKeyRepeated(KEY_W) || input.IsKeyPressed(KEY_W)) {
      modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 100.0f*frameDelay/1000000.0f, 0)) * modelMat; 
      renderer.SetQuadModelMatrix(index3, modelMat);
    }
    if(input.IsKeyRepeated(KEY_S) || input.IsKeyPressed(KEY_S)) {
      modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, -100.0f*frameDelay/1000000.0f, 0)) * modelMat; 
      renderer.SetQuadModelMatrix(index3, modelMat);
    }
    if(input.IsKeyRepeated(KEY_A) || input.IsKeyPressed(KEY_A)) {
      modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f*frameDelay/1000000.0f, 0, 0)) * modelMat; 
      renderer.SetQuadModelMatrix(index3, modelMat);
    }
    if(input.IsKeyRepeated(KEY_D) || input.IsKeyPressed(KEY_D)) {
      modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f*frameDelay/1000000.0f, 0, 0)) * modelMat; 
      renderer.SetQuadModelMatrix(index3, modelMat);
    }

    renderer.Clear();
    
    renderer.Draw();

    // This clears all the keypress data from this frame so there is no
    // residual in the next one
    ClearKeys(window);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clear out used resources
  glfwTerminate();	
  return 0;

}


