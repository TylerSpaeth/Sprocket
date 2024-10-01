#include "Sprocket.h"

int main() {

  /////////////////////////////////////////////////////////
  /////////////////// INITITIALIZATION ////////////////////
  /////////////////////////////////////////////////////////

  const unsigned int xDimension = 1066;
  const unsigned int yDimension = 600;

  const unsigned int maxQuads = 100000;

  Window::SetWindowAttributes(xDimension, yDimension, "New Window");
  Window* window = &Window::GetInstance();
  Input* input = window->GetInput();

  window->InitializeRenderer(maxQuads);
  Renderer renderer = *window->GetRenderer();
 
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
  while(!window->ShouldClose()) {

    // In microseconds
    double frameDelay = window->GetTimeSinceLastChecked();

    // Print fps
    //std::cout << 1000000 / frameDelay << "FPS\n";

    if(input->IsKeyPressed(KEY_ESCAPE)) {
      window->SetShouldClose();
    }
    
    if(input->IsKeyRepeated(KEY_W) || input->IsKeyPressed(KEY_W)) {
      modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 100.0f*frameDelay/1000000.0f, 0)) * modelMat; 
      renderer.SetQuadModelMatrix(index3, modelMat);
    }
    if(input->IsKeyRepeated(KEY_S) || input->IsKeyPressed(KEY_S)) {
      modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, -100.0f*frameDelay/1000000.0f, 0)) * modelMat; 
      renderer.SetQuadModelMatrix(index3, modelMat);
    }
    if(input->IsKeyRepeated(KEY_A) || input->IsKeyPressed(KEY_A)) {
      modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f*frameDelay/1000000.0f, 0, 0)) * modelMat; 
      renderer.SetQuadModelMatrix(index3, modelMat);
    }
    if(input->IsKeyRepeated(KEY_D) || input->IsKeyPressed(KEY_D)) {
      modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f*frameDelay/1000000.0f, 0, 0)) * modelMat; 
      renderer.SetQuadModelMatrix(index3, modelMat);
    }
    
    renderer.Clear();
    
    renderer.Draw();

    // This clears all the keypress data from this frame so there is no
    // residual in the next one
    window->ClearInputs();
    window->SwapBuffers();
    window->PollEvents();
  }

  // TODO make sure everything is getting cleaned up

  window->Close();
  return 0;

}
