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

    friend class Scene;
    
    private:

      QuadRenderer(){}

      std::function<void(Event&)> m_EventCallback;

      void VerifyCallback();

    public:

      void RenderNewQuad(TransformComponent transform, QuadRendererComponent& renderer);

      void SetModelMatrix(TransformComponent transform, QuadRendererComponent& renderer);

      void UpdateQuad(QuadRendererComponent& renderer);

      void DeleteQuad(const unsigned int quadID);
  };

}

#endif