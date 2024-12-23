#include <gtest/gtest.h>
#include "ECS/Collision.h"
#include "ECS/Component.h"

// TODO test other collision types

TEST(CollisionTests, BoxBox) {

  Sprocket::TransformComponent t1;
  t1.position = {0,0,0};
  t1.rotation = {0,0,0};
  t1.scale = {1,1,1};

  Sprocket::TransformComponent t2;
  t2.position = {100.001,100.001,0};
  t2.rotation = {0,0,0};
  t2.scale = {1,1,1};

  Sprocket::BoxColliderComponent b;
  b.height = 100;
  b.width = 100;

  // Loop the second box around the entire border to the first box and make sure it does not collide
  for(int i = 0; i < 200; i++) {
    EXPECT_FALSE(Sprocket::Collision::Collides(b,t1,b,t2));
    t2.position.x -= 1;
  }
  t2.position.x -= .02;
  for(int i = 0; i < 200; i++) {
    EXPECT_FALSE(Sprocket::Collision::Collides(b,t1,b,t2));
    t2.position.y -= 1;
  }
  t2.position.y -= .02;
  for(int i = 0; i < 200; i++) {
    EXPECT_FALSE(Sprocket::Collision::Collides(b,t1,b,t2));
    t2.position.x += 1;
  }
  t2.position.x += .02;
  for(int i = 0; i < 201; i++) {
    EXPECT_FALSE(Sprocket::Collision::Collides(b,t1,b,t2));
    t2.position.y += 1;
  }

  // Run the borders of the boxes against each other an must sure that they collide
  t2.position = {100,100,0};

  for(int i = 0; i < 200; i++) {
    EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
    t2.position.x -= 1;
  }
  for(int i = 0; i < 200; i++) {
    EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
    t2.position.y -= 1;
  }
  for(int i = 0; i < 200; i++) {
    EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
    t2.position.x += 1;
  }
  for(int i = 0; i < 200; i++) {
    EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
    t2.position.y += 1;
  }

  // Put second box in the middle of the first and make sure it collides no matter the difference i scale
  t2.position = {0,0,0};
  EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
  t2.scale = {.5,.5,.5};
  EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
  t2.scale = {2,2,2};
  EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
  t2.scale = {0,0,0};
  EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));

  // Tests for rotated box
  
  // Test with all corners of box sticking out of the other box
  t2.scale = {1,1,1};
  t2.rotation.z = 45;
  EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
  
  // No collision
  {
    t2.position.x = 50.001 + sqrt(2*pow(50,2));
    t2.position.y = -50.001;

    // Run point of rotated on right edge of non-rotated
    for(int i = 0; i < 100; i++) {
      EXPECT_FALSE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.y += 1;
    }
    t2.position.y += 0.2;

    // Run bottom left edge of rotated along top right point of unrotated
    for(int i = 0; i < 100/sqrt(2); i++) {
      EXPECT_FALSE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.y += 1;
      t2.position.x -= 1;
    }

    t2.position.x = 50.001;
    t2.position.y = 50.001 + sqrt(2*pow(50,2));

    // Run bottom point of rotated along top edge of unrotated
    for(int i = 0; i < 100; i++) {
      EXPECT_FALSE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.x -= 1;
    }
    t2.position.x -= 0.2;

    
    // Run bottom right edge of rotated along top left point of unrotated
    for(int i = 0; i < 100/sqrt(2); i++) {
      EXPECT_FALSE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.y -= 1;
      t2.position.x -= 1;
    }

    t2.position.x = -50.001 - sqrt(2*pow(50,2));
    t2.position.y = 50.001;

    // Run right point of rotated along left edge of unrotated
    for(int i = 0; i < 100; i++) {
      EXPECT_FALSE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.y -= 1;
    }
    t2.position.y -= 0.2;

    // Run top right edge of rotated along bottom left point of unrotated
    for(int i = 0; i < 100/sqrt(2); i++) {
      EXPECT_FALSE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.y -= 1;
      t2.position.x += 1;
    }

    t2.position.x = -50.001;
    t2.position.y = -50.001 - sqrt(2*pow(50,2));

    // Run top point of rotated along bottom edge of unrotated
    for(int i = 0; i < 100; i++) {
      EXPECT_FALSE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.x += 1;
    }
    t2.position.x += 0.2;

    // Run top left edge of rotated along bottom right point of unrotated
    for(int i = 0; i < 100/sqrt(2); i++) {
      EXPECT_FALSE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.y += 1;
      t2.position.x += 1;
    }
  }

  // Collision
  {
    t2.position.x = 50 + sqrt(2*pow(50,2));
    t2.position.y = -50;

    // Run point of rotated on right edge of non-rotated
    for(int i = 0; i < 100; i++) {
      EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.y += 1;
    }

    // Run bottom left edge of rotated along top right point of unrotated
    for(int i = 0; i < 100/sqrt(2); i++) {
      EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.y += 1;
      t2.position.x -= 1;
    }

    t2.position.x = 50;
    t2.position.y = 50 + sqrt(2*pow(50,2));

    // Run bottom point of rotated along top edge of unrotated
    for(int i = 0; i < 100; i++) {
      EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.x -= 1;
    }
    
    // Run bottom right edge of rotated along top left point of unrotated
    for(int i = 0; i < 100/sqrt(2); i++) {
      EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.y -= 1;
      t2.position.x -= 1;
    }

    t2.position.x = -50 - sqrt(2*pow(50,2));
    t2.position.y = 50;

    // Run right point of rotated along left edge of unrotated
    for(int i = 0; i < 100; i++) {
      EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.y -= 1;
    }

    // Run top right edge of rotated along bottom left point of unrotated
    for(int i = 0; i < 100/sqrt(2); i++) {
      EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.y -= 1;
      t2.position.x += 1;
    }

    t2.position.x = -50;
    t2.position.y = -50 - sqrt(2*pow(50,2));

    // Run top point of rotated along bottom edge of unrotated
    for(int i = 0; i < 100; i++) {
      EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.x += 1;
    }

    // Run top left edge of rotated along bottom right point of unrotated
    for(int i = 0; i < 100/sqrt(2); i++) {
      EXPECT_TRUE(Sprocket::Collision::Collides(b,t1,b,t2));
      t2.position.y += 1;
      t2.position.x += 1;
    }
  }

}

