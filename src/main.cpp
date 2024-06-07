#include "config.h"
#include "triangle_mesh.h"

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);

unsigned int make_module(const std::string& filepath, unsigned int module_type);

int main() {
    
    //Make a window
    GLFWwindow* window;

    if (!glfwInit()) {
        std::cout << "GLFW couldn't start" << std::endl;
        return -1;
    }

    //Set window to current open window that will be rendered to; set parameters (size, etc) 
    window = glfwCreateWindow(640, 480, "My Window", NULL, NULL);
    glfwMakeContextCurrent(window);

    //Run through system and find all glad function definitions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        return -1;
    }


    //WINDOW COLOR!!!!!!
    glClearColor(0.25f, 0.15f, 0.27f, 1.0f);


    unsigned int shader = make_shader(
        "../../shaders/vertex.txt",
        "../../shaders/fragment.txt"
    );
    
    TriangleMesh t;

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        //Clear the screen using the color defined above
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        t.draw();
        glfwSwapBuffers(window);

    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath) {

    std::vector<unsigned int> modules;
    modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

    //create the shader, attach the modules one by one, link it all together
    unsigned int shader = glCreateProgram();
    for (unsigned int shaderModule : modules) {
        glAttachShader(shader, shaderModule);
    }
    glLinkProgram(shader);

    //Chech if successful 
    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        char errorLog[1024];
        glGetProgramInfoLog(shader, 1024, NULL, errorLog);
        std::cout << "Shader Linking error:\n" << errorLog << std::endl;
    }

    //delete after, don't need it anymore 
    for (unsigned int shaderModule : modules) {
        glDeleteShader(shaderModule);
    }

    return shader;

}

//Takes a file path and makes a shader module
//"makes one mega string"
unsigned int make_module(const std::string& filepath, unsigned int module_type) {

    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    file.open(filepath);
    while (std::getline(file, line)) {
        bufferedLines << line << "\n";
    }
    std::string shaderSource = bufferedLines.str();
    const char* shaderSrc = shaderSource.c_str();
    bufferedLines.str("");
    file.close();

    //create shader module and attach some (multiple) source code to it  
    unsigned int shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    //check if successful 
    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if (!success) {
        char errorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
    }

    return shaderModule;
}