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

            AnimationFrame frame1;
            AnimationFrame frame2;
            AnimationFrame frame3;
            AnimationFrame frame4;
            Sprite sprite1;
            Sprite sprite2;
            Sprite sprite3;
            Sprite sprite4;
            sprite1.texturePath = "textures/Bandit-Idle96x96.png";
            sprite2.texturePath = "textures/Bandit-Idle96x96.png";
            sprite3.texturePath = "textures/Bandit-Idle96x96.png";
            sprite4.texturePath = "textures/Bandit-Idle96x96.png";
            sprite1.textureXUVCoords = {0.25, 0.25, 0, 0};
            sprite2.textureXUVCoords = {0.5, 0.5, 0.25, 0.25};
            sprite3.textureXUVCoords = {0.75, 0.75, 0.5, 0.5};
            sprite4.textureXUVCoords = {1, 1, 0.75, 0.75};
            sprite1.textureYUVCoords = {1,0,0,1};
            sprite2.textureYUVCoords = { 1,0,0,1 };
            sprite3.textureYUVCoords = { 1,0,0,1 };
            sprite4.textureYUVCoords = { 1,0,0,1 };
            frame1.sprite = sprite1;
            frame2.sprite = sprite2;
            frame3.sprite = sprite3;
            frame4.sprite = sprite4;
            frame1.frameDuration = .25;
            frame2.frameDuration = .25;
            frame3.frameDuration = .25;
            frame4.frameDuration = .25;

            cowboyIdle.animationFrames.push_back(frame1);
            cowboyIdle.animationFrames.push_back(frame2);
            cowboyIdle.animationFrames.push_back(frame3);
            cowboyIdle.animationFrames.push_back(frame4);

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