#ifndef TESTANIMATION_HPP
#define TESTANIMATION_HPP

#include "Sprocket.h"

namespace Sprocket {

    class TestAnimation : public Entity {

    private:
        Animation circleTree;
        Animation cowboyIdle;
        bool flip = false;

    public:

        TestAnimation() {
            AddComponent<AnimationComponent>();
            AddComponent<BoxColliderComponent>();

            for (auto& sprite : GetSpritesFromSpritesheet("textures/Bandit-Idle96x96.png", 384, 96, 96, 96)) {
                AnimationFrame frame;
                frame.sprite = sprite;
                frame.frameDuration = .25;
                cowboyIdle.animationFrames.push_back(frame);
            }

            // TransformComponent are the only components that should be modified in the constructor.
            // Others can be, but it's not recommended.
            GetComponent<TransformComponent>()->LocalPosition().y += 5; 
        }

        void Start() override {

            Sprite tree;
            tree.texturePath = "textures/BiggerBetterTree.png";
            AnimationFrame frame1;
            frame1.frameDuration = .5;
            frame1.sprite = tree;

            Sprite circle;
            circle.color= { 1,0,0,1 };
            circle.texturePath = "textures/Circle.png";
            AnimationFrame frame2;
            frame2.frameDuration = .2;
            frame2.sprite = circle;

            circleTree.animationFrames.push_back(frame1);
            circleTree.animationFrames.push_back(frame2);

            GetComponent<AnimationComponent>()->SetAnimation(circleTree);

        }

        void Update(float deltaTime) override {
            if (!flip && GetComponent<BoxColliderComponent>()->CollidesWithAnything()) {
                GetComponent<AnimationComponent>()->SetAnimation(cowboyIdle);
            }
        }

    };

}

#endif