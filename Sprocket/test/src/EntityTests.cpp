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

  Sprocket::TestComponent testComponent;
  Sprocket::TransformComponent transformComponent;
  
  // Verify that both components have the correct type
  EXPECT_EQ(Sprocket::ComponentType::TEST_COMPONENT, testComponent.GetComponentType());
  EXPECT_EQ(Sprocket::ComponentType::TRANSFORM_COMPONENT, transformComponent.GetComponentType());

  // Verify that a transform component can not be added
  EXPECT_THROW(e->AddComponent(transformComponent), std::invalid_argument);

  // Add the same component twice to e
  unsigned int id1 = e->AddComponent(testComponent);
  unsigned int id2 = e->AddComponent(testComponent);

  // Verify that they have different ids
  EXPECT_NE(id1, id2);
  
  EXPECT_EQ(((Sprocket::TestComponent&)e->GetComponent(id1)).value, ((Sprocket::TestComponent&)e->GetComponent(id2)).value);

  // Verify that the original value for the first component is not what we are going to set it to
  // and then verify that changing the value actually updates the value
  EXPECT_NE(11951, ((Sprocket::TestComponent&)e->GetComponent(id1)).value);
  ((Sprocket::TestComponent&)e->GetComponent(id1)).value = 11951;
  EXPECT_EQ(11951, ((Sprocket::TestComponent&)e->GetComponent(id1)).value);

  // Make sure that the only value that changed was the one referenced by id1
  EXPECT_NE(((Sprocket::TestComponent&)e->GetComponent(id1)).value, testComponent.value);
  EXPECT_NE(((Sprocket::TestComponent&)e->GetComponent(id1)).value, ((Sprocket::TestComponent&)e->GetComponent(id2)).value);

  unsigned int id3 = e->AddComponent(testComponent);

  // Verify that trying to remove a component at an invalid id does not work
  EXPECT_THROW(e->RemoveComponent(id1+id2+id3), std::invalid_argument);

  EXPECT_NO_THROW(e->RemoveComponent(id2));

  // Verify that you can not remove the same component twice
  EXPECT_THROW(e->RemoveComponent(id2), std::invalid_argument);

  // Make sure the correct values still exist
  EXPECT_EQ(11951, ((Sprocket::TestComponent&)e->GetComponent(id1)).value);
  EXPECT_THROW(e->GetComponent(id2), std::invalid_argument);
  EXPECT_EQ(testComponent.value, ((Sprocket::TestComponent&)e->GetComponent(id3)).value);

  // Make sure that the id gets reassigned now that it is free
  EXPECT_EQ(id2, e->AddComponent(testComponent));

  delete e;
  delete scene;
}