#include <gtest/gtest.h>

#include "Core/Input.h"
#include "Events/Event.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

TEST(InputTests, InitializeSystem) {
  Sprocket::Input::Init();
}

TEST(InputTests, VerifyInitialValues) {
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_SPACE));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_APOSTROPHE));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_COMMA));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_MINUS));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_PERIOD));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_SLASH));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_0));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_1));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_2));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_3));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_4));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_5));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_6));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_7));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_8));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_SEMICOLON));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_EQUAL));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_A));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_B));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_C));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_D));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_E));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_G));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_H));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_I));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_J));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_K));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_L));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_M));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_N));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_O));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_P));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_Q));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_R));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_S));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_T));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_U));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_V));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_W));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_X));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_Y));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_Z));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_LEFT_BRACKET));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_BACKSLASH));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_RIGHT_BRACKET));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_GRAVE_ACCENT));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_WORLD_1));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_WORLD_2));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_ESCAPE));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_ENTER));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_TAB));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_BACKSPACE));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_INSERT));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_DELETE));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_RIGHT));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_LEFT));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_DOWN));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_UP));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_PAGE_UP));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_PAGE_DOWN));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_HOME));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_END));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_CAPS_LOCK));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_SCROLL_LOCK));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_NUM_LOCK));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_PRINT_SCREEN));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_PAUSE));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F1));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F2));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F3));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F4));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F5));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F6));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F7));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F8));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F9));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F10));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F11));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F12));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F13));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F14));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F15));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F16));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F17));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F18));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F19));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F20));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F21));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F22));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F23));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F24));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_F25));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_0));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_1));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_2));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_3));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_4));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_5));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_6));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_7));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_8));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_9));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_DECIMAL));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_DIVIDE));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_MULTIPLY));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_SUBTRACT));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_ADD));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_ENTER));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_KP_EQUAL));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_LEFT_SHIFT));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_LEFT_CONTROL));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_LEFT_ALT));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_LEFT_SUPER));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_RIGHT_SHIFT));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_RIGHT_CONTROL));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_RIGHT_ALT));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_RIGHT_SUPER));
  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_MENU));

  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_1));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_2));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_3));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_4));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_5));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_6));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_7));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_8));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_LAST));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_LEFT));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_RIGHT));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_MIDDLE));

  EXPECT_EQ(0, Sprocket::Input::GetMouseXPosition());
  EXPECT_EQ(0, Sprocket::Input::GetMouseYPosition());
  EXPECT_EQ(0, Sprocket::Input::GetMouseXScrollOffset());
  EXPECT_EQ(0, Sprocket::Input::GetMouseYScrollOffset());
}

TEST(InputTests, KeyboardPress) {

  {
    Sprocket::KeyPressedEvent e(Sprocket::Keycode::KEY_0);
    Sprocket::Input::OnEvent(e);
  }

  EXPECT_TRUE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_0));

  {
    Sprocket::KeyReleasedEvent e(Sprocket::Keycode::KEY_0);
    Sprocket::Input::OnEvent(e);
  }

  EXPECT_FALSE(Sprocket::Input::IsKeyPressed(Sprocket::Keycode::KEY_0));
}

TEST(InputTests, MousePress) {

  {
    Sprocket::MouseButtonPressedEvent e(Sprocket::MouseButton::MOUSE_BTN_1);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_TRUE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_1));
  EXPECT_TRUE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_LEFT));
  {
    Sprocket::MouseButtonReleasedEvent e(Sprocket::MouseButton::MOUSE_BTN_LEFT);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_1));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_LEFT));

  {
    Sprocket::MouseButtonPressedEvent e(Sprocket::MouseButton::MOUSE_BTN_2);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_TRUE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_2));
  EXPECT_TRUE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_RIGHT));
  {
    Sprocket::MouseButtonReleasedEvent e(Sprocket::MouseButton::MOUSE_BTN_RIGHT);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_2));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_RIGHT));

  {
    Sprocket::MouseButtonPressedEvent e(Sprocket::MouseButton::MOUSE_BTN_3);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_TRUE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_3));
  EXPECT_TRUE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_MIDDLE));
  {
    Sprocket::MouseButtonReleasedEvent e(Sprocket::MouseButton::MOUSE_BTN_MIDDLE);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_3));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_MIDDLE));

  {
    Sprocket::MouseButtonPressedEvent e(Sprocket::MouseButton::MOUSE_BTN_4);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_TRUE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_4));
  {
    Sprocket::MouseButtonReleasedEvent e(Sprocket::MouseButton::MOUSE_BTN_4);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_4));
  
  {
    Sprocket::MouseButtonPressedEvent e(Sprocket::MouseButton::MOUSE_BTN_5);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_TRUE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_5));
  {
    Sprocket::MouseButtonReleasedEvent e(Sprocket::MouseButton::MOUSE_BTN_5);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_5));

  {
    Sprocket::MouseButtonPressedEvent e(Sprocket::MouseButton::MOUSE_BTN_6);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_TRUE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_6));
  {
    Sprocket::MouseButtonReleasedEvent e(Sprocket::MouseButton::MOUSE_BTN_6);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_6));

  {
    Sprocket::MouseButtonPressedEvent e(Sprocket::MouseButton::MOUSE_BTN_7);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_TRUE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_7));
  {
    Sprocket::MouseButtonReleasedEvent e(Sprocket::MouseButton::MOUSE_BTN_7);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_7));

  {
    Sprocket::MouseButtonPressedEvent e(Sprocket::MouseButton::MOUSE_BTN_8);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_TRUE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_8));
  EXPECT_TRUE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_LAST));
  {
    Sprocket::MouseButtonReleasedEvent e(Sprocket::MouseButton::MOUSE_BTN_LAST);
    Sprocket::Input::OnEvent(e);
  }
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_8));
  EXPECT_FALSE(Sprocket::Input::IsMouseButtonPressed(Sprocket::MouseButton::MOUSE_BTN_LAST));

}

TEST(InputTests, MouseMove) {

  {
    Sprocket::MouseMovedEvent e(150,-120);
    Sprocket::Input::OnEvent(e);
  }

  EXPECT_EQ(150, Sprocket::Input::GetMouseXPosition());
  EXPECT_EQ(-120, Sprocket::Input::GetMouseYPosition());

  {
    Sprocket::MouseMovedEvent e(1,-15);
    Sprocket::Input::OnEvent(e);
  }

  EXPECT_EQ(1, Sprocket::Input::GetMouseXPosition());
  EXPECT_EQ(-15, Sprocket::Input::GetMouseYPosition());
  
}

TEST(InputTests, MouseScroll) {

  {
    Sprocket::MouseScrolledEvent e(150,-120);
    Sprocket::Input::OnEvent(e);
  }

  EXPECT_EQ(150, Sprocket::Input::GetMouseXScrollOffset());
  EXPECT_EQ(-120, Sprocket::Input::GetMouseYScrollOffset());

  {
    Sprocket::MouseScrolledEvent e(1,-15);
    Sprocket::Input::OnEvent(e);
  }

  EXPECT_EQ(1, Sprocket::Input::GetMouseXScrollOffset());
  EXPECT_EQ(-15, Sprocket::Input::GetMouseYScrollOffset());
  
}