TEST(CollisionTests, CircleCircle) {

  Sprocket::TransformComponent t1;
  t1.position = {0,0,0};
  t1.rotation = {0,0,0};
  t1.scale = {1,1,1};

  Sprocket::TransformComponent t2;
  t2.position = {100.00,0,0};
  t2.rotation = {0,0,0};
  t2.scale = {1,1,1};

  Sprocket::CircleColliderComponent c;
  c.radius = 50;

  // Rotate one circle around the other just far enough apart not to collide
  for(int i = 1; i <= 360; i++) {
    t2.position.x = 100.01 * cos((M_PI * i /360) * 2);
    t2.position.y = 100.01 * sin((M_PI * i /360) * 2);
    EXPECT_FALSE(Sprocket::Collision::Collides(c,t1,c,t2));
  }
  // Rotate one circle around the other with edges touching the whole time
  for(int i = 1; i <= 360; i++) {
    t2.position.x = 100 * cos((M_PI * i /360) * 2);
    t2.position.y = 100 * sin((M_PI * i /360) * 2);
    EXPECT_TRUE(Sprocket::Collision::Collides(c,t1,c,t2));
  }

  // Verify that stacked circles does not cause a problem
  t2.position = {0,0,0};
  EXPECT_TRUE(Sprocket::Collision::Collides(c,t1,c,t2));  
  t2.scale.x = .5;
  EXPECT_TRUE(Sprocket::Collision::Collides(c,t1,c,t2));
  EXPECT_TRUE(Sprocket::Collision::Collides(c,t2,c,t1));

  // Verify rotation has no effect
  t2.scale.x = 1;
  t2.position = {100.00,0,0};
  t2.rotation.z = 45;

  // Rotate one circle around the other just far enough apart not to collide
  for(int i = 1; i <= 360; i++) {
    t2.position.x = 100.01 * cos((M_PI * i /360) * 2);
    t2.position.y = 100.01 * sin((M_PI * i /360) * 2);
    EXPECT_FALSE(Sprocket::Collision::Collides(c,t1,c,t2));
  }
  // Rotate one circle around the other with edges touching the whole time
  for(int i = 1; i <= 360; i++) {
    t2.position.x = 100 * cos((M_PI * i /360) * 2);
    t2.position.y = 100 * sin((M_PI * i /360) * 2);
    EXPECT_TRUE(Sprocket::Collision::Collides(c,t1,c,t2));
  }

}

