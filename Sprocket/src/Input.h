#ifndef INPUT_H
#define INPUT_H

#include <array>
#include <map>
#include "ThirdParty/GLFW/glfw3.h"
#include "Keycodes.h"
#include "MouseButtons.h"

#include "Macros.h"

struct KeyState {
  bool Pressed;
  bool Repeat;
  bool Released;
};

struct MouseButtonState {
  bool Pressed;
  bool Released;
 };

class SPROCKET_API Input {
  private:
    GLFWwindow* m_Window;
    std::map<unsigned int, KeyState> m_Keys;
    std::map<unsigned int, MouseButtonState> m_MouseButtons;
    double m_CursorXPos;
    double m_CursorYPos;
    double m_MouseScrollXOffset;
    double m_MouseScrollYOffset;
  public:
    const std::array<unsigned int, 120> s_Keycodes = {
        KEY_SPACE,        
        KEY_APOSTROPHE,   
        KEY_COMMA,        
        KEY_MINUS,        
        KEY_PERIOD,       
        KEY_SLASH,        
        KEY_0,            
        KEY_1,            
        KEY_2,            
        KEY_3,            
        KEY_4,            
        KEY_5,            
        KEY_6,            
        KEY_7,            
        KEY_8,            
        KEY_9,            
        KEY_SEMICOLON,    
        KEY_EQUAL,        
        KEY_A,            
        KEY_B,            
        KEY_C,            
        KEY_D,            
        KEY_E,            
        KEY_F,            
        KEY_G,            
        KEY_H,            
        KEY_I,            
        KEY_J,            
        KEY_K,            
        KEY_L,            
        KEY_M,            
        KEY_N,            
        KEY_O,            
        KEY_P,            
        KEY_Q,            
        KEY_R,            
        KEY_S,            
        KEY_T,            
        KEY_U,            
        KEY_V,            
        KEY_W,            
        KEY_X,            
        KEY_Y,            
        KEY_Z,            
        KEY_LEFT_BRACKET, 
        KEY_BACKSLASH,    
        KEY_RIGHT_BRACKET,
        KEY_GRAVE_ACCENT, 
        KEY_WORLD_1,      
        KEY_WORLD_2,      
        KEY_ESCAPE,       
        KEY_ENTER,        
        KEY_TAB,          
        KEY_BACKSPACE,    
        KEY_INSERT,       
        KEY_DELETE,       
        KEY_RIGHT,        
        KEY_LEFT,         
        KEY_DOWN,         
        KEY_UP,           
        KEY_PAGE_UP,      
        KEY_PAGE_DOWN,    
        KEY_HOME,         
        KEY_END,          
        KEY_CAPS_LOCK,    
        KEY_SCROLL_LOCK,  
        KEY_NUM_LOCK,     
        KEY_PRINT_SCREEN, 
        KEY_PAUSE,        
        KEY_F1,           
        KEY_F2,           
        KEY_F3,           
        KEY_F4,           
        KEY_F5,           
        KEY_F6,           
        KEY_F7,           
        KEY_F8,           
        KEY_F9,           
        KEY_F10,          
        KEY_F11,          
        KEY_F12,          
        KEY_F13,          
        KEY_F14,          
        KEY_F15,          
        KEY_F16,          
        KEY_F17,          
        KEY_F18,          
        KEY_F19,          
        KEY_F20,          
        KEY_F21,          
        KEY_F22,          
        KEY_F23,          
        KEY_F24,          
        KEY_F25,          
        KEY_KP_0,         
        KEY_KP_1,         
        KEY_KP_2,         
        KEY_KP_3,         
        KEY_KP_4,         
        KEY_KP_5,         
        KEY_KP_6,         
        KEY_KP_7,         
        KEY_KP_8,         
        KEY_KP_9,         
        KEY_KP_DECIMAL,   
        KEY_KP_DIVIDE,    
        KEY_KP_MULTIPLY,  
        KEY_KP_SUBTRACT,  
        KEY_KP_ADD,       
        KEY_KP_ENTER,     
        KEY_KP_EQUAL,     
        KEY_LEFT_SHIFT,   
        KEY_LEFT_CONTROL, 
        KEY_LEFT_ALT,     
        KEY_LEFT_SUPER,   
        KEY_RIGHT_SHIFT,  
        KEY_RIGHT_CONTROL,
        KEY_RIGHT_ALT,    
        KEY_RIGHT_SUPER,  
        KEY_MENU         
    };

    const std::array<unsigned int, 12> s_MouseButtonCodes = {
        MOUSE_BTN_1,
        MOUSE_BTN_2,
        MOUSE_BTN_3,
        MOUSE_BTN_4,
        MOUSE_BTN_5,
        MOUSE_BTN_6,
        MOUSE_BTN_7,
        MOUSE_BTN_8,
        MOUSE_BTN_LAST,
        MOUSE_BTN_LEFT,
        MOUSE_BTN_RIGHT,
        MOUSE_BTN_MIDDLE
    };
    
    Input(GLFWwindow* window);
      
    void UpdateKeyState(int key, int action);
    inline bool IsKeyPressed(int key) const {return m_Keys.at(key).Pressed;}
    inline bool IsKeyReleased(int key) const {return m_Keys.at(key).Released;}
    inline bool IsKeyRepeated(int key) const {return m_Keys.at(key).Repeat;}
    
    void UpdateCursorPosition(double cursorXPos, double cursorYPos);
    inline double GetCursorXPosition() const {return m_CursorXPos;}
    inline double GetCursorYPosition() const {return m_CursorYPos;}
    void EnableCursor();
    void HideCursor();
    void DisableCursor();
    void EnableRawMouseInput();
    void DisableRawMouseInput();

    void UpdateMouseButtonState(int button, int action);
    inline bool IsMouseButtonPressed(int button) const {return m_MouseButtons.at(button).Pressed;}
    inline bool IsMouseButtonReleased(int button) const {return m_MouseButtons.at(button).Released;}

    void UpdateMouseScrollOffset(double xOffset, double yOffset);
    inline double GetMouseScrollXOffset() const {return m_MouseScrollXOffset;}
    inline double GetMouseScrollYOffset() const {return m_MouseScrollYOffset;}
};

#endif