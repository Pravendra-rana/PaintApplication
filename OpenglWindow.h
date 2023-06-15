#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <string>

#include <QString>

class OpenGLWindow : public QOpenGLWidget, protected QOpenGLFunctions 
{

   Q_OBJECT


public:
    OpenGLWindow(QWidget* parent = nullptr);

    void initializeGL()override;
    void paintGL()override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent* event) override;

    void createLine(QPoint mousePos);
    void createCircle(QPoint mousePos);
    void createRectangle(QPoint mousePos);
    static bool drawLineMode;
    static bool drawCircleMode;
    static bool drawRectangleMode;
    static bool drawPolygonMode;
    static bool highlightMode;

    std::vector<std::vector<float>> geometryBase;
    std::vector<std::vector<float>> highLight;



private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
   
    QOpenGLShaderProgram* m_program;


    GLuint m_Vbo;
    GLuint m_highlightVbo;

    QOpenGLVertexArrayObject m_Vao, m_highlightVao;
  
    std::vector<float> lineVertices;
    std::vector<float> circleVertices;
    std::vector<float> rectangleVertices;

    QMatrix4x4 projectionMatrix;
    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;
   
signals:
    void lineCreated(const QString& name);
};

