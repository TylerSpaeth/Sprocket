#include "Sprocket.h"
#include "GameScene/GameScene.hpp"
#include "MenuScene/MenuScene.hpp"
#include <functional>

class TestApplication : public Sprocket::Application {

    Sprocket::Scene* gameScene;
    Sprocket::Scene* menuScene;

    Sprocket::Logger logger;

public:
    TestApplication() : Sprocket::Application() {}
    ~TestApplication() {}

    float displayedDeltaTime = 0.0f;
    std::vector<float> frameTimes;

    void Start() {
        using namespace Sprocket;

        Window::EnableVSync(false);

        menuScene = new MenuScene();
        SceneManager::AddScene(1, menuScene);
        gameScene = new GameScene();
        SceneManager::AddScene(2, gameScene);

        SceneManager::SetActiveScene(1);

        // Show ImGui demo window
        /*ImGuiImpl::SubmitRenderFunction([]() {
            ImGui::ShowDemoWindow();
        });*/
        ImGuiImpl::SubmitRenderFunction(std::bind(&TestApplication::SceneSelector, this));
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

    void SceneSelector() {
        auto scenes = Sprocket::SceneManager::GetAllSceneIndicies();
        std::vector<std::string> storage;
        storage.reserve(scenes.size());
        std::vector<const char*> scenesAsStrings;
        for (auto scene : scenes) {
            storage.push_back(std::to_string(scene));
            scenesAsStrings.push_back(storage.at(storage.size()-1).c_str());
        }
        int selectedIndex = Sprocket::SceneManager::GetActiveSceneIndex();
        ImGui::Begin("Scene Selector");
        if (ImGui::BeginCombo("Selected Scene", scenesAsStrings[selectedIndex])) {
            for (int i = 0; i < scenesAsStrings.size(); i++) {
                const bool isSelected = selectedIndex == i;
                if (ImGui::Selectable(scenesAsStrings[i], isSelected)) {
                    Sprocket::SceneManager::SetActiveScene(i);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::End();
    }
};

Sprocket::Application* Sprocket::CreateApplication() {

    TestApplication* app = new TestApplication();

    app->SetWindowTitle("Sprocket Test Application");

    return app;
}