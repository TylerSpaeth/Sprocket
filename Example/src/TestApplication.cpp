#include "Sprocket.h"
#include "Camera.hpp"
#include "Player.hpp"
#include "Tiles.hpp"
#include <functional>
#include <random>

class TestApplication : public Sprocket::Application {

    Sprocket::Entity* player;
    Sprocket::Entity* camera;
    Sprocket::Entity* tileMap;
    Sprocket::Logger logger;

public:
    TestApplication() : Sprocket::Application() {}
    ~TestApplication() {}
    void Start() {
        using namespace Sprocket;
        Scene* scene = SceneManager::GetActiveScene();

        player = new Player();
        scene->SubmitEntityToScene(*player);

        camera = new Camera();
        ((Camera*)camera)->m_EntityToFollow = player;
        scene->SubmitEntityToScene(*camera);

        tileMap = new Tiles();
        scene->SubmitEntityToScene(*tileMap);

    }

    void Update(float deltaTime) {
        using namespace Sprocket;

        // Print frame time and fps
        logger.Debug(std::format("Frame time: {:.3f} ms, FPS: {}", deltaTime * 1000, (int)(1000000 / (deltaTime * 1000000))));

        // Close on escape
        if (Input::IsKeyPressed(KEY_ESCAPE)) {
            WindowCloseEvent* wc = new WindowCloseEvent();
            OnEvent(*wc);
        }

    }
};

Sprocket::Application* Sprocket::CreateApplication() {

    TestApplication* app = new TestApplication();

    Window::Init(1066, 600, "Test Window");
    Window::EnableVSync(false);
    Window::RegisterEventCallback(std::bind(&Application::OnEvent, app, std::placeholders::_1));
    app->RegisterEventCallback(Window::OnEvent, EventCategory::UNCATEGORIZED);

    Input::Init();
    app->RegisterEventCallback(Input::OnEvent, EventCategory::APPLICATION);

    Renderer::Init(500000, 1066, 600);
    app->RegisterEventCallback(Renderer::OnEvent, EventCategory::UNCATEGORIZED);

    SceneManager::Init(std::bind(&Application::OnEvent, app, std::placeholders::_1));
    app->RegisterEventCallback(SceneManager::OnEvent, EventCategory::UNCATEGORIZED);

    Physics::Init();
    app->RegisterEventCallback(Physics::OnEvent, EventCategory::UNCATEGORIZED);

    return app;
}