#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ECS/Scene.h>
#include <ECS/Entity.h>

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

TEST(EntityTests, EntityFunctionalityTest) {
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