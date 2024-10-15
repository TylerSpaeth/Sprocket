#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ECS/Scene.h>
#include <ECS/Entity.h>
#include <ECS/Component.h>

TEST(EntityTests, RootEntityTest) {

  // Scene is the only way to get a scene root
  Sprocket::Scene* scene = new Sprocket::Scene();
  Sprocket::RootEntity* root = scene->GetSceneRoot();

  // Verify that this is a root with no children
  EXPECT_TRUE(root->IsRoot());
  EXPECT_EQ(0, root->GetChildren().size());
  
  // Verify that the Children can be added and removed
  Sprocket::Entity* e = new Sprocket::Entity(root);
  Sprocket::Entity* e2 = new Sprocket::Entity(root);
  EXPECT_EQ(2, root->GetChildren().size());
  EXPECT_EQ(e, root->GetChildren().at(0));
  delete e;
  EXPECT_EQ(1, root->GetChildren().size());
  EXPECT_EQ(e2, root->GetChildren().at(0));

  delete e2;
  delete scene;
}

TEST(EntityTests, EntityExceptionsTest) {
  Sprocket::Scene* scene = new Sprocket::Scene();
  Sprocket::RootEntity* root = scene->GetSceneRoot();

  // Check Constructor exceptions
  EXPECT_THROW(Sprocket::Entity(nullptr), std::invalid_argument);
  
  // Check SetParent exceptions
  Sprocket::Entity* e = new Sprocket::Entity(root);
  EXPECT_THROW(e->SetParent(e), std::invalid_argument);
  EXPECT_THROW(e->SetParent(nullptr), std::invalid_argument);
  
  delete e;
  delete scene;
}

TEST(EntityTests, EntityTreeFunctionsTest) {
  Sprocket::Scene* scene = new Sprocket::Scene();
  Sprocket::RootEntity* root = scene->GetSceneRoot();

  Sprocket::Entity* e = new Sprocket::Entity(root);

  // Verify entity is constructed with proper values
  EXPECT_FALSE(e->IsRoot());
  EXPECT_EQ(0, e->GetChildren().size());
  EXPECT_EQ(root, e->GetParent());

  Sprocket::Entity* child1 = new Sprocket::Entity(e);
  Sprocket::Entity* child2 = new Sprocket::Entity(e);
  Sprocket::Entity* child3 = new Sprocket::Entity(e);

  // Verify the entity now has 3 children
  EXPECT_EQ(3, e->GetChildren().size());

  child3->SetParent(root);

  // Verify that the root and entity have proper numbers of children
  EXPECT_EQ(2, e->GetChildren().size());
  EXPECT_EQ(2, root->GetChildren().size());

  delete e;

  // Verify that root now has all of the children
  // Order is not something that is being guarenteed right now
  EXPECT_EQ(3, root->GetChildren().size());
  EXPECT_THAT(root->GetChildren(), testing::Contains(child1));
  EXPECT_THAT(root->GetChildren(), testing::Contains(child2));
  EXPECT_THAT(root->GetChildren(), testing::Contains(child3));

  delete child1;
  delete child2;
  delete child3;
  delete scene;
}

TEST(EntityTests, EntityComponentFunctionTests) {
  Sprocket::Scene* scene = new Sprocket::Scene();
  Sprocket::RootEntity* root = scene->GetSceneRoot();

  Sprocket::Entity* e = new Sprocket::Entity(root);

  // TODO reimplement this with a different component

  delete e;
  delete scene;
}

