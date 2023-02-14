#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <string>
#include <cstdlib>

using namespace std;

// Globals
float x, y, score = 0;
int flying = 0;
int countLevel = 0;
int coinCollected = 0;
float my_min(float a, float b)
{
    if (a < b)
    {
        return a;
    }
    return b;
}
float my_max(float a, float b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

// Predefining the Functions
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

void lostGame(GLFWwindow *window)
{
    cout << "\n\n\nYour Score is " << (int)score << endl;
    cout << "Total Coins Collected are " << coinCollected << endl;
    cout << "-----------------You Lost!-----------------\n\n\n"
         << endl;
    glfwSetWindowShouldClose(window, true);
}

void Levels()
{
    if (score > 100 && countLevel == 0)
    {
        cout << "\n\n\nYou Completed Level 1!" << endl;
        countLevel++;
    }
    if (score > 200 && countLevel == 1)
    {
        cout << "You Completed Level 2!" << endl;
        countLevel++;
    }
    if (score > 300 && countLevel == 2)
    {
        cout << "You Completed Level 3!" << endl;
        countLevel++;
    }
}

void winGame(GLFWwindow *window)
{
    if (score > 400)
    {
        cout << "\nYour Score is " << (int)score << endl;
        cout << "Total Coins Collected are " << coinCollected << endl;
        cout << "-----------------You Win!-----------------\n\n\n"
             << endl;
        glfwSetWindowShouldClose(window, true);
    }
}

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// Shaders
const char *playerVertexShaderSource = "#version 330 core\n"
                                       "layout (location = 0) in vec3 aPos;\n"
                                       "layout (location = 1) in vec3 aColor;\n"
                                       "layout (location = 2) in vec2 aTexCoord;\n"
                                       "uniform vec4 offSet;\n"
                                       "uniform mat4 transform;\n"
                                       "out vec3 ourColor;\n"
                                       "out vec2 TexCoord;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   gl_Position = (transform * vec4(aPos,1.0f)) + offSet;\n"
                                       "   ourColor = aColor;\n"
                                       "   TexCoord = aTexCoord;\n"
                                       "}\n\0";
const char *playerFragmentShaderSource = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         "in vec3 ourColor;\n"
                                         "in vec2 TexCoord;\n"
                                         "uniform sampler2D ourTexture;\n"
                                         "uniform int isZapper;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   vec4 temp = texture(ourTexture, TexCoord);\n"
                                         "   if (temp.a < 1)\n"
                                         "{\n"
                                         "            discard;\n"
                                         "}\n"
                                         "float dist = TexCoord.y;\n"
                                         "vec4 yellow = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                         "vec4 red = vec4(1.0f, 0.0f,0.0f, 1.0f);\n"
                                         "float step1 = 0.0f; \n"
                                         "float step2 = 0.5f;\n"
                                         "float step3 = 1.0f;\n"

                                         "vec4 color = mix(yellow, red, smoothstep(step1, step2, dist));\n"
                                         "color = mix(color, yellow, smoothstep(step2, step3, dist));\n"
                                         "if(isZapper == 1)\n"
                                         "{\n"
                                         " FragColor = color;\n"
                                         "}\n"
                                         "else{\n"
                                         "FragColor =  temp;}\n"
                                         "}\n\0";
const char *player2VertexShaderSource = "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "layout (location = 1) in vec3 aColor;\n"
                                        "layout (location = 2) in vec2 aTexCoord;\n"
                                        "uniform vec4 offSet;\n"
                                        "uniform mat4 transform;\n"
                                        "out vec3 ourColor;\n"
                                        "out vec2 TexCoord;\n"
                                        "void main()\n"
                                        "{\n"
                                        "   gl_Position = (transform * vec4(aPos,1.0f)) + offSet;\n"
                                        "   ourColor = aColor;\n"
                                        "   TexCoord = aTexCoord;\n"
                                        "}\n\0";
const char *player2FragmentShaderSource = "#version 330 core\n"
                                          "out vec4 FragColor;\n"
                                          "in vec3 ourColor;\n"
                                          "in vec2 TexCoord;\n"
                                          "uniform sampler2D ourTexture;\n"
                                          "uniform int isZapper;\n"
                                          "void main()\n"
                                          "{\n"
                                          "   vec4 temp = texture(ourTexture, TexCoord);\n"
                                          "   if (temp.a < 1)\n"
                                          "{\n"
                                          "            discard;\n"
                                          "}\n"
                                          "float dist = TexCoord.y;\n"
                                          "vec4 yellow = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                          "vec4 red = vec4(1.0f, 0.0f,0.0f, 1.0f);\n"
                                          "float step1 = 0.0f; \n"
                                          "float step2 = 0.5f;\n"
                                          "float step3 = 1.0f;\n"

                                          "vec4 color = mix(yellow, red, smoothstep(step1, step2, dist));\n"
                                          "color = mix(color, yellow, smoothstep(step2, step3, dist));\n"
                                          "if(isZapper == 1)\n"
                                          "{\n"
                                          " FragColor = color;\n"
                                          "}\n"
                                          "else{\n"
                                          "FragColor =  vec4(0.0f,0.0f,0.0f,0.0f);}\n"
                                          "}\n\0";
const char *backGroundVertexShaderSource = "#version 330 core\n"
                                           "layout (location = 0) in vec3 aPos;\n"
                                           "layout (location = 1) in vec3 aColor;\n"
                                           "layout (location = 2) in vec2 aTexCoord;\n"
                                           "uniform vec4 offSet;\n"
                                           "out vec3 ourColor;\n"
                                           "out vec2 TexCoord;\n"
                                           "void main()\n"
                                           "{\n"
                                           "   gl_Position = vec4(aPos.x - offSet.x , aPos.yz,1.0f);\n"
                                           "   ourColor = aColor;\n"
                                           "   TexCoord = aTexCoord;\n"
                                           "}\n\0";
const char *backGroundFragmentShaderSource = "#version 330 core\n"
                                             "out vec4 FragColor;\n"
                                             "in vec3 ourColor;\n"
                                             "in vec2 TexCoord;\n"
                                             "uniform sampler2D ourTexture;\n"
                                             "void main()\n"
                                             "{\n"
                                             "   FragColor = texture(ourTexture, TexCoord);\n"
                                             "}\n\0";
const char *zapperVertexShaderSource = "#version 330 core\n"
                                       "layout (location = 0) in vec3 aPos;\n"
                                       "layout (location = 1) in vec3 aColor;\n"
                                       "layout (location = 2) in vec2 aTexCoord;\n"
                                       "uniform vec4 offSet;\n"
                                       "out vec3 ourColor;\n"
                                       "out vec2 TexCoord;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   gl_Position = vec4(aPos.x - offSet.x,aPos.yz,1.0f);\n"
                                       "   ourColor = aColor;\n"
                                       "   TexCoord = aTexCoord;\n"
                                       "}\n\0";
const char *zapperFragmentShaderSource = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         "in vec3 ourColor;\n"
                                         "in vec2 TexCoord;\n"
                                         "uniform sampler2D ourTexture;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   vec4 temp = texture(ourTexture, TexCoord);\n"
                                         "   if (temp.a < 1)\n"
                                         "{\n"
                                         "            discard;\n"
                                         "}\n"
                                         "float dist = TexCoord.y;\n"
                                         "vec4 yellow = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                         "vec4 red = vec4(1.0f, 0.0f,0.0f, 1.0f);\n"
                                         "float step1 = 0.0f; \n"
                                         "float step2 = 0.5f;\n"
                                         "float step3 = 1.0f;\n"

                                         "vec4 color = mix(yellow, red, smoothstep(step1, step2, dist));\n"
                                         "color = mix(color, yellow, smoothstep(step2, step3, dist));\n"
                                         " FragColor = color;\n"
                                         "}\n\0";
const char *zapper2VertexShaderSource = "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "layout (location = 1) in vec3 aColor;\n"
                                        "layout (location = 2) in vec2 aTexCoord;\n"
                                        "uniform vec4 offSet;\n"
                                        "out vec3 ourColor;\n"
                                        "out vec2 TexCoord;\n"
                                        "void main()\n"
                                        "{\n"
                                        "   gl_Position = vec4(aPos.x - offSet.x,aPos.yz,1.0f);\n"
                                        "   ourColor = aColor;\n"
                                        "   TexCoord = aTexCoord;\n"
                                        "}\n\0";
const char *zapper2FragmentShaderSource = "#version 330 core\n"
                                          "out vec4 FragColor;\n"
                                          "in vec3 ourColor;\n"
                                          "in vec2 TexCoord;\n"
                                          "uniform sampler2D ourTexture;\n"
                                          "void main()\n"
                                          "{\n"
                                          "   vec4 temp = texture(ourTexture, TexCoord);\n"
                                          "   if (temp.a < 1)\n"
                                          "{\n"
                                          "            discard;\n"
                                          "}\n"
                                          "float dist = TexCoord.y;\n"
                                          "vec4 yellow = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                          "vec4 red = vec4(1.0f, 0.0f,0.0f, 1.0f);\n"
                                          "float step1 = 0.0f; \n"
                                          "float step2 = 0.5f;\n"
                                          "float step3 = 1.0f;\n"
                                          "vec4 color = mix(yellow, red, smoothstep(step1, step2, dist));\n"
                                          "color = mix(color, yellow, smoothstep(step2, step3, dist));\n"
                                          " FragColor = color;\n"
                                          "}\n\0";
const char *coinVertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "layout (location = 1) in vec3 aColor;\n"
                                     "layout (location = 2) in vec2 aTexCoord;\n"
                                     "uniform vec4 offSet;\n"
                                     "out vec3 ourColor;\n"
                                     "out vec2 TexCoord;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x - offSet.x,aPos.yz,1.0f);\n"
                                     "   ourColor = aColor;\n"
                                     "   TexCoord = aTexCoord;\n"
                                     "}\n\0";
const char *coinFragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "in vec3 ourColor;\n"
                                       "in vec2 TexCoord;\n"
                                       "uniform sampler2D ourTexture;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = texture(ourTexture, TexCoord);\n"
                                       "   if (FragColor.a < 1)\n"
                                       "{\n"
                                       "            discard;\n"
                                       "}\n"
                                       "}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Rendering the Player Shader Program ----------------------------------------------------------------------------------------
    unsigned int playervertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(playervertexShader, 1, &playerVertexShaderSource, NULL);
    glCompileShader(playervertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(playervertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(playervertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int playerfragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(playerfragmentShader, 1, &playerFragmentShaderSource, NULL);
    glCompileShader(playerfragmentShader);

    glGetShaderiv(playerfragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(playerfragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int playershaderProgram = glCreateProgram();
    glAttachShader(playershaderProgram, playervertexShader);
    glAttachShader(playershaderProgram, playerfragmentShader);
    glLinkProgram(playershaderProgram);

    glGetProgramiv(playershaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(playershaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(playervertexShader);
    glDeleteShader(playerfragmentShader);

    // Rendering the Player ----------------------------------------------------------------------------------------
    float playerVertices[] = {
        // positions            // colors           // texture coords
        0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    unsigned int playerIndices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    unsigned int VBO[8], VAO[8], EBO[8];
    glGenVertexArrays(1, &VAO[0]);
    glGenBuffers(1, &VBO[0]);
    glGenBuffers(1, &EBO[0]);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(playerIndices), playerIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Player Texture ----------------------------------------------------------------------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("images/player.jpeg", &width, &height, &nrChannels, 0);

    if (data)
    {
        if (nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Rendering the Second Player Shader Program ------------------------------------------
    unsigned int player2vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(player2vertexShader, 1, &player2VertexShaderSource, NULL);
    glCompileShader(player2vertexShader);

    glGetShaderiv(player2vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(player2vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int player2fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(player2fragmentShader, 1, &player2FragmentShaderSource, NULL);
    glCompileShader(player2fragmentShader);

    glGetShaderiv(player2fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(player2fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int player2shaderProgram = glCreateProgram();
    glAttachShader(player2shaderProgram, player2vertexShader);
    glAttachShader(player2shaderProgram, player2fragmentShader);
    glLinkProgram(player2shaderProgram);

    glGetProgramiv(player2shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(player2shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(player2vertexShader);
    glDeleteShader(player2fragmentShader);

    // Rendering the 2nd Player ----------------------------------------------------------------------------------------
    float player2Vertices[] = {
        // positions            // colors           // texture coords
        0.30f, 0.30f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.30f, -0.30f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.30f, -0.30f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.30f, 0.30f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    unsigned int player2Indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    glGenVertexArrays(1, &VAO[6]);
    glGenBuffers(1, &VBO[6]);
    glGenBuffers(1, &EBO[6]);

    glBindVertexArray(VAO[6]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(player2Vertices), player2Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[6]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(player2Indices), player2Indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Player 2 Texture ----------------------------------------------------------------------------------------
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width2, height2, nrChannels2;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data2 = stbi_load("images/player.jpeg", &width2, &height2, &nrChannels2, 0);

    if (data2)
    {
        if (nrChannels2 == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);

    // Rendering the Background Shader Program ----------------------------------------------------------------------------------------
    unsigned int backGroundvertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(backGroundvertexShader, 1, &backGroundVertexShaderSource, NULL);
    glCompileShader(backGroundvertexShader);

    glGetShaderiv(backGroundvertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(backGroundvertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int backGroundfragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(backGroundfragmentShader, 1, &backGroundFragmentShaderSource, NULL);
    glCompileShader(backGroundfragmentShader);

    glGetShaderiv(backGroundfragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(backGroundfragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int backGroundshaderProgram = glCreateProgram();
    glAttachShader(backGroundshaderProgram, backGroundvertexShader);
    glAttachShader(backGroundshaderProgram, backGroundfragmentShader);
    glLinkProgram(backGroundshaderProgram);

    glGetProgramiv(backGroundshaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(backGroundshaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(backGroundvertexShader);
    glDeleteShader(backGroundfragmentShader);

    // Rendering the Background ----------------------------------------------------------------------------------------
    float backgroundVertices[] = {
        // positions            // colors           // texture coords
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    unsigned int backgroundIndices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    glGenVertexArrays(1, &VAO[1]);
    glGenBuffers(1, &VBO[1]);
    glGenBuffers(1, &EBO[1]);

    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), backgroundVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(backgroundIndices), backgroundIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Background Texture ----------------------------------------------------------------------------------------
    unsigned int backGroundtexture;
    glGenTextures(1, &backGroundtexture);
    glBindTexture(GL_TEXTURE_2D, backGroundtexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int backGroundwidth, backGroundheight, backGroundnrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *backGrounddata = stbi_load("images/BackGround.jpeg", &backGroundwidth, &backGroundheight, &backGroundnrChannels, 0);

    if (backGrounddata)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, backGroundwidth, backGroundheight, 0, GL_RGB, GL_UNSIGNED_BYTE, backGrounddata);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(backGrounddata);

    // Rendering the Background Second Time ----------------------------------------------------------------------------------------
    float backgroundVerticesTwice[] = {
        // positions            // colors           // texture coords
        2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    glGenVertexArrays(1, &VAO[2]);
    glGenBuffers(1, &VBO[2]);
    glGenBuffers(1, &EBO[2]);

    glBindVertexArray(VAO[2]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVerticesTwice), backgroundVerticesTwice, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(backgroundIndices), backgroundIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Rendering the Background Third Time ----------------------------------------------------------------------------------------
    float backgroundVerticesThrice[] = {
        // positions            // colors           // texture coords
        3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    glGenVertexArrays(1, &VAO[3]);
    glGenBuffers(1, &VBO[3]);
    glGenBuffers(1, &EBO[3]);

    glBindVertexArray(VAO[3]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVerticesThrice), backgroundVerticesThrice, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(backgroundIndices), backgroundIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Rendering the Zappers Shader Program ----------------------------------------------------------------------------------------
    unsigned int zappervertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(zappervertexShader, 1, &zapperVertexShaderSource, NULL);
    glCompileShader(zappervertexShader);

    glGetShaderiv(zappervertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(zappervertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int zapperfragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(zapperfragmentShader, 1, &zapperFragmentShaderSource, NULL);
    glCompileShader(zapperfragmentShader);

    glGetShaderiv(zapperfragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(zapperfragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int zappershaderProgram = glCreateProgram();
    glAttachShader(zappershaderProgram, zappervertexShader);
    glAttachShader(zappershaderProgram, zapperfragmentShader);
    glLinkProgram(zappershaderProgram);

    glGetProgramiv(zappershaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(zappershaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(zappervertexShader);
    glDeleteShader(zapperfragmentShader);

    // Rendering the Zapper ----------------------------------------------------------------------------------------
    float zapperVertices[] = {
        // First Zapper
        // Positions            // Colors           // Texture Coordinates
        0.9f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        0.9f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        0.7f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        0.7f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // top left
        // Second Zapper
        0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        0.3f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        0.1f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // top left
        // Third Zapper
        1.9f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        1.9f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        1.7f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        1.7f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // top left
        // Fourth Zapper
        2.6f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        2.6f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        2.4f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        2.4f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    unsigned int zapperIndices[] = {
        0, 1, 3,    // first Triangle
        1, 2, 3,    // second Triangle
        4, 5, 7,    // first Triangle
        5, 6, 7,    // second Triangle
        8, 9, 11,   // first Triangle
        9, 10, 11,  // second Triangle
        12, 13, 15, // first Triangle
        13, 14, 15  // second Triangle
    };
    glGenVertexArrays(1, &VAO[4]);
    glGenBuffers(1, &VBO[4]);
    glGenBuffers(1, &EBO[4]);

    glBindVertexArray(VAO[4]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zapperVertices), zapperVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(zapperIndices), zapperIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Zapper Texture ----------------------------------------------------------------------------------------
    unsigned int zappertexture;
    glGenTextures(1, &zappertexture);
    glBindTexture(GL_TEXTURE_2D, zappertexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int zapperwidth, zapperheight, zappernrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *zapperdata = stbi_load("images/wood.jpeg", &zapperwidth, &zapperheight, &zappernrChannels, 0);

    if (zapperdata)
    {
        if (zappernrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, zapperwidth, zapperheight, 0, GL_RGB, GL_UNSIGNED_BYTE, zapperdata);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, zapperwidth, zapperheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, zapperdata);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(zapperdata);

    // Rendering the Zappers Type-2 Shader Program ----------------------------------------------------------------------------------------
    unsigned int zapper2vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(zapper2vertexShader, 1, &zapper2VertexShaderSource, NULL);
    glCompileShader(zapper2vertexShader);

    glGetShaderiv(zapper2vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(zapper2vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int zapper2fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(zapper2fragmentShader, 1, &zapper2FragmentShaderSource, NULL);
    glCompileShader(zapper2fragmentShader);

    glGetShaderiv(zapper2fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(zapper2fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int zapper2shaderProgram = glCreateProgram();
    glAttachShader(zapper2shaderProgram, zapper2vertexShader);
    glAttachShader(zapper2shaderProgram, zapper2fragmentShader);
    glLinkProgram(zapper2shaderProgram);

    glGetProgramiv(zapper2shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(zapper2shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(zapper2vertexShader);
    glDeleteShader(zapper2fragmentShader);

    // Rendering the Zapper ----------------------------------------------------------------------------------------
    float zapper2Vertices[] = {
        // First Zapper
        // Positions            // Colors           // Texture Coordinates
        0.1f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        0.1f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.1f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.1f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top left
        // Second Zapper
        1.9f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        1.9f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        1.7f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        1.7f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top left
        // Third Zapper
        1.1f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        1.1f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        0.9f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        0.9f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top left
        // Fourth Zapper
        3.1f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        3.1f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        2.9f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        2.9f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top left
    };

    unsigned int zapper2Indices[] = {
        0, 1, 3,    // first Triangle
        1, 2, 3,    // second Triangle
        4, 5, 7,    // first Triangle
        5, 6, 7,    // second Triangle
        8, 9, 11,   // first Triangle
        9, 10, 11,  // second Triangle
        12, 13, 15, // first Triangle
        13, 14, 15  // second Triangle
    };
    glGenVertexArrays(1, &VAO[5]);
    glGenBuffers(1, &VBO[5]);
    glGenBuffers(1, &EBO[5]);

    glBindVertexArray(VAO[5]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zapper2Vertices), zapper2Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(zapper2Indices), zapper2Indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Zapper Texture ----------------------------------------------------------------------------------------
    unsigned int zapper2texture;
    glGenTextures(1, &zapper2texture);
    glBindTexture(GL_TEXTURE_2D, zapper2texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int zapper2width, zapper2height, zapper2nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *zapper2data = stbi_load("images/metal.jpeg", &zapper2width, &zapper2height, &zapper2nrChannels, 0);

    if (zapper2data)
    {
        if (zapper2nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, zapper2width, zapper2height, 0, GL_RGB, GL_UNSIGNED_BYTE, zapper2data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, zapper2width, zapper2height, 0, GL_RGBA, GL_UNSIGNED_BYTE, zapper2data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(zapper2data);

    // Rendering the Coins Shader Program ----------------------------------------------------------------------------------------
    unsigned int coinvertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(coinvertexShader, 1, &coinVertexShaderSource, NULL);
    glCompileShader(coinvertexShader);

    glGetShaderiv(coinvertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(coinvertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int coinfragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(coinfragmentShader, 1, &coinFragmentShaderSource, NULL);
    glCompileShader(coinfragmentShader);

    glGetShaderiv(coinfragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(coinfragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int coinshaderProgram = glCreateProgram();
    glAttachShader(coinshaderProgram, coinvertexShader);
    glAttachShader(coinshaderProgram, coinfragmentShader);
    glLinkProgram(coinshaderProgram);

    glGetProgramiv(coinshaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(coinshaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(coinvertexShader);
    glDeleteShader(coinfragmentShader);

    // Rendering the Coins ----------------------------------------------------------------------------------------
    float coinVertices[] = {
        // First Coin
        // Positions            // Colors           // Texture Coordinates
        2.1f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        2.1f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        1.9f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        1.9f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top left
    };

    unsigned int coinIndices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3, // second Triangle
    };
    glGenVertexArrays(1, &VAO[7]);
    glGenBuffers(1, &VBO[7]);
    glGenBuffers(1, &EBO[7]);

    glBindVertexArray(VAO[7]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coinVertices), coinVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[7]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(coinIndices), coinIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Coins Texture ----------------------------------------------------------------------------------------
    unsigned int cointexture;
    glGenTextures(1, &cointexture);
    glBindTexture(GL_TEXTURE_2D, cointexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int coinwidth, coinheight, coinnrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *coindata = stbi_load("images/coin.jpeg", &coinwidth, &coinheight, &coinnrChannels, 0);

    if (coindata)
    {
        if (coinnrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, coinwidth, coinwidth, 0, GL_RGB, GL_UNSIGNED_BYTE, coindata);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, coinwidth, coinwidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, coindata);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(coindata);

    // Movement Management ----------------------------------------------------------------------------------------
    x = 0;
    y = 0;

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
    trans = glm::translate(trans, glm::vec3(-1.5f, 0.0f, 0.0f));

    float backGroundOffSet = -0.00099;
    float zapperOffSet = 0;
    float coinOffSet = 0;

    // Render Loop ----------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        srand(time(0));
        int randomNumber = rand() % 2;

        Levels();
        winGame(window);

        score += 0.1;

        // Coin Collisions
        float coin1Collision = 1.9 - coinOffSet;
        if (coin1Collision >= -0.9 && coin1Collision <= -0.7)
        {
            if (y >= 0.5 && y <= 0.7)
            {
                coinOffSet = -1 + randomNumber;
                coinCollected++;
            }
        }

        // Zapper Collisions
        float zapper1Collision = 0.1 - zapperOffSet;
        if (zapper1Collision >= -0.9 && zapper1Collision <= -0.7)
        {
            if (y >= -0.5 && y <= 0.0)
            {
                lostGame(window);
            }
        }

        float zapper2Collision = 0.7 - zapperOffSet;
        if (zapper2Collision >= -0.9 && zapper2Collision <= -0.7)
        {
            if (y >= -0.5 && y <= 0.1)
            {
                lostGame(window);
            }
        }

        float zapper3Collision = 1.7 - zapperOffSet;
        if (zapper3Collision >= -0.9 && zapper3Collision <= -0.7)
        {
            if (y >= -0.5 && y <= 0.1)
            {
                lostGame(window);
            }
            if (y >= 0.8 && y <= 1.0)
            {
                lostGame(window);
            }
        }

        float zapper4Collision = 2.4 - zapperOffSet;
        if (zapper4Collision >= -0.9 && zapper4Collision <= -0.7)
        {
            if (y >= -0.5 && y <= 0.5)
            {
                lostGame(window);
            }
        }

        float zapper5Collision = -0.1 - zapperOffSet;
        if (zapper5Collision >= -0.9 && zapper5Collision <= -0.7)
        {
            if (y >= 0.8 && y <= 1.0)
            {
                lostGame(window);
            }
        }

        float zapper6Collision = 0.9 - zapperOffSet;
        if (zapper6Collision >= -0.9 && zapper6Collision <= -0.7)
        {
            if (y >= 0.8 && y <= 1.0)
            {
                lostGame(window);
            }
        }

        float zapper7Collision = 2.9 - zapperOffSet;
        if (zapper7Collision >= -0.9 && zapper7Collision <= -0.7)
        {
            if (y >= 0.8 && y <= 1.0)
            {
                lostGame(window);
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Rendering the BackGround
        backGroundOffSet += 0.005;
        glUseProgram(backGroundshaderProgram);
        int backGround1OffsetLocation = glGetUniformLocation(backGroundshaderProgram, "offSet");
        glUniform4f(backGround1OffsetLocation, backGroundOffSet, 0.0f, 0.0f, 0.0f);

        if (backGroundOffSet > 2)
        {
            backGroundOffSet = -2.0;
        }

        glBindTexture(GL_TEXTURE_2D, backGroundtexture);
        // Rendering Background 1
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Rendering Background 2
        glBindVertexArray(VAO[2]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Rendering Background 3
        glBindVertexArray(VAO[3]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Rendering the Zapper
        glUseProgram(zappershaderProgram);
        glBindTexture(GL_TEXTURE_2D, zappertexture);
        int zapperOffsetLocation = glGetUniformLocation(zappershaderProgram, "offSet");
        zapperOffSet += 0.005;
        glUniform4f(zapperOffsetLocation, zapperOffSet, 0.0f, 0.0f, 0.0f);
        glBindVertexArray(VAO[4]);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

        if (zapperOffSet > 4)
        {
            zapperOffSet = -1.0;
        }

        // Rendering the Second Zapper
        glUseProgram(zapper2shaderProgram);
        glBindTexture(GL_TEXTURE_2D, zapper2texture);
        int zapper2OffsetLocation = glGetUniformLocation(zapper2shaderProgram, "offSet");
        glUniform4f(zapper2OffsetLocation, zapperOffSet, 0.0f, 0.0f, 0.0f);
        glBindVertexArray(VAO[5]);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

        // Rendering the Coin
        glUseProgram(coinshaderProgram);
        glBindTexture(GL_TEXTURE_2D, cointexture);
        int coinOffsetLocation = glGetUniformLocation(coinshaderProgram, "offSet");
        coinOffSet += 0.005;
        glUniform4f(coinOffsetLocation, coinOffSet, 0.0f, 0.0f, 0.0f);
        glBindVertexArray(VAO[7]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (coinOffSet > 4)
        {
            coinOffSet = -1 + randomNumber;
        }

        // Rendering the 2nd Player
        glUseProgram(player2shaderProgram);
        int vertex2OffsetLocation = glGetUniformLocation(player2shaderProgram, "offSet");
        unsigned int transform2Loc = glGetUniformLocation(player2shaderProgram, "transform");
        int glowPlayer2Location = glGetUniformLocation(player2shaderProgram, "isZapper");
        if (flying == 1)
        {
            glUniform1i(glowPlayer2Location, 1);
        }
        else
        {
            glUniform1i(glowPlayer2Location, 0);
        }
        glUniformMatrix4fv(transform2Loc, 1, GL_FALSE, glm::value_ptr(trans));
        glUniform4f(vertex2OffsetLocation, x, y, 0.0f, 0.0f);

        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO[6]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Rendering the Player
        glUseProgram(playershaderProgram);
        int vertexOffsetLocation = glGetUniformLocation(playershaderProgram, "offSet");
        unsigned int transformLoc = glGetUniformLocation(playershaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glUniform4f(vertexOffsetLocation, x, y, 0.0f, 0.0f);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        y -= 0.01;
        y = my_max(y, -0.40);
        flying = 0;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, &VAO[0]);
    glDeleteBuffers(2, &VBO[0]);
    glDeleteBuffers(2, &EBO[0]);
    glDeleteProgram(playershaderProgram);
    glDeleteProgram(backGroundshaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        y += 0.02;
        y = my_min(y, 0.90);
        flying = 1;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}