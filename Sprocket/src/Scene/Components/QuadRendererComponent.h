#ifndef QUADRENDERERCOMPONENT_H
#define QUADRENDERERCOMPONENT_H

#include "Component.h"
#include "Subcomponents/Sprite.h"

#include "ThirdParty/glm/glm.hpp"

#include "Core/Sprocket.pch"

namespace Sprocket {

    /// @brief QuadRendererComponent are all fixed size squares. To modify the shape an size, utilize
    /// the TransformComponent of the Entity this is attached to.
    class SPROCKET_API QuadRendererComponent : public EventDrivenComponent {

        friend class Entity;
        friend class AnimationComponent;

    private:

        // This correspondes to the id in the rendering system. It is needed to do anything with 
        // the component in the rendering system.
        int m_QuadID = -1;

        glm::vec4 m_QuadColor = { 1,1,1,1 };
        Sprite m_Sprite;

    public:

        // NOTE currently calling either of these updates will cause the renderer to switch between 
        // color and texture. It should be determined whether this is correct behavior.

        glm::vec4 GetQuadColor() const;

        /// @brief Updates the Quad color to the given RGBA color. If the quad is currently 
        /// displaying a texture, that will be replaced with this color.
        /// @param newColor - The RGBA color to use.
        void SetQuadColor(glm::vec4 newColor);

        Sprite GetSprite() const;

        void SetSprite(const Sprite& sprite);

    private:

        QuadRendererComponent();
        ~QuadRendererComponent();

        /// @brief Submits this quad renderer into the rendering system
        void RenderNew();

        /// @brief Updates the model matrix of this quad renderer based on the given transform
        /// @param position 
        /// @param rotation 
        /// @param scale 
        void UpdateModelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        /// @brief Pulls this quad renderer out of the rendering system.
        void RemoveRender();

        /// @brief Helper function to send texture related events.
        void SendTextureEvent();

        void RegisterEventCallback(const std::function<void(Event&)> eventCallback) override;

    };

}

#endif