#include "Window.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/WindowEvent.h"

#include "ThirdParty/glad/glad.h"
#include "ThirdParty/GLFW/glfw3.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////NONCLASS DEFINITIONS/////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    static GLFWwindow* InitGLFWwindow(const unsigned int xDimension, const unsigned int yDimension, const char* windowTitle);
    static void RegisterCallbacks(GLFWwindow* window);
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
    static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void WindowCloseCallback(GLFWwindow* window);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void Window::EnableVSync(bool enable) {
        if (enable) {
            glfwSwapInterval(1);
        }
        else {
            glfwSwapInterval(0);
        }
    }

    void Window::EnableCursor() {
        glfwSetInputMode((GLFWwindow*)s_Instance->m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Window::DisableCursor() {
        glfwSetInputMode((GLFWwindow*)s_Instance->m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Window::HideCursor() {
        glfwSetInputMode((GLFWwindow*)s_Instance->m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    void Window::LockAspectRatio() {
        glfwSetWindowAspectRatio((GLFWwindow*)s_Instance->m_Window, s_Instance->m_XDimension, s_Instance->m_YDimension);
    }

    void Window::UnlockAspectRatio() {
        glfwSetWindowAspectRatio((GLFWwindow*)s_Instance->m_Window, GLFW_DONT_CARE, GLFW_DONT_CARE);
    }

    const glm::vec2 Window::GetWindowDimensions() {
        return { s_Instance->m_XDimension, s_Instance->m_YDimension };
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void Window::Init(const unsigned int xDimension, const unsigned int yDimension, const std::string& windowTitle) {
        if (!s_Instance) {
            s_Instance = new Window();
            s_Instance->m_XDimension = xDimension;
            s_Instance->m_YDimension = yDimension;
            s_Instance->m_Window = InitGLFWwindow(xDimension, yDimension, windowTitle.c_str());
            glfwSetWindowUserPointer((GLFWwindow*)s_Instance->m_Window, (void*)&s_Instance->m_EventCallback);
            RegisterCallbacks((GLFWwindow*)s_Instance->m_Window);

            // Initialize GLAD
            // MUST BE DONE BEFORE ANY OPENGL CALLS INCLUDING INTIALIZING RENDERER
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        }
    }

    void Window::OnEvent(Event& event) {
        EventType type = event.GetEventType();
        switch (type) {
        case EventType::APP_UPDATE:
            OnUpdate();
            break;
        case EventType::APP_SHUTDOWN:
            OnShutdown();
            break;
        }
    }

    void Window::OnShutdown() {
        glfwTerminate();
    }

    void Window::OnUpdate() {
        glfwSwapBuffers((GLFWwindow*)s_Instance->m_Window);
        glfwPollEvents();
    }

    void Window::RegisterEventCallback(const std::function<void(Event&)> eventCallback) {
        s_Instance->m_EventCallback = eventCallback;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////NONCLASS///////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    static GLFWwindow* InitGLFWwindow(const unsigned int xDimension, const unsigned int yDimension, const char* windowTitle) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(xDimension, yDimension, windowTitle, NULL, NULL);
        if (window == NULL) {
            glfwTerminate();
        }
        // Make the window we created the current context on the current thread
        glfwMakeContextCurrent(window);

        return window;
    }

    static void RegisterCallbacks(GLFWwindow* window) {
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetCursorPosCallback(window, CursorPositionCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetScrollCallback(window, ScrollCallback);
        glfwSetWindowCloseCallback(window, WindowCloseCallback);
    }

    // Set the size of the OpenGL viewport when the window is resized
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);

        const std::function<void(Event&)> eventCallback = *(const std::function<void(Event&)>*) glfwGetWindowUserPointer(window);

        WindowResizedEvent* e  = new WindowResizedEvent(width, height);
        eventCallback(*e);
        delete e;
    }

    // This updates the key states in the input object that is stored in the glfw user pointer
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod) {
        const std::function<void(Event&)> eventCallback = *(const std::function<void(Event&)>*) glfwGetWindowUserPointer(window);
        switch (action) {
        case GLFW_REPEAT:
        case GLFW_PRESS: {
            KeyPressedEvent e((Keycode)key);
            eventCallback(e);
            break;
        }
        case GLFW_RELEASE: {
            KeyReleasedEvent e((Keycode)key);
            eventCallback(e);
            break;
        }
        }
    }

    static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
        const std::function<void(Event&)> eventCallback = *(const std::function<void(Event&)>*) glfwGetWindowUserPointer(window);
        MouseMovedEvent e(xpos, ypos);
        eventCallback(e);
    }

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        const std::function<void(Event&)> eventCallback = *(const std::function<void(Event&)>*) glfwGetWindowUserPointer(window);
        switch (action) {
        case GLFW_PRESS: {
            MouseButtonPressedEvent e((MouseButton)button);
            eventCallback(e);
            break;
        }
        case GLFW_RELEASE: {
            MouseButtonReleasedEvent e((MouseButton)button);
            eventCallback(e);
            break;
        }
        }
    }

    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        const std::function<void(Event&)> eventCallback = *(const std::function<void(Event&)>*) glfwGetWindowUserPointer(window);
        MouseScrolledEvent e(xoffset, yoffset);
        eventCallback(e);
    }

    static void WindowCloseCallback(GLFWwindow* window) {
        const std::function<void(Event&)> eventCallback = *(const std::function<void(Event&)>*) glfwGetWindowUserPointer(window);
        ApplicationShutdownEvent e;
        eventCallback(e);
    }

}