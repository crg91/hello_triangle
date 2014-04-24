/*
You are free to use this program anywhere!
Produced by Chirag Varde (twitter handle: _specular)
For any query mailto:chirag.varde.crg@gmail.com
*/

#include<iostream>
#include<GL/glew.h>
#include<GL/glut.h>
#include<fstream>

using namespace std;

#define WIDTH	512
#define HEIGHT	512

//global identifiers
GLuint program_id;
GLuint vbo_id;
GLuint index_buffer_id;

struct Vertex
{
	GLfloat pos[4];
	GLfloat col[4];
};

//array for position
struct Vertex vertices[3] = {
					{-0.5, -0.5, 0.0, 1.0,
					1.0, 0.0, 0.0, 0.0},
					{0.5, -0.5, 0.0, 1.0,
					0.0, 1.0, 0.0, 0.0},
					{0.0, 0.5, 0.0, 1.0,
					0.0, 0.0, 1.0, 0.0}
			};

GLuint indices[3] =	{
				0, 1, 2
			};
//function prototypes
void initialize();
void display();
void create_program();
void create_vbo();
void bind_shader_variables();

void initialize()
{
	//background color is dark yellow
	glClearColor(0.2, 0.2, 0.0, 0.0);

	//depth test is disabled by default
	glEnable(GL_DEPTH_TEST);

	//culling is disabled by default. Default front face is counter clock wise!!! By Default back face is culled
	glEnable(GL_CULL_FACE);

	create_program();
	create_vbo();
	bind_shader_variables();
}

//callback function for rendering
void display()
{
	glUseProgram(program_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
	glFlush();
	glutSwapBuffers();
}

void bind_shader_variables()
{
	glUseProgram(program_id);
	//attributes
	GLuint pos_id = glGetAttribLocation(program_id, "in_position");
	GLuint col_id = glGetAttribLocation(program_id, "in_color");
#if 0
	cout<<"pos_id = "<<pos_id<<endl;
	cout<<"col_id = "<<col_id<<endl;
#endif
	glVertexAttribPointer(pos_id, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, pos));
	glEnableVertexAttribArray(pos_id);	
	glVertexAttribPointer(col_id, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, col));
	glEnableVertexAttribArray(col_id);	
}

void create_vbo()
{
	glGenBuffers(1, &vbo_id);
#if 0
	cout<<"vbo_id = "<<vbo_id<<endl;
#endif
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct Vertex)*3, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &index_buffer_id);
#if 0
	cout<<"index_buffer_id = "<<index_buffer_id<<endl;
#endif
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*3, indices, GL_STATIC_DRAW);
}

void create_program()
{
	GLint status; //to check compile and linking status

	//compile vertex shader
	ifstream vertex_shader_file;
	int vertex_shader_length;
	vertex_shader_file.open("simple_shader.vert", ios::in);
	//go to end of the file to get total characters in the file
	vertex_shader_file.seekg(0, ios::end);
	vertex_shader_length = vertex_shader_file.tellg();
	vertex_shader_file.seekg(0, ios::beg);
	//allocated temporary character array for shader source
	char* vertex_shader_source = new char[vertex_shader_length + 1];//one extra character is for NULL
	vertex_shader_file.read(vertex_shader_source, vertex_shader_length);
	vertex_shader_source[vertex_shader_length] = '\0';
#if 0
	for(int i = 0; i<vertex_shader_length; i++)
		cout<<vertex_shader_source[i];
	cout<<endl;
#endif
	vertex_shader_file.close();
	GLuint vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, (const char **)&vertex_shader_source, NULL);
	delete[] vertex_shader_source;
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		cout<<"Vertex Shader did not compile\n";
		exit(0);
	}

	//compile fragment shader
	ifstream fragment_shader_file;
	int fragment_shader_length;
	fragment_shader_file.open("simple_shader.frag", ios::in);
	//go to end of the file to get total characters in the file
	fragment_shader_file.seekg(0, ios::end);
	fragment_shader_length = fragment_shader_file.tellg();
	fragment_shader_file.seekg(0, ios::beg);

	//allocated temporary character array for shader source
	char* fragment_shader_source = new char[fragment_shader_length + 1];//one extra character is for NULL
	fragment_shader_file.read(fragment_shader_source, fragment_shader_length);
	fragment_shader_source[vertex_shader_length] = '\0';
#if 0
	for(int i = 0; i<fragment_shader_length; i++)
		cout<<fragment_shader_source[i];
	cout<<endl;
#endif
	fragment_shader_file.close();
	
	GLuint fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, (const char **)&fragment_shader_source, NULL);
	delete[] fragment_shader_source;
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		cout<<"Fragment Shader did not compile\n";
		exit(0);
	}

	//create program
	program_id = glCreateProgram();
	if(program_id == 0)
	{
		cout<<"Program not created\n";
		exit(0);
	}
	
	//attach shaders
	glAttachShader(program_id, vertex_shader);
	glAttachShader(program_id, fragment_shader);

	//link vertex and fragment shaders
	glLinkProgram(program_id);
	glGetProgramiv(program_id, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		cout<<"Linking Failed\n";
		exit(0);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Hello Triangle");
	glewInit();
	initialize();

	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
