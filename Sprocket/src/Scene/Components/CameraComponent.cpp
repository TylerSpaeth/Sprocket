#include "CameraComponent.h"

#include "Events/RenderEvent.h"

#include "Utils/RendererUtils.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    CameraComponent::~CameraComponent() {
        if (m_EventCallback) {
            UpdateCameraTransform(glm::vec3(0), glm::vec3(0), glm::vec3(1));
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void CameraComponent::UpdateCameraTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

        RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::VIEW_MATRIX, -1);

        e->m_Matrix = RendererUtils::CalculateMatrixFromVecs(position, rotation, scale);
        m_EventCallback(*e);
        delete e;

    }

    void CameraComponent::OnActivate(OnActivateParams& onActivateParams) {
        RegisterEventCallback(onActivateParams.eventCallback);
        UpdateCameraTransform(onActivateParams.position, onActivateParams.rotation, onActivateParams.scale);
    }

    void CameraComponent::OnDeactivate(OnDeactivateParams& onDeactivateParams) {
        UpdateCameraTransform(glm::vec3(0), glm::vec3(0), glm::vec3(1));
        m_EventCallback = nullptr;
    }

    void CameraComponent::OnUpdate(OnUpdateParams& onUpdateParams) {
        if (onUpdateParams.updatedTransform) {
            UpdateCameraTransform(onUpdateParams.position, onUpdateParams.rotation, onUpdateParams.scale);
        }
    }

}