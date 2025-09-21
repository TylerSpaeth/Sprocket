#include "CameraComponent.h"

#include "Events/RenderEvent.h"

#include "Utils/RendererUtils.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    CameraComponent::~CameraComponent() {
        if (m_EventCallback) {
            UpdateCameraTransform(glm::vec3(0), glm::vec3(0), glm::vec3(1));
        }
    }

    void CameraComponent::UpdateCameraTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

        RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::VIEW_MATRIX, -1);

        e->m_Matrix = RendererUtils::CalculateMatrixFromVecs(position, rotation, scale);
        m_EventCallback(*e);
        delete e;

    }

}