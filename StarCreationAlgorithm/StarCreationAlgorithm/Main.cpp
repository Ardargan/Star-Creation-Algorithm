#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<fstream>
#include<sstream>
#include<glm/vec3.hpp>
#include<math.h>
#include<iostream>
#include<algorithm>
//7 4 2024

GLfloat verticies[5005];
GLuint indicies[2002];

void DrawStar(int pointCount, float radius, int skipNumber)
{
	verticies[0] = cos((360.0f / pointCount * 3.141592f / 180.0f * 0) + (90 * 3.141592f / 180.0f)) * radius;
	verticies[1] = sin((360.0f / pointCount * 3.141592f / 180.0f * 0) + (90 * 3.141592f / 180.0f));
	verticies[2] = 0.8 + sin(0) * radius;
	verticies[3] = 0.8 + cos(0) * radius;
	verticies[4] = 0.8 + tan(0) * radius;
	indicies[0] = 0;
	indicies[1] = 1 * skipNumber;
	for (int i = 1; i < pointCount + 1; i++)
	{
		verticies[5 * i] = cos((360.0f / pointCount * 3.141592f / 180.0f * i) + (90 * 3.141592f / 180.0f)) * radius;
		verticies[5 * i + 1] = sin((360.0f / pointCount * 3.141592f / 180.0f * i) + (90 * 3.141592f / 180.0f)) * radius;
		verticies[5 * i + 2] = 0.8 + sin((360.0f / pointCount * 3.141592f / 180.0f * i) + (90 * 3.141592f / 180.0f)) ;
		verticies[5 * i + 3] = 0.8 + cos((360.0f / pointCount * 3.141592f / 180.0f * i) + (90 * 3.141592f / 180.0f)) ;
		verticies[5 * i + 4] = 0.8 + tan((360.0f / pointCount * 3.141592f / 180.0f * i) + (90 * 3.141592f / 180.0f)) ;
		indicies[2 * i] = (i * skipNumber) % pointCount;
		indicies[2 * i + 1] = ((i + 1) * skipNumber) % pointCount;
	}

}

int main()
{
	while (1)
	{
		std::fill(verticies, verticies + 5005, 0);
		std::fill(indicies, indicies + 2002, 0);

		int a, b;
		std::cout << "Please enter the desired number of points on a circle(max. 1000): \n";
		std::cin >> a;
		std::cout << "Please enter the number of lines to be drawn between the points you selected earlier \n(Do not choose a number that exactly divides the selected points and bigger than half of the selected points; otherwise, you will not obtain a star): \n";
		std::cin >> b;

		DrawStar(a, 1.0f, b);

		glfwInit(); glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		GLFWwindow* window = glfwCreateWindow(1000, 1000, "StarAlgorithm", NULL, NULL);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
		gladLoadGL();
		glViewport(0, 0, 1000, 1000);

		std::ifstream VertexTxt("vertex.txt"); std::string VertexSource;
		VertexTxt.seekg(0, std::ios::end);
		VertexSource.resize(VertexTxt.tellg());
		VertexTxt.seekg(0, std::ios::beg);
		VertexTxt.read(&VertexSource[0], VertexSource.size());
		VertexTxt.close();
		std::ifstream FragmentTxt("fragment.txt"); std::string FragmentSource;
		FragmentTxt.seekg(0, std::ios::end);
		FragmentSource.resize(FragmentTxt.tellg());
		FragmentTxt.seekg(0, std::ios::beg);
		FragmentTxt.read(&FragmentSource[0], FragmentSource.size());
		FragmentTxt.close();

		const char* VertexContent = VertexSource.c_str(); const char* FragmentContent = FragmentSource.c_str();

		GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER); GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(VertexShader, 1, &VertexContent, NULL); glShaderSource(FragmentShader, 1, &FragmentContent, NULL);
		glCompileShader(VertexShader); glCompileShader(FragmentShader);
		GLuint ShaderProgram = glCreateProgram();
		glAttachShader(ShaderProgram, VertexShader); glAttachShader(ShaderProgram, FragmentShader);
		glLinkProgram(ShaderProgram);

		GLuint VertexArray, VertexBuffer, ElementBuffer;
		glGenVertexArrays(1, &VertexArray);
		glBindVertexArray(VertexArray);
		glGenBuffers(1, &VertexBuffer); glGenBuffers(1, &ElementBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glLineWidth(5.0f);
		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.0f, 0.00f, 0.00f, 1.0f); glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(ShaderProgram);
			glBindVertexArray(VertexArray);

			glDrawElements(GL_LINES, sizeof(indicies) / sizeof(int), GL_UNSIGNED_INT, 0);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		glDeleteProgram(ShaderProgram);
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	return 0;
}
