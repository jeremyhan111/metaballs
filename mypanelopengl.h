#ifndef MYPANELOPENGL_H
#define MYPANELOPENGL_H

#include <QtWidgets>
#include <QtOpenGL>
#include "drawable.h"
#include "circle.h"
#include "line.h"

using namespace cs40;

/* define a new type app_mode_t that stores the current mode*/
/* put this block in mypanelopengl.h but outside the class */

class MyPanelOpenGL : public QOpenGLWidget
{
    Q_OBJECT

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);

public:
    explicit MyPanelOpenGL(QWidget *parent = 0);
    virtual ~MyPanelOpenGL();

private:

    bool printCircles;
    bool printGrid;
    bool test;

    float m_width, m_height; //should technically be int!
    Line *** m_Field; //2d array of pointers to lines
    Line *** m_grid;
    float m_fsize; //actuaslly int!

    /* shader/program objects */
    QOpenGLShader *m_vertexShader;
    QOpenGLShader *m_fragmentShader;
    QOpenGLShaderProgram *m_shaderProgram;
    QMatrix4x4 m_matrix;
    vec3 curr_color; // set by setColor methods (mostly inline)

    /* list of drawable shapes */
    QList<cs40::Circle*> m_shapes;

    QTimer* m_timer;      /* event timer */

    /* helpers */
    void createShaders();
    void destroyShaders();

    /*helper methods for reacting to mousePressEvent based on m_currentMode*/
    void addCircle();
    void translate();

signals:
    /* empty */

public slots:

    /* trigger update of GL window */
    void calculateLine(int i, int j, int config);
    void updateTime();
    float fFunc(float x, float y);
    float lerpFunc(float b_x, float b_y, float d_x, float d_y, bool yDir);

};

#endif // MYPANELOPENGL_H
