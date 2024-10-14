#include <gtest/gtest.h>
#include "Events/Event.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvents.h"
#include "Events/ApplicationEvent.h"
#include "Events/RenderEvent.h"

TEST(EventTests, VerifyEventTypes) {
  {
    Sprocket::Event event;
    Sprocket::EventType type = event.GetEventType();
    EXPECT_EQ(type, Sprocket::EventType::NONE);
  }
  {
    Sprocket::ApplicationUpdateEvent event(0);
    Sprocket::EventType type = event.GetEventType();
    EXPECT_EQ(type, Sprocket::EventType::APP_UPDATE);
  }
  {
    Sprocket::KeyPressedEvent event(Sprocket::Keycode::KEY_0);
    Sprocket::EventType type = event.GetEventType();
    EXPECT_EQ(type, Sprocket::EventType::KEY_PRESSED);
  }
  {
    Sprocket::KeyReleasedEvent event(Sprocket::Keycode::KEY_0);
    Sprocket::EventType type = event.GetEventType();
    EXPECT_EQ(type, Sprocket::EventType::KEY_RELEASED);
  }
  {
    Sprocket::MouseButtonPressedEvent event(Sprocket::MouseButton::MOUSE_BTN_1);
    Sprocket::EventType type = event.GetEventType();
    EXPECT_EQ(type, Sprocket::EventType::MOUSE_BTN_PRESSED);
  }
  {
    Sprocket::MouseButtonReleasedEvent event(Sprocket::MouseButton::MOUSE_BTN_1);
    Sprocket::EventType type = event.GetEventType();
    EXPECT_EQ(type, Sprocket::EventType::MOUSE_BTN_RELEASED);
  }
  {
    Sprocket::MouseMovedEvent event(0,0);
    Sprocket::EventType type = event.GetEventType();
    EXPECT_EQ(type, Sprocket::EventType::MOUSE_MOVED);
  }
  {
    Sprocket::MouseScrolledEvent event(0,0);
    Sprocket::EventType type = event.GetEventType();
    EXPECT_EQ(type, Sprocket::EventType::MOUSE_SCROLLED);
  }
  {
    Sprocket::WindowCloseEvent event;
    Sprocket::EventType type = event.GetEventType();
    EXPECT_EQ(type, Sprocket::EventType::WINDOW_CLOSE);
  }
  {
    Sprocket::RenderNewEvent event(100);
    Sprocket::EventType type = event.GetEventType();
    EXPECT_EQ(type, Sprocket::EventType::RENDER_NEW);
  }
  {
    Sprocket::RenderUpdateEvent event(Sprocket::RenderUpdateType::MODEL_MATRIX);
    Sprocket::EventType type = event.GetEventType();
    EXPECT_EQ(type, Sprocket::EventType::RENDER_UPDATE);
  }
}

TEST(EventTests, ApplicationEventTest) {
  Sprocket::ApplicationUpdateEvent event1(0);
  Sprocket::ApplicationUpdateEvent event2(1.31f);
  EXPECT_EQ(0, event1.GetDeltaTime());
  EXPECT_EQ(1.31f, event2.GetDeltaTime());
}

TEST(EventTests, KeyboardEventTest) {
  {
    Sprocket::KeyPressedEvent event(Sprocket::Keycode::KEY_0);
    EXPECT_EQ(Sprocket::Keycode::KEY_0, event.GetKeycode());
    EXPECT_EQ(Sprocket::Keycode::KEY_0, ((Sprocket::KeyEvent)event).GetKeycode());
    EXPECT_EQ(48, event.GetKeycode());
  }
  {
    Sprocket::KeyReleasedEvent event(Sprocket::Keycode::KEY_7);
    EXPECT_EQ(Sprocket::Keycode::KEY_7, event.GetKeycode());
    EXPECT_EQ(Sprocket::Keycode::KEY_7, ((Sprocket::KeyEvent)event).GetKeycode());
    EXPECT_EQ(55, event.GetKeycode());
  }
}

TEST(EventTests, MouseEventTests) {
  {
    Sprocket::MouseButtonPressedEvent event(Sprocket::MouseButton::MOUSE_BTN_1);
    EXPECT_EQ(Sprocket::MouseButton::MOUSE_BTN_1, event.GetMouseButton());
    EXPECT_EQ(Sprocket::MouseButton::MOUSE_BTN_1, ((Sprocket::MouseButtonEvent)event).GetMouseButton());
    EXPECT_EQ(0, event.GetMouseButton());
  }
  {
    Sprocket::MouseButtonPressedEvent event(Sprocket::MouseButton::MOUSE_BTN_LEFT);
    EXPECT_EQ(Sprocket::MouseButton::MOUSE_BTN_LEFT, event.GetMouseButton());
    EXPECT_EQ(Sprocket::MouseButton::MOUSE_BTN_LEFT, ((Sprocket::MouseButtonEvent)event).GetMouseButton());
    EXPECT_EQ(0, event.GetMouseButton());
  }
  {
    Sprocket::MouseMovedEvent event(0,0);
    EXPECT_EQ(0, event.GetXPosition());
    EXPECT_EQ(0, event.GetYPosition());
  }
  {
    Sprocket::MouseMovedEvent event(100,-200);
    EXPECT_EQ(100., event.GetXPosition());
    EXPECT_EQ(-200, event.GetYPosition());
  }
  {
    Sprocket::MouseScrolledEvent event(0,0);
    EXPECT_EQ(0, event.GetXOffset());
    EXPECT_EQ(0, event.GetYOffset());
  }
  {
    Sprocket::MouseScrolledEvent event(200.123,-100);
    EXPECT_EQ(200.123, event.GetXOffset());
    EXPECT_EQ(-100, event.GetYOffset());
  }
}