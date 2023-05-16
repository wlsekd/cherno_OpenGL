/*
* 绘制流程：
* 绑定着色器-》绑定顶点缓冲区-》设置顶点布局-》绑定索引缓冲区-》发出绘制调用
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "shader.h"

/*
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

//无法看到编译器报错的行号
static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE=-1,VERTEX=0,FRAGMENT=1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(),ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        //栈内分配内存，自动释放不需要free
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

//传入参数 顶点着色器和像素着色器
//unsigned int等可以替换为opengl定义的参数类型
static unsigned int CreateShader(const std::string& veretxShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, veretxShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
*/
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //核心配置文件创建窗口,需要创建一个顶点数组，兼容配置文件有默认的直接使用
    // 总有一个顶点数组被绑定，如何绑定顶点缓冲区且指定顶点布局后再绘制
    // 两个选择：忽略顶点数组绘制或者为每个图形创建顶点数组，
    // 即为整个程序创建一个全局vao绑定不同的缓冲区和顶点规格或者为每一个图形都创建vao
    // 以上需要根据实际情况选择
    // NVDIA的论文讨论过关于此问题
    // vao在底层占用更多的显存，并且效率不如一个一个绑定，但现在的条件不需要考虑太多，直接使用vao加快代码速度
    // 根据实际情况对vao的使用做测试再进行选择
    //opengl主版本为3，次版本为3，即opengl3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //设置帧率
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    /*加大括号的原因：
    * 结构函数里的GLCall在尝试通过循环清空错误码，而解构函数是main返回时调用的
    * 因为过早关闭窗口，是因为glfwTerminate()之后glGetError()会返回错误码
    * 也就是glfwTerminate()之后调用的，所以导致无法情况错误码从而陷入死循环
    * 花括号限定vb和ib对象作用域之后，结构函数会在离开这个作用域之前被调用，页就是glfwTerminate之前*/
    {
        //坐标数据
        float positions[] = {
            -0.5f, -0.5f, // 0
             0.5f, -0.5f, // 1
             0.5f,  0.5f, // 2
            -0.5f,  0.5f, // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };
        /*
        //保存实际的顶点数组对象ID
        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        //生成顶点阵列
        GLCall(glBindVertexArray(vao));
        */
        
        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);//GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
        
        va.AddBuffer(vb, layout);
        /*以下抽象为类
        * unsigned int buffer;
        //创建缓冲区，返回buffer用于操作缓冲区
        GLCall(glGenBuffers(1, &buffer));
        //绑定缓冲区，设置Array
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
        //添加缓冲区数据
        //下面代码绑定了buffer和vao
        GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

        */
        /*最主流的两个：顶点着色器和像素/片段着色器
        */
        //GLCall(glEnableVertexAttribArray(0));
        //索引，坐标，数据类型，标准化，大小，指针（偏移量，理解指针即数字）
        //GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

        IndexBuffer ib(indices, 6);
        /*该内容抽象为类
        unsigned int ibo;
        GLCall(glGenBuffers(1, &ibo));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));
        */

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        /*
        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
        //使用着色器
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));
        */
        /*
        //设置shader的uniform
        GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);
        GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));
        */
        

        //解绑
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;
        //glBindBuffer(GL_ARRAY_BUFFER, 0);未绑定数据，类似于没有选择响应图层所以无法绘制
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            //glClear(GL_COLOR_BUFFER_BIT);
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
            /*
            //渲染时再绑定
            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
            */
            //代码将顶点数组和顶点缓冲连接起来，不需要顶点缓冲即可绘制图形
            //GLCall(glBindVertexArray(vao));
            //va.Bind();
            /*有了vao以后，可以不用声明下面这些*/
                /*GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
                GLCall(glEnableVertexAttribArray(0));
                GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));*/
                //只需要绑定vao和ibo
                    //GLCall(glBindVertexArray(vao));
                    /*GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));*/
            //ib.Bind();


            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            //清空其他函数的错误
            //GLClearError();
            //绘制Array中的图形
            //坐标方式glDrawArrays(GL_TRIANGLES, 0, 6);
            //索引方式
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            //获取上面函数的错误
            //ASSERT(GLLogCall());

            renderer.Draw(va, ib, shader);
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}