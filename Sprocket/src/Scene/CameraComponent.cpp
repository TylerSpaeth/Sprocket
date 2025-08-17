#include "CameraComponent.h"

#include "Events/RenderEvent.h"

#include "Utils/RendererUtils.hpp"

#include "ThirdParty/glm/gtc/matrix_transform.hpp"

namespace Sprocket {

    void CameraComponent::UpdateCameraPosition(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

        RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::VIEW_MATRIX, -1);

        e->m_Matrix = RendererUtils::CalculateMatrixFromVecs(position, rotation, scale);
        m_EventCallback(*e);
        free(e);

    }

}