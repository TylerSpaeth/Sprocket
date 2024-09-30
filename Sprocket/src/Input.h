#ifndef INPUT_H
#define INPUT_H

#include <array>
#include <map>
#include "ThirdParty/GLFW/glfw3.h"

#include "Macros.h"

// Keycodes
#define KEY_SPACE                     GLFW_KEY_SPACE        
#define KEY_APOSTROPHE                GLFW_KEY_APOSTROPHE   
#define KEY_COMMA                     GLFW_KEY_COMMA        
#define KEY_MINUS                     GLFW_KEY_MINUS        
#define KEY_PERIOD                    GLFW_KEY_PERIOD       
#define KEY_SLASH                     GLFW_KEY_SLASH        
#define KEY_0                         GLFW_KEY_0            
#define KEY_1                         GLFW_KEY_1            
#define KEY_2                         GLFW_KEY_2            
#define KEY_3                         GLFW_KEY_3            
#define KEY_4                         GLFW_KEY_4            
#define KEY_5                         GLFW_KEY_5            
#define KEY_6                         GLFW_KEY_6            
#define KEY_7                         GLFW_KEY_7            
#define KEY_8                         GLFW_KEY_8            
#define KEY_9                         GLFW_KEY_9            
#define KEY_SEMICOLON                 GLFW_KEY_SEMICOLON    
#define KEY_EQUAL                     GLFW_KEY_EQUAL        
#define KEY_A                         GLFW_KEY_A            
#define KEY_B                         GLFW_KEY_B            
#define KEY_C                         GLFW_KEY_C            
#define KEY_D                         GLFW_KEY_D            
#define KEY_E                         GLFW_KEY_E            
#define KEY_F                         GLFW_KEY_F            
#define KEY_G                         GLFW_KEY_G            
#define KEY_H                         GLFW_KEY_H            
#define KEY_I                         GLFW_KEY_I            
#define KEY_J                         GLFW_KEY_J            
#define KEY_K                         GLFW_KEY_K            
#define KEY_L                         GLFW_KEY_L            
#define KEY_M                         GLFW_KEY_M            
#define KEY_N                         GLFW_KEY_N            
#define KEY_O                         GLFW_KEY_O            
#define KEY_P                         GLFW_KEY_P            
#define KEY_Q                         GLFW_KEY_Q            
#define KEY_R                         GLFW_KEY_R            
#define KEY_S                         GLFW_KEY_S            
#define KEY_T                         GLFW_KEY_T            
#define KEY_U                         GLFW_KEY_U            
#define KEY_V                         GLFW_KEY_V            
#define KEY_W                         GLFW_KEY_W            
#define KEY_X                         GLFW_KEY_X            
#define KEY_Y                         GLFW_KEY_Y            
#define KEY_Z                         GLFW_KEY_Z            
#define KEY_LEFT_BRACKET              GLFW_KEY_LEFT_BRACKET 
#define KEY_BACKSLASH                 GLFW_KEY_BACKSLASH    
#define KEY_RIGHT_BRACKET             GLFW_KEY_RIGHT_BRACKET
#define KEY_GRAVE_ACCENT              GLFW_KEY_GRAVE_ACCENT 
#define KEY_WORLD_1                   GLFW_KEY_WORLD_1      
#define KEY_WORLD_2                   GLFW_KEY_WORLD_2    
#define KEY_ESCAPE                    GLFW_KEY_ESCAPE       
#define KEY_ENTER                     GLFW_KEY_ENTER        
#define KEY_TAB                       GLFW_KEY_TAB          
#define KEY_BACKSPACE                 GLFW_KEY_BACKSPACE    
#define KEY_INSERT                    GLFW_KEY_INSERT       
#define KEY_DELETE                    GLFW_KEY_DELETE       
#define KEY_RIGHT                     GLFW_KEY_RIGHT        
#define KEY_LEFT                      GLFW_KEY_LEFT         
#define KEY_DOWN                      GLFW_KEY_DOWN         
#define KEY_UP                        GLFW_KEY_UP           
#define KEY_PAGE_UP                   GLFW_KEY_PAGE_UP      
#define KEY_PAGE_DOWN                 GLFW_KEY_PAGE_DOWN    
#define KEY_HOME                      GLFW_KEY_HOME         
#define KEY_END                       GLFW_KEY_END          
#define KEY_CAPS_LOCK                 GLFW_KEY_CAPS_LOCK    
#define KEY_SCROLL_LOCK               GLFW_KEY_SCROLL_LOCK  
#define KEY_NUM_LOCK                  GLFW_KEY_NUM_LOCK     
#define KEY_PRINT_SCREEN              GLFW_KEY_PRINT_SCREEN 
#define KEY_PAUSE                     GLFW_KEY_PAUSE        
#define KEY_F1                        GLFW_KEY_F1           
#define KEY_F2                        GLFW_KEY_F2           
#define KEY_F3                        GLFW_KEY_F3           
#define KEY_F4                        GLFW_KEY_F4           
#define KEY_F5                        GLFW_KEY_F5           
#define KEY_F6                        GLFW_KEY_F6           
#define KEY_F7                        GLFW_KEY_F7           
#define KEY_F8                        GLFW_KEY_F8           
#define KEY_F9                        GLFW_KEY_F9           
#define KEY_F10                       GLFW_KEY_F10          
#define KEY_F11                       GLFW_KEY_F11          
#define KEY_F12                       GLFW_KEY_F12          
#define KEY_F13                       GLFW_KEY_F13          
#define KEY_F14                       GLFW_KEY_F14          
#define KEY_F15                       GLFW_KEY_F15          
#define KEY_F16                       GLFW_KEY_F16          
#define KEY_F17                       GLFW_KEY_F17          
#define KEY_F18                       GLFW_KEY_F18          
#define KEY_F19                       GLFW_KEY_F19          
#define KEY_F20                       GLFW_KEY_F20          
#define KEY_F21                       GLFW_KEY_F21          
#define KEY_F22                       GLFW_KEY_F22          
#define KEY_F23                       GLFW_KEY_F23          
#define KEY_F24                       GLFW_KEY_F24          
#define KEY_F25                       GLFW_KEY_F25          
#define KEY_KP_0                      GLFW_KEY_KP_0         
#define KEY_KP_1                      GLFW_KEY_KP_1         
#define KEY_KP_2                      GLFW_KEY_KP_2         
#define KEY_KP_3                      GLFW_KEY_KP_3         
#define KEY_KP_4                      GLFW_KEY_KP_4         
#define KEY_KP_5                      GLFW_KEY_KP_5         
#define KEY_KP_6                      GLFW_KEY_KP_6         
#define KEY_KP_7                      GLFW_KEY_KP_7         
#define KEY_KP_8                      GLFW_KEY_KP_8         
#define KEY_KP_9                      GLFW_KEY_KP_9         
#define KEY_KP_DECIMAL                GLFW_KEY_KP_DECIMAL   
#define KEY_KP_DIVIDE                 GLFW_KEY_KP_DIVIDE    
#define KEY_KP_MULTIPLY               GLFW_KEY_KP_MULTIPLY  
#define KEY_KP_SUBTRACT               GLFW_KEY_KP_SUBTRACT  
#define KEY_KP_ADD                    GLFW_KEY_KP_ADD       
#define KEY_KP_ENTER                  GLFW_KEY_KP_ENTER     
#define KEY_KP_EQUAL                  GLFW_KEY_KP_EQUAL     
#define KEY_LEFT_SHIFT                GLFW_KEY_LEFT_SHIFT   
#define KEY_LEFT_CONTROL              GLFW_KEY_LEFT_CONTROL 
#define KEY_LEFT_ALT                  GLFW_KEY_LEFT_ALT     
#define KEY_LEFT_SUPER                GLFW_KEY_LEFT_SUPER   
#define KEY_RIGHT_SHIFT               GLFW_KEY_RIGHT_SHIFT  
#define KEY_RIGHT_CONTROL             GLFW_KEY_RIGHT_CONTROL
#define KEY_RIGHT_ALT                 GLFW_KEY_RIGHT_ALT    
#define KEY_RIGHT_SUPER               GLFW_KEY_RIGHT_SUPER  
#define KEY_MENU                      GLFW_KEY_MENU   

// Mouse Button Codes
#define MOUSE_BTN_1                   GLFW_MOUSE_BUTTON_1
#define MOUSE_BTN_2                   GLFW_MOUSE_BUTTON_2
#define MOUSE_BTN_3                   GLFW_MOUSE_BUTTON_3
#define MOUSE_BTN_4                   GLFW_MOUSE_BUTTON_4
#define MOUSE_BTN_5                   GLFW_MOUSE_BUTTON_5
#define MOUSE_BTN_6                   GLFW_MOUSE_BUTTON_6
#define MOUSE_BTN_7                   GLFW_MOUSE_BUTTON_7
#define MOUSE_BTN_8                   GLFW_MOUSE_BUTTON_8 
#define MOUSE_BTN_LAST                GLFW_MOUSE_BUTTON_LAST
#define MOUSE_BTN_LEFT                GLFW_MOUSE_BUTTON_LEFT
#define MOUSE_BTN_RIGHT               GLFW_MOUSE_BUTTON_RIGHT
#define MOUSE_BTN_MIDDLE              GLFW_MOUSE_BUTTON_MIDDLE

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