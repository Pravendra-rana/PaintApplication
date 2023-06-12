#include "PaintApplication.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PaintApplication w;
    //Line w;
    w.setWindowTitle("Welcome To Paint Application");
    w.show();
    return a.exec();
}
//static const char* vertexShaderSource =
//"attribute highp vec4 posAttr;\n"
//"attribute lowp vec4 colAttr;\n"
//"varying lowp vec4 col;\n"
//"uniform highp mat4 matrix;\n"
//"void main() {\n"
//"   col = colAttr;\n"
//"   gl_Position = matrix * posAttr;\n"
//"}\n";
//
//static const char* fragmentShaderSource =
//"varying lowp vec4 col;\n"
//"void main() {\n"
//"   gl_FragColor = col;\n"
//"}\n";