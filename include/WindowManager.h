#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowManager {
    public:
        // Delete copy constructor and assignment operator
        WindowManager(const WindowManager&) = delete;
        void operator=(const WindowManager&) = delete;

        // Get a reference to the window manager
        static WindowManager& getInstance()
        {
            static WindowManager instance;
            return instance;
        }

        // Member function prototypes 
        bool init();
        bool isInitialized() const { return m_isInitialized; }
        int getMaxAttributes() const; // Get maximum number of vertex attributes supported by the hardware

        GLFWwindow* getWindow();


    private: 

        // Use an initializer list to skip assigning a default value to variables on construction
        // Init order needs to match declaration order 
        WindowManager() : 
            m_isInitialized(false),
            m_width(1280),
            m_height(720),
            m_title("LearnOpenGL") {}

        bool m_isInitialized;
        int m_width;
        int m_height;
        const char* m_title;

        GLFWwindow* m_window;
};