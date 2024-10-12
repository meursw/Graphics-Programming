//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <iostream>
//
//#include "../shader.h"
//
//#define STB_IMAGE_IMPLEMENTATION
//#include "../stb_image.h"
//
//GLFWwindow* init_window();
//
//void framebuffer_size_callback(GLFWwindow* win, int width, int height);  // callback function that gets called when resizing glfw window.
//void processInput(GLFWwindow* win, Shader&);
//
//float vertices[] = {
//     positions          // colors           // texture coords
//     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
//     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
//    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
//    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
//};
//
//unsigned int indices[] = {
//    0, 1, 3, // first triangle
//    3, 1, 2  // second triangle
//};
//
//float mix{ 0.0 };
//
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//int main() {
//
//    GLFWwindow* window = init_window();
//
//    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // when the window gets resized, call the framebuffer_size_callback (user defined).
//
//    unsigned int VAO, VBO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER,VBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//    
//    Shader shaderProgram("Hello Textures/texture_shader.vs", "Hello Textures/texture_shader.fs");
//
//     position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);                   
//     color attribute                              
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//     texture attribute
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//    glEnableVertexAttribArray(2);
//
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//    stbi_set_flip_vertically_on_load(true);
//
//    unsigned int texture, texture2;
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
//     set the texture wrapping parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // note that we set the container wrapping method to GL_CLAMP_TO_EDGE
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//     set texture filtering parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//    int width, height, nrChannels;
//    unsigned char* data = stbi_load("Assets/container.jpg", &width, &height, &nrChannels, 0);
//    if (data) {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else {
//        std::cout << "Failed to load texture" << std::endl;
//    }
//    stbi_image_free(data);
//
//    glGenTextures(1, &texture2);
//    glBindTexture(GL_TEXTURE_2D, texture2);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     set texture filtering parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        
//    data = stbi_load("Assets/awesomeface.png", &width, &height, &nrChannels, 0);
//    if (data) {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    stbi_image_free(data);
//
//
//    shaderProgram.use();
//    glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture"), 0);
//    shaderProgram.setInt("texture2", 1);
//
//     uncomment this call to draw in wireframe polygons.
//     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//   
//    /*
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    */
//
//     render loop
//    while (!glfwWindowShouldClose(window)) {
//        processInput(window, shaderProgram);
//
//        glClearColor(0.254f, 0.135f, 0.203f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, texture);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, texture2);
//
//        glUniform1f(glGetUniformLocation(shaderProgram.ID, "u_mix"), mix);
//        shaderProgram.setFloat("u_mix", mix);
//        shaderProgram.use();
//        glBindVertexArray(VAO);
//
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
//        glfwPollEvents();
//        glfwSwapBuffers(window);
//    }
//
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    shaderProgram.m_delete();
//
//    glfwTerminate();
//    return 0;
//}
//
//GLFWwindow* init_window() {
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        std::exit(-1);
//    }
//    glfwMakeContextCurrent(window);
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // load GLAD
//        std::cout << "Failed to init GLAD" << std::endl;
//        std::exit(-1);
//    }
//
//    return window;
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//}
//
//void processInput(GLFWwindow* window, Shader &shaderProgram) {
//    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
//        float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//        float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//
//        glClearColor(r, g, b, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//    }
//    
//    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
//        mix += 0.05f;
//        if (mix > 1.0f)
//            mix = 1.0f;
//    }
//
//    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
//        mix -= 0.05f;
//        if (mix < 0.0f)
//            mix = 0.0f;
//    }
//
//    mix = sin(4.0*glfwGetTime()) * 0.5 + 0.5;
//    
//}