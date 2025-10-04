#ifndef TEXTRENDERERCOMPONENT_H
#define TEXTRENDERERCOMPONENT_H

#include "Component.h"

#include "Core/Macros.h"

#include "Events/Event.h"

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket {

    class SPROCKET_API TextRendererComponent : public EventDrivenComponent {
        friend class Entity;
    private:

        // This correspondes to the id in the rendering system. It is needed to do anything with 
        // the component in the rendering system.
        int m_QuadID = -1;

        std::string m_FontPath;
        std::string m_Text;

        glm::vec4 m_Color = {1,1,1,1};
    
    public:

        TextRendererComponent();
        ~TextRendererComponent();

        /// @brief This sets the data on the for this TextRenderer. It may only be set once.
        /// @param fontPath The path to the font file.
        /// @param m_Text The text to be displayed
        /// @return True if setting data is successful, false otherwise.
        const bool SetData(const std::string& fontPath, const std::string& m_Text);
        const std::string GetFontPath() const;
        const std::string GetText() const;

        void SetColor(const glm::vec4 color);
        const glm::vec4 GetColor() const;

    private:

        void RegisterEventCallback(const std::function<void(Event&)> eventCallback) override;

        /// @brief Submits this into the rendering system
        void RenderNew();

        /// @brief Updates the model matrix of this based on the given transform
        /// @param position 
        /// @param rotation 
        /// @param scale 
        void UpdateModelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        /// @brief Pulls this out of the rendering system.
        void RemoveRender();

    };

}

#endif