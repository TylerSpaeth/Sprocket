#include "CameraComponent.h"

#include "Core/Global.h"
#include "Core/Input.h"
#include "Core/Window.h"

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

    glm::vec2 CameraComponent::GetMousePositionInWorldSpace() {
        float mouseX = Input::GetMouseXPosition();
        float mouseY = Input::GetMouseYPosition();

        glm::vec2 windowDimensions = Window::GetWindowDimensions();

        float mouseXFromCenter = mouseX - (windowDimensions.x / 2.0f);
        float mouseYFromCenter = (windowDimensions.y / 2.0f) - mouseY;

        return { mouseXFromCenter / Global::PixelsPerUnit() + m_Position.x, mouseYFromCenter / Global::PixelsPerUnit() + m_Position.y};
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void CameraComponent::UpdateCameraTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

        m_Position = position;

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