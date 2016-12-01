#ifndef MYPANELOPENGL_H
#define MYPANELOPENGL_H

#include <QtWidgets>
#include <QtOpenGL>
#include "drawable.h"
#include "circle.h"

using namespace cs40;

/* define a new type app_mode_t that stores the current mode*/
/* put this block in mypanelopengl.h but outside the class */
typedef enum CS40_APP_MODE{
  NONE=0,
  MOVING,
  DELETING,
  CHANGE_COLOR,
  ADD_CIRCLE,
  ADD_TRIANGLE,
  ADD_RECTANGLE,
  ADD_LINE
} app_mode_t;

class MyPanelOpenGL : public QOpenGLWidget
{
    Q_OBJECT

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:
    explicit MyPanelOpenGL(QWidget *parent = 0);
    virtual ~MyPanelOpenGL();

private:
    int m_width, m_height;
    float ** m_Field;
    int m_scalarSize;

    /* shader/program objects */
    QOpenGLShader *m_vertexShader;
    QOpenGLShader *m_fragmentShader;
    QOpenGLShaderProgram *m_shaderProgram;
    QMatrix4x4 m_matrix;
    app_mode_t m_currentMode;
    vec3 curr_color; // set by setColor methods (mostly inline)
    QList<vec2> live_clicks; //buffer for clicks that have happened.
                              // cleared regularly.
    int movingShapeI; //index of 'current' shape for MOVING mode

    /* list of drawable shapes */
    QList<cs40::Circle*> m_shapes;

    QTimer* m_timer;      /* event timer */

    /* helpers */
    void createShaders();
    void destroyShaders();

    /*helper methods for reacting to mousePressEvent based on m_currentMode*/
    void moving();
    void deleting();
    void changeColor();
    void addCircle(bool random);
    void addLine();
    void addTriangle();
    void addRectangle();
    void translate();

signals:
    /* empty */

public slots:
    //change modes. each time we change mode, reset live clicks to avoid previous
    //  clicks effecting current mode reactions
    void modeMoving(){ live_clicks.clear(); m_currentMode=MOVING; }
    void modeDeleting(){ live_clicks.clear(); m_currentMode=DELETING; }
    void modeChangeColor(){ live_clicks.clear(); m_currentMode=CHANGE_COLOR; }
    void modeAddCircle(){live_clicks.clear(); m_currentMode=ADD_CIRCLE;}
    void modeAddLine(){live_clicks.clear(); m_currentMode=ADD_LINE;}
    void modeAddTriangle(){live_clicks.clear(); m_currentMode=ADD_TRIANGLE;}
    void modeAddRectangle(){live_clicks.clear(); m_currentMode=ADD_RECTANGLE;}

    //setColor methods. Pretty straight forward. setRandom in .cpp file
    void setRed(){ curr_color=vec3(1.,0,0); }
    void setBlack(){ curr_color=vec3(0,0,0); }
    void setBlue(){ curr_color=vec3(0,0,1.); }
    void setGreen(){ curr_color=vec3(0,1.,0); }
    void setYellow(){ curr_color=vec3(1.,1.,0); }
    void setRandom(); //new random color only generated when box actually clicked

    /* trigger update of GL window */
    void updateTime();
    float computeFunction(float x, float y);

};

#endif // MYPANELOPENGL_H
