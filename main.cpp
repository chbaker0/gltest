#include "Includes.h"

#include "ContextWindow.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "ShaderProgramManager.hpp"
#include "StaticMesh.h"
#include "Light.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef DEBUG
void APIENTRY debugCallback(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar *message,
                            const void *userParam);
#endif

int main()
{
    using namespace std;

	ContextWindow win;

	win.setHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	win.setHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	win.setHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	win.setHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	win.setHint(GLFW_RESIZABLE, GL_FALSE);
	win.setHint(GLFW_SRGB_CAPABLE, GL_TRUE);
	#ifdef DEBUG
	win.setHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	#endif // DEBUG

	win.setDimensions(800, 800);
    win.setTitle("Test");
    win.create();
	win.makeCurrent();

    if(ogl_LoadFunctions() == ogl_LOAD_FAILED) return 1;

    glViewport(0, 0, 800, 800);

    ShaderProgramManager shaderProgramManager;
    globalProgramManager = &shaderProgramManager;

    #ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_CALLBACK_FUNCTION);
    glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, nullptr);
    #endif

	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

    StaticMesh testCube("resources/meshes/monkey.dae");

	std::string log;
	ShaderProgram *prog = globalProgramManager->createProgram("main");
	{
		Shader vert(GL_VERTEX_SHADER, ifstream("resources/shaders/main.vert.glsl"));
		cerr << vert.compile(&log) << endl;
		cerr << log << endl;

		Shader frag(GL_FRAGMENT_SHADER, ifstream("resources/shaders/main.frag.glsl"));
		cerr << frag.compile(&log) << endl;
		cerr << log << endl;

		prog->attachShader(vert);
		prog->attachShader(frag);
		cerr << prog->link(&log) << endl;
		cerr << log << endl;
		prog->detachShader(vert);
		prog->detachShader(frag);
	}

	Texture monkeyTexture(GL_TEXTURE_2D, "resources/textures/monkey_diffuse.png");
	monkeyTexture.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	monkeyTexture.setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	monkeyTexture.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	monkeyTexture.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    PointLight pointLightsWorldSpace[2] =
    {
        {{1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 5.0f}, 1.0f},
        {{1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, -2.0f, 2.0f}, 1.0f}
    };

	GLuint pointLightBlock;
	glGenBuffers(1, &pointLightBlock);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointLightBlock);
//	glBufferData(GL_SHADER_STORAGE_BUFFER,
//                 PointLight::calcArraySizeStd430(2),
//                 nullptr, )
	glBufferStorage(GL_SHADER_STORAGE_BUFFER,
                    PointLight::calcArraySizeStd430(2),
                    nullptr, GL_MAP_WRITE_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    double currentTime = 0.0f;
	while(!win.shouldClose())
	{
	    double lastTime = currentTime;
	    currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
	    // Clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Select shader program
		prog->use();

        // Calculate transformation matrices
		glm::mat4 cameraClipTransform = glm::perspective(70.0f, 1.0f, 1.0f, 50.0f);
		glm::mat4 modelWorldTransform(1.0f);
		glm::mat4 worldCameraTransform(1.0f);
        worldCameraTransform =
            glm::lookAt(glm::vec3{0.0f,-5.0f, 0.0f},
                                 {0.0f, 0.0f, 0.0f},
                                 {0.0f, 0.0f, 1.0f});

        // Update light positions
        pointLightsWorldSpace[0].position =
            glm::vec3(2.0 * glm::sin(currentTime),
                      2.0 * glm::cos(currentTime),
                      0.0f);

        // Write the light data to the shader storage buffer
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointLightBlock);
        void *buffer = glMapBuffer(GL_SHADER_STORAGE_BUFFER,
                                   GL_WRITE_ONLY);
        if(buffer == nullptr)
            return 1;

        std::size_t offset = 0;
        for(std::size_t i = 0; i < 2; ++i)
        {
            offset = pointLightsWorldSpace[i]
                     .writeStd430Transform(buffer, offset,
                                           PointLight::calcArraySizeStd430(2),
                                           worldCameraTransform);
        }
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pointLightBlock);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        // Send the model->camera and camera->clip transformation matrices to shader program
		glUniformMatrix4fv(1, 1, false, glm::value_ptr(cameraClipTransform));
		glUniformMatrix4fv(0, 1, false, glm::value_ptr(worldCameraTransform * modelWorldTransform));

        // Bind the texture and draw
        monkeyTexture.bindTo(0);
        testCube.bindVertexArray();
        glDrawElements(GL_TRIANGLES, testCube.getIndexCount(), GL_UNSIGNED_SHORT, 0);

		win.swapBuffers();
		win.pollEvents();
	}
}

#ifdef DEBUG
void APIENTRY debugCallback(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar *message,
                            const void *userParam)
{
    using namespace std;

    cerr << "OpenGL debug\n";
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        cerr << "High severity ";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        cerr << "Medium severity ";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        cerr << "Low severity ";
        break;
    default:
        cerr << "Notification ";
        break;
    }
    switch(source)
    {
    case GL_DEBUG_SOURCE_API:
        cerr << "from API call: ";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        cerr << "from window system: ";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        cerr << "from shader compiler: ";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        cerr << "from user: ";
        break;
    default:
        cerr << "from unknown: ";
        break;
    }
    switch(type)
    {
    case GL_DEBUG_TYPE_ERROR:
        cerr << "Error\n";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        cerr << "Deprecated behavior\n";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        cerr << "Undefined behavior\n";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        cerr << "Unportable behavior\n";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        cerr << "Performance issue\n";
        break;
    case GL_DEBUG_TYPE_MARKER:
        cerr << "Marker\n";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        cerr << "Push group\n";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        cerr << "Pop group\n";
        break;
    default:
        cerr << "Unknown message\n";
        break;
    }
    cerr << "Text: \"" << message << "\"" << endl;
}
#endif
