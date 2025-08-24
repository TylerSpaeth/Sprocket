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

    float displayedDeltaTime = 0.0f;
    std::vector<float> frameTimes;

    void Start() {
        using namespace Sprocket;

        Window::EnableVSync(false);

        Scene* scene = SceneManager::GetActiveScene();

        player = new Player();
        scene->SubmitEntityToScene(*player);

        camera = new Camera();
        ((Camera*)camera)->m_EntityToFollow = player;
        scene->SubmitEntityToScene(*camera);

        tileMap = new Tiles();
        scene->SubmitEntityToScene(*tileMap);

        // Show ImGui demo window
        /*ImGuiImpl::SubmitRenderFunction([]() {
            ImGui::ShowDemoWindow();
        });*/
        ImGuiImpl::SubmitRenderFunction(std::bind(&TestApplication::FrameTimer, this));

    }

    void Update(float deltaTime) {
        using namespace Sprocket;

        // Calculate average frame time over last 100 frames
        this->frameTimes.push_back(deltaTime);
        if(this->frameTimes.size() > 100) {
            float total = 0.0f;
            for(auto ft : this->frameTimes) {
                total += ft;
            }
            this->frameTimes.clear();
            this->displayedDeltaTime = total / 100.0f;
        }

        // Print frame time and fps
        logger.Debug(std::format("Frame time: {:.3f} ms, FPS: {}", deltaTime * 1000, (int)(1000000 / (deltaTime * 1000000))));

        // Close on escape
        if (Input::IsKeyPressed(KEY_ESCAPE)) {
            ApplicationShutdownEvent e;
            OnEvent(e);
        }

    }

    void Shutdown() {}

    void FrameTimer() {
        ImGui::Begin("Performance");
        ImGui::Text(std::format("Frame time: {:.3f} ms, FPS: {}", displayedDeltaTime * 1000, (int)(1000000 / (displayedDeltaTime * 1000000))).c_str());
        ImGui::End();
    }
};

Sprocket::Application* Sprocket::CreateApplication() {

    TestApplication* app = new TestApplication();

    app->SetWindowTitle("Sprocket Test Application");

    return app;
}