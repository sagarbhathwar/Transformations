#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include "window.h"
#include "vertex.h"
#include "input.h"

// Front Verticies
#define VERTEX_FTR Vertex( QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_FTL Vertex( QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_FBL Vertex( QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_FBR Vertex( QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f ) )

// Back Verticies
#define VERTEX_BTR Vertex( QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_BTL Vertex( QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f ) )
#define VERTEX_BBL Vertex( QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f ) )
#define VERTEX_BBR Vertex( QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f ) )

//Colored cube
static const Vertex sg_vertexes[] = {
  // Face 1 (Front)
    VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
    VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
  // Face 2 (Back)
    VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
    VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
  // Face 3 (Top)
    VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
    VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
  // Face 4 (Bottom)
    VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
    VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
  // Face 5 (Left)
    VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
    VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
  // Face 6 (Right)
    VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
    VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR

Window::Window(){ m_transform.translate(0.0f, 0.0f, -5.0f); }

Window::~Window()
{
    makeCurrent();
    teardownGL();
}

static QString vertexShaderSourceCode =
        "#version 440\n"
        "layout(location = 0) in vec3 position;\n"
        "layout(location = 1) in vec3 color;\n"
        "out vec4 vColor;\n"
        "uniform mat4 modelToWorld;\n"
        "uniform mat4 worldToCamera;\n"
        "uniform mat4 cameraToView;\n"
        "void main()\n"
        "{\n"
        "gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);\n"
          "vColor = vec4(color, 1.0);\n"
        "}\n";

static QString fragmentShaderSourceCode =
        "#version 440\n"
        "in vec4 vColor;\n"
        "out vec4 fColor;\n"
        "void main()\n"
        "{\n"
           "fColor = vColor;\n"
        "}\n";


void Window::initializeGL()
{
    initializeOpenGLFunctions();
    connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    printContextInformation();

    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceCode);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceCode);
    m_program->link();
    m_program->bind();

    u_modelToWorld = m_program->uniformLocation("modelToWorld");
    //u_worldToView = m_program->uniformLocation("worldToView"); //deprecated
    u_worldToCamera = m_program->uniformLocation("worldToCamera");
    u_cameraToView = m_program->uniformLocation("cameraToView");

    m_vertex.create();
    m_vertex.bind();
    m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertex.allocate(sg_vertexes, sizeof(sg_vertexes));

    m_object.create();
    m_object.bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
    m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

    m_object.release();
    m_vertex.release();
    m_program->release();
}

void Window::resizeGL(int width, int height)
{
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
}

void Window::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    //m_program->setUniformValue(u_worldToView, m_projection); //deprecated
    m_program->bind();
    m_program->setUniformValue(u_worldToCamera, m_camera.toMatrix());
    m_program->setUniformValue(u_cameraToView, m_projection);
    {
        m_object.bind();
        m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
        glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));
        m_object.release();
    }
    m_program->release();
}

void Window::update()
{
    Input::update();

    if (Input::buttonPressed(Qt::RightButton))
    {
      static const float transSpeed = 0.025f;
      static const float rotSpeed   = 0.025f;

      m_camera.rotate(-rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
      m_camera.rotate(-rotSpeed * Input::mouseDelta().y(), m_camera.right());

      QVector3D translation;
      if (Input::keyPressed(Qt::Key_W)) translation += m_camera.forward();
      if (Input::keyPressed(Qt::Key_S)) translation -= m_camera.forward();
      if (Input::keyPressed(Qt::Key_A)) translation -= m_camera.right();
      if (Input::keyPressed(Qt::Key_D)) translation += m_camera.right();
      if (Input::keyPressed(Qt::Key_Q)) translation -= m_camera.up();
      if (Input::keyPressed(Qt::Key_E)) translation += m_camera.up();
      m_camera.translate(transSpeed * translation);
    }

    m_transform.rotate(1.0f, QVector3D(0.3f, 0.3f, 0.3f));
    QOpenGLWindow::update();
}

void Window::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat()) event->ignore();
    else Input::registerKeyPressed(event->key());
}

void Window::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat()) event->ignore();
    else Input::registerKeyReleased(event->key());
}

void Window::mousePressEvent(QMouseEvent* event) { Input::registerMousePressed(event->button()); }

void Window::mouseReleaseEvent(QMouseEvent* event){ Input::registerMouseReleased(event->button()); }

void Window::teardownGL()
{
    m_vertex.destroy();
    m_object.destroy();;
    delete m_program;
}

void Window::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    glType = (context() -> isOpenGLES()) ? "OpenGL ES" : "Open GL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    #define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
    }
    #undef CASE
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