// TODO perform more intensive tests on the corners and rotation 
TEST(CollisionTests, BoxCircle) {

  Sprocket::TransformComponent bt;
  bt.position = {0,0,0};
  bt.rotation = {0,0,0};
  bt.scale = {1,1,1};

  Sprocket::TransformComponent ct;
  ct.position = {100.001,-50,0};
  ct.rotation = {0,0,0};
  ct.scale = {1,1,1};

  Sprocket::BoxColliderComponent b;
  b.height = 100;
  b.width = 100;

  Sprocket::CircleColliderComponent c;
  c.radius = 50;

  ////////// NO COLLISION //////////

  // Run circle along right edge
  for(int i = 0; i < 100; i++) {
    EXPECT_FALSE(Sprocket::Collision::Collides(b,bt,c,ct));
    EXPECT_FALSE(Sprocket::Collision::Collides(c,ct,b,bt));
    ct.position.y++;
  }

  // Check circle at btop right corner
  ct.position.x = 50 + sqrt(pow(50,2)/2) + .001;
  ct.position.y = 50 + sqrt(pow(50,2)/2) + .001;
  EXPECT_FALSE(Sprocket::Collision::Collides(b,bt,c,ct));
  EXPECT_FALSE(Sprocket::Collision::Collides(c,ct,b,bt));

  // Run circle along top edge
  ct.position.x = 50;
  ct.position.y = 100.001;
  for(int i = 0; i < 100; i++) {
    EXPECT_FALSE(Sprocket::Collision::Collides(b,bt,c,ct));
    EXPECT_FALSE(Sprocket::Collision::Collides(c,ct,b,bt));
    ct.position.x--;
  }

  // Check circle at top left corner
  ct.position.x = -50 - sqrt(pow(50,2)/2) - .001;
  ct.position.y = 50 + sqrt(pow(50,2)/2) + .001;
  EXPECT_FALSE(Sprocket::Collision::Collides(b,bt,c,ct));
  EXPECT_FALSE(Sprocket::Collision::Collides(c,ct,b,bt));

  // Run circle along left edge
  ct.position.x = -100.001;
  ct.position.y = 50;
  for(int i = 0; i < 100; i++) {
    EXPECT_FALSE(Sprocket::Collision::Collides(b,bt,c,ct));
    EXPECT_FALSE(Sprocket::Collision::Collides(c,ct,b,bt));
    ct.position.y--;
  }

  // Check circle at bottom left corner
  ct.position.x = -50 - sqrt(pow(50,2)/2) - .001;
  ct.position.y = -50 - sqrt(pow(50,2)/2) - .001;
  EXPECT_FALSE(Sprocket::Collision::Collides(b,bt,c,ct));
  EXPECT_FALSE(Sprocket::Collision::Collides(c,ct,b,bt));

  // Run circle along bottom edge
  ct.position.x = -50;
  ct.position.y = -100.001;
  for(int i = 0; i < 100; i++) {
    EXPECT_FALSE(Sprocket::Collision::Collides(b,bt,c,ct));
    EXPECT_FALSE(Sprocket::Collision::Collides(c,ct,b,bt));
    ct.position.x++;
  }

  // Check circle at bottom right corner
  ct.position.x = 50 + sqrt(pow(50,2)/2) + .001;
  ct.position.y = -50 - sqrt(pow(50,2)/2) - .001;
  EXPECT_FALSE(Sprocket::Collision::Collides(b,bt,c,ct));
  EXPECT_FALSE(Sprocket::Collision::Collides(c,ct,b,bt));


  ////////// COLLISION //////////

  ct.position.x = 100;
  ct.position.y = -50;
  // Run circle along right edge
  for(int i = 0; i < 100; i++) {
    EXPECT_TRUE(Sprocket::Collision::Collides(b,bt,c,ct));
    EXPECT_TRUE(Sprocket::Collision::Collides(c,ct,b,bt));
    ct.position.y++;
  }

  // Check circle at btop right corner
  ct.position.x = 50 + sqrt(pow(50,2)/2);
  ct.position.y = 50 + sqrt(pow(50,2)/2);
  EXPECT_TRUE(Sprocket::Collision::Collides(b,bt,c,ct));
  EXPECT_TRUE(Sprocket::Collision::Collides(c,ct,b,bt));

  // Run circle along top edge
  ct.position.x = 50;
  ct.position.y = 100.;
  for(int i = 0; i < 100; i++) {
    EXPECT_TRUE(Sprocket::Collision::Collides(b,bt,c,ct));
    EXPECT_TRUE(Sprocket::Collision::Collides(c,ct,b,bt));
    ct.position.x--;
  }

  // Check circle at top left corner
  ct.position.x = -50 - sqrt(pow(50,2)/2);
  ct.position.y = 50 + sqrt(pow(50,2)/2);
  EXPECT_TRUE(Sprocket::Collision::Collides(b,bt,c,ct));
  EXPECT_TRUE(Sprocket::Collision::Collides(c,ct,b,bt));

  // Run circle along left edge
  ct.position.x = -100;
  ct.position.y = 50;
  for(int i = 0; i < 100; i++) {
    EXPECT_TRUE(Sprocket::Collision::Collides(b,bt,c,ct));
    EXPECT_TRUE(Sprocket::Collision::Collides(c,ct,b,bt));
    ct.position.y--;
  }

  // Check circle at bottom left corner
  ct.position.x = -50 - sqrt(pow(50,2)/2);
  ct.position.y = -50 - sqrt(pow(50,2)/2);
  EXPECT_TRUE(Sprocket::Collision::Collides(b,bt,c,ct));
  EXPECT_TRUE(Sprocket::Collision::Collides(c,ct,b,bt));

  // Run circle along bottom edge
  ct.position.x = -50;
  ct.position.y = -100;
  for(int i = 0; i < 100; i++) {
    EXPECT_TRUE(Sprocket::Collision::Collides(b,bt,c,ct));
    EXPECT_TRUE(Sprocket::Collision::Collides(c,ct,b,bt));
    ct.position.x++;
  }

  // Check circle at bottom right corner
  ct.position.x = 50 + sqrt(pow(50,2)/2);
  ct.position.y = -50 - sqrt(pow(50,2)/2);
  EXPECT_TRUE(Sprocket::Collision::Collides(b,bt,c,ct));
  EXPECT_TRUE(Sprocket::Collision::Collides(c,ct,b,bt));
  
}