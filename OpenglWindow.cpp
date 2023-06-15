#include "OpenglWindow.h"
#include<QMouseEvent>
#include <QOpenGLWidget>
#include<qmessagebox.h>
#include <cmath>



#define M_PI 3.1415926535897932384626433832795
static const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 u_projectionMatrix;"
"uniform mat4 u_viewMatrix;"
"uniform mat4 u_modelMatrix;"
"void main()\n"
"{\n"
"    gl_Position = u_projectionMatrix *  u_viewMatrix * u_modelMatrix * vec4(position, 1.0);\n"
"}";

static const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 fragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"    fragColor = color;\n"
"}";
bool OpenGLWindow::drawLineMode = false;
bool OpenGLWindow::drawCircleMode = false;
bool OpenGLWindow::drawRectangleMode = false;
bool OpenGLWindow::drawPolygonMode = false;
bool OpenGLWindow::highlightMode = false;

OpenGLWindow::OpenGLWindow(QWidget* parent)
	: QOpenGLWidget(parent)
{
	setMouseTracking(true);
}

void OpenGLWindow::initializeGL()
{
	initializeOpenGLFunctions();

	QOpenGLShader* vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
	vertexShader->compileSourceCode(vertexShaderSource);

	QOpenGLShader* fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
	fragmentShader->compileSourceCode(fragmentShaderSource);

	m_program = new QOpenGLShaderProgram(this);
	m_program->addShader(vertexShader);
	m_program->addShader(fragmentShader);
	m_program->link();

	m_posAttr = m_program->attributeLocation("position");

	glGenBuffers(1, &m_Vbo);
	glGenBuffers(1, &m_highlightVbo);

	// bind the vertex array object of line
	m_Vao.create();
	m_Vao.bind();

	//unbind the vertex array object of line
	m_Vao.release();

	//bind the vertex array object of circle
	m_highlightVao.create();
	m_highlightVao.bind();

	//unbind the vertex array object of circle
	m_highlightVao.release();
}

void OpenGLWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

	m_program->bind();
	m_program->setUniformValue("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f)); // White color
	m_program->setUniformValue("u_projectionMatrix", projectionMatrix);
	m_program->setUniformValue("u_viewMatrix", viewMatrix);
	m_program->setUniformValue("u_modelMatrix", modelMatrix);

	if (!geometryBase.empty())
	{
		for (int i = 0; i < geometryBase.size(); i++)
		{
			QOpenGLVertexArrayObject::Binder vaoBinder(&m_Vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
			glBufferData(GL_ARRAY_BUFFER, geometryBase[i].size() * sizeof(float), geometryBase[i].data(), GL_DYNAMIC_DRAW);
			glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(m_posAttr);
			glDrawArrays(GL_LINE_LOOP, 0, geometryBase[i].size() / 3);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		if (highlightMode)
		{
			m_program->setUniformValue("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
			for (int i = 0; i < highLight.size(); i++)
			{
				QOpenGLVertexArrayObject::Binder vaoBinder(&m_highlightVao);
				glBindBuffer(GL_ARRAY_BUFFER, m_highlightVbo);
				glBufferData(GL_ARRAY_BUFFER, highLight[i].size() * sizeof(float), highLight[i].data(), GL_DYNAMIC_DRAW);
				glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
				glEnableVertexAttribArray(m_posAttr);
				glDrawArrays(GL_LINE_LOOP, 0, highLight[i].size() / 3);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			highLight.clear();
			highlightMode = false;
		}
	}
	m_program->release();
}

void OpenGLWindow::resizeGL(int width, int height)
{
}

void OpenGLWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)

	{
		QPoint mousePos = event->pos();
		if (drawLineMode)
		{
			createLine(mousePos);
		}
		else if (drawCircleMode)
		{
			createCircle(mousePos);
		}
		else if (drawRectangleMode)
		{
			createRectangle(mousePos);
			if (rectangleVertices.size() == 24)
			{
				createRectangle(mousePos);
			}
		}
	}
	QOpenGLWidget::mousePressEvent(event);

}

void OpenGLWindow::createLine(QPoint mousePos)
{
	int viewportWidth = width();
	int viewportHeight = height();

	float x = (2.0f * mousePos.x() / viewportWidth) - 1.0f;
	float y = 1.0f - (2.0f * mousePos.y() / viewportHeight);

	lineVertices.push_back(x);
	lineVertices.push_back(y);
	lineVertices.push_back(0.0f);

	if (lineVertices.size() % 6 == 0)
	{
		geometryBase.push_back(lineVertices);
	    lineVertices.clear();
		emit lineCreated("Line");
	}
	update();


}

void OpenGLWindow::createCircle(QPoint mousePos)
{
	int viewportWidth = width();
	int viewportHeight = height();

	float x = (2.0f * mousePos.x() / viewportWidth) - 1.0f;
	float y = 1.0f - (2.0f * mousePos.y() / viewportHeight);

	static bool centerSet = false;
	static QPointF center;
	if (!centerSet)
	{
		center = QPointF(x, y);
		centerSet = true;
	}
	else
	{
		// Number of line segments to the circle
		const int numSegments = 100;
		float radius = std::sqrt((x - center.x()) * (x - center.x()) + (y - center.y()) * (y - center.y()));
		circleVertices.clear();

		for (int i = 0; i <= numSegments; ++i)
		{
			float angle = 2.0f * M_PI * i / numSegments;
			float cx = center.x() + radius * std::cos(angle);
			float cy = center.y() + radius * std::sin(angle);

			circleVertices.push_back(cx);
			circleVertices.push_back(cy);
			circleVertices.push_back(0.0f);
		}

		geometryBase.push_back(circleVertices);

		/*glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);*/

		centerSet = false;
	}

	if (circleVertices.size() == 303)
	{
		emit lineCreated("Circle");
	}
	circleVertices.clear();
	update();

}

void OpenGLWindow::createRectangle(QPoint mousePos)
{
	int viewportWidth = width();
	int viewportHeight = height();

	float x1 = (2.0f * mousePos.x() / viewportWidth) - 1.0f;
	float y1 = 1.0f - (2.0f * mousePos.y() / viewportHeight);

	if (rectangleVertices.size() < 22) {

		if (rectangleVertices.size() != 0)
		{
			rectangleVertices.push_back(x1);
			rectangleVertices.push_back(y1);
			rectangleVertices.push_back(0.0f);
		}

		rectangleVertices.push_back(x1);
		rectangleVertices.push_back(y1);
		rectangleVertices.push_back(0.0f);

		if (rectangleVertices.size() > 18) 
		{
			rectangleVertices.push_back(rectangleVertices[0]);
			rectangleVertices.push_back(rectangleVertices[1]);
			rectangleVertices.push_back(rectangleVertices[2]);

			geometryBase.push_back(rectangleVertices);
			rectangleVertices.clear();
		}
	}

	// Update the buffer data
	/*glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, rectangleVertices.size() * sizeof(float), rectangleVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	if (rectangleVertices.size() % 24 == 0)
	{
		emit lineCreated("Rectangle");
	}

	update();

}


