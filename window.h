#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>

#include "transform3d.h"
#include "input.h"
#include "camera3d.h"

class QOpenGLShaderProgram;

class Window : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    Window();
    ~Window();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

protected slots:
    void teardownGL();
    void update();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram* m_program;

    int u_modelToWorld;
    int u_worldToCamera;
    int u_cameraToView;
    //int u_worldToView; //deprecated since we have a camera now
    QMatrix4x4 m_projection;
    Camera3D m_camera;
    Transform3D m_transform;

    void printContextInformation();
};

#endif // WINDOW_H

//NOTES

//OpenGLBuffer
/* OpenGLBuffer are like unique ids to dynamic memory on GPU
 * client application can avoid uploading vertices, indices, texture image data, etc every time they are needed
 * http://doc.qt.io/qt-5/qopenglbuffer.html
*/

//OpenGLVertexArrayObject
/*container object that encapsulates the state needed to specify per-vertex attribute data to the OpenGL pipeline
 * To put it another way, a VAO remembers the states of buffer objects and their associated state
 * This allows a very easy and efficient method of switching between OpenGL buffer states for rendering different "objects" in a scene
 * Simple : Keeps track of all the buffers and bind information that is associated with a draw call
 * In scene initialization function, for each visual object:
 *     *Bind the VAO
 *     *Set vertex data state for this visual object (vertices, normals, texture coordinates etc.)
 *     *Unbind (release()) the VAO
 * In render function, for each visual object:
 *     *Bind the VAO (and shader program if needed)
 *     *Call a glDraw*() function
 *     *Unbind (release()) the VAO
 * http://doc.qt.io/qt-5/qopenglvertexarrayobject.html
*/

//OpenGLShaderProgram
/* QOpenGLShaderProgram class allows OpenGL shader programs to be linked and used.
 * http://doc.qt.io/qt-5/qopenglshaderprogram.html
 * Vertex Shaders
 *      *Takes input information and processes it
 *      *Finds final position for GPU to render it
 *      *Position and color
 * Fragment Shaders
 *      *Takes the interpolated results from the Vertex Shader, and outputs calculations
 *      *Outputs to BackBuffer
 */
