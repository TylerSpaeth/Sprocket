#ifndef QUADRENDERER_H
#define QUADRENDERER_H

#include "Events/Event.h"
#include "Events/RenderEvent.h"
#include "ECS/Component.h"

#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"

#include <functional>

namespace Sprocket {
  
  class QuadRenderer {
    friend class SceneManager;
    private:
      // Singleton Components
      static QuadRenderer* s_Instance;
      QuadRenderer(){}
      QuadRenderer(const QuadRenderer&) = delete;
      QuadRenderer operator=(const QuadRenderer*) = delete;

      std::function<void(Event&)> m_EventCallback;

      static void Init() {
        if(!s_Instance) s_Instance = new QuadRenderer();
      }
    public:

      static void RenderNewQuad(TransformComponent transform, QuadRendererComponent& renderer);

      static void SetModelMatrix(TransformComponent transform, QuadRendererComponent& renderer);

      static void UpdateQuad(TransformComponent transform, QuadRendererComponent& renderer);
  };

}

#endif