TEST(EntityTests, TransformFunctionsBasicTest) {
  Sprocket::Scene* scene = new Sprocket::Scene();
  Sprocket::RootEntity* root = scene->GetSceneRoot();

  Sprocket::Entity* e = new Sprocket::Entity(root);

  EXPECT_EQ(e->GetLocalTransform().position, e->GetGlobalTransform().position);
  EXPECT_EQ(e->GetLocalTransform().rotation, e->GetGlobalTransform().rotation);
  EXPECT_EQ(e->GetLocalTransform().scale, e->GetGlobalTransform().scale);

  Sprocket::TransformComponent& t = e->GetLocalTransform();

  // Verify initial values
  EXPECT_EQ(t.componentType, Sprocket::ComponentType::TRANSFORM_COMPONENT);
  EXPECT_THROW(e->AddComponent(t), std::invalid_argument);
  EXPECT_EQ(glm::vec3(0), t.position);
  EXPECT_EQ(glm::vec3(0), t.rotation);
  EXPECT_EQ(glm::vec3(1), t.scale);

  t.position.x = 100;
  EXPECT_EQ(100, e->GetLocalTransform().position.x);

  Sprocket::Entity* e2 = new Sprocket::Entity(e);
  EXPECT_EQ(glm::vec3(0), e2->GetLocalTransform().position);
  EXPECT_EQ(glm::vec3(0), e2->GetLocalTransform().rotation);
  EXPECT_EQ(glm::vec3(1), e2->GetLocalTransform().scale);

  EXPECT_EQ(glm::vec3(100,0,0), e2->GetGlobalTransform().position);
  EXPECT_EQ(glm::vec3(0), e2->GetGlobalTransform().rotation);
  EXPECT_EQ(glm::vec3(1), e2->GetGlobalTransform().scale);

  t.rotation.y = 90;

  EXPECT_EQ(glm::vec3(100,0,0), e2->GetGlobalTransform().position);
  EXPECT_EQ(glm::vec3(0, 90, 0), e2->GetGlobalTransform().rotation);
  EXPECT_EQ(glm::vec3(1), e2->GetGlobalTransform().scale);

  EXPECT_EQ(glm::vec3(0), e2->GetLocalTransform().position);
  EXPECT_EQ(glm::vec3(0), e2->GetLocalTransform().rotation);
  EXPECT_EQ(glm::vec3(1), e2->GetLocalTransform().scale);

  delete e2;
  delete e;
  delete scene;
}

TEST(EntityTests, TransformFunctionsComplexTest) {
  Sprocket::Scene* scene = new Sprocket::Scene();
  Sprocket::RootEntity* root = scene->GetSceneRoot();

  Sprocket::Entity* e1 = new Sprocket::Entity(root);
  e1->GetLocalTransform().position = glm::vec3(100,50,5);
  Sprocket::Entity* e2 = new Sprocket::Entity(e1);
  Sprocket::Entity* e3 = new Sprocket::Entity(e2);
  Sprocket::Entity* e4 = new Sprocket::Entity(e3);
  Sprocket::Entity* e5 = new Sprocket::Entity(e1);
  Sprocket::Entity* e6 = new Sprocket::Entity(e5);

  EXPECT_EQ(glm::vec3(100,50,5), e1->GetGlobalTransform().position);
  EXPECT_EQ(glm::vec3(100,50,5), e2->GetGlobalTransform().position);
  EXPECT_EQ(glm::vec3(100,50,5), e3->GetGlobalTransform().position);
  EXPECT_EQ(glm::vec3(100,50,5), e4->GetGlobalTransform().position);
  EXPECT_EQ(glm::vec3(100,50,5), e5->GetGlobalTransform().position);
  EXPECT_EQ(glm::vec3(100,50,5), e6->GetGlobalTransform().position);

  e5->GetLocalTransform().rotation = glm::vec3(45,90,135);

  EXPECT_EQ(glm::vec3(0), e1->GetGlobalTransform().rotation);
  EXPECT_EQ(glm::vec3(0), e2->GetGlobalTransform().rotation);
  EXPECT_EQ(glm::vec3(0), e3->GetGlobalTransform().rotation);
  EXPECT_EQ(glm::vec3(0), e4->GetGlobalTransform().rotation);
  EXPECT_EQ(glm::vec3(45,90,135), e5->GetGlobalTransform().rotation);
  EXPECT_EQ(glm::vec3(45,90,135), e6->GetGlobalTransform().rotation);

  delete e5;

  EXPECT_EQ(glm::vec3(100,50,5), e6->GetGlobalTransform().position);
  EXPECT_EQ(glm::vec3(0), e6->GetGlobalTransform().rotation);

  e1->GetLocalTransform().scale.x = 3;
  e2->GetLocalTransform().scale.x = 0;
  e3->GetLocalTransform().scale.x = 100;
  e6->GetLocalTransform().scale.x = .5;

  EXPECT_EQ(glm::vec3(3,1,1), e1->GetGlobalTransform().scale);
  EXPECT_EQ(glm::vec3(0,1,1), e2->GetGlobalTransform().scale);
  EXPECT_EQ(glm::vec3(0,1,1), e3->GetGlobalTransform().scale);
  EXPECT_EQ(glm::vec3(0,1,1), e4->GetGlobalTransform().scale);
  EXPECT_EQ(glm::vec3(1.5,1,1), e6->GetGlobalTransform().scale);

  delete e6;
  delete e4;
  delete e3;
  delete e2;
  delete e1;
  delete scene;
}