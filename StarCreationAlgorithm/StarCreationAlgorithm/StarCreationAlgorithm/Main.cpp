#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<fstream>
#include<sstream>
#include<string>
#include<glm/vec3.hpp>
#include<math.h>
#include<chrono>
#include<thread>
#include<vector>
#include<iostream>
//7 4 2024
GLfloat verticies[5005];
GLuint indicies[5005];
std::vector<glm::vec3> noktaKonum;

void DrawStar(int pointCount, float radius, int skipNumber)
{
	verticies[0] = 1.0f * radius;
	verticies[1] = 0;
	verticies[2] = 0.3 + sin(0) * radius;
	verticies[3] = 0.3 + cos(0) * radius;
	verticies[4] = 0.3 + tan(0) * radius;
	indicies[0] = 0;
	indicies[1] = 1 * skipNumber;
	for (int i = 1; i < pointCount + 1; i++)
	{
		verticies[5 * i] = cos(360.0f / pointCount * 3.14f / 180.0f * i) * radius;
		verticies[5 * i + 1] = sin(360.0f / pointCount * 3.14f / 180.0f * i) * radius;
		verticies[5 * i + 2] = 0.5 + sin(360.0f / pointCount * 3.14f / 180.0f * i) * radius;
		verticies[5 * i + 3] = 0.5 + cos(360.0f / pointCount * 3.14f / 180.0f * i) * radius;
		verticies[5 * i + 4] = 0.5 + tan(360.0f / pointCount * 3.14f / 180.0f * i) * radius;
		indicies[2 * i] = (i * skipNumber) % pointCount;
		indicies[2 * i + 1] = ((i + 1) * skipNumber) % pointCount;
	}

}

int main()
{
	int pointCount, skipNumber;
	float radius;
	std::cout << "number of points(maks. 1000): \n";
	std::cin >> pointCount;
	std::cout << "point skip number(half of the point count and divides point count with remaining): \n";
	std::cin >> skipNumber;
	std::cout << "radius(min. -1 maks +1): \n";
	std::cin >> radius;
	DrawStar(pointCount, radius, skipNumber);

	glfwInit(); glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	noktaKonum.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.00f, 0.00f, 1.0f); glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(ShaderProgram);

		glBindVertexArray(VertexArray);

		glDrawElements(GL_LINES, sizeof(indicies) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}
