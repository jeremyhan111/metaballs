#include "mypanelopengl.h"
#include <cmath>
#include <iostream>
#include <QVector4D>
#include "triangle.h"
#include "rectangle.h"
#include "line.h"
#include "circle.h"
#include "geomfun.h"

using namespace cs40;

/*constructor*/
MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
    QOpenGLWidget(parent) {

    /* not much to do here */
    m_shaderProgram=NULL;
    m_vertexShader=NULL;
    m_fragmentShader=NULL;
    m_currentMode=MOVING;
    curr_color=vec3(0,0,0);
    movingShapeI = -1;
    m_matrix.setColumn(3,QVector4D(-1,1,0,1));

    m_timer = NULL;
}

/* destructor */
MyPanelOpenGL::~MyPanelOpenGL(){
    destroyShaders();
    /* free all the shapes */
    while(m_shapes.size() > 0 ){
        delete m_shapes.last();
        m_shapes.pop_back();
    }
}

/*initializeGL - calle at outset so initialize stuff */
void MyPanelOpenGL::initializeGL()
{
    glEnable(GL_CULL_FACE); /* required, culling *disabled* by default */
    glFrontFace(GL_CW); /* optional, GL_CCW is default */
    glCullFace(GL_BACK);  /* optional, GL_BACK is default */
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    createShaders();

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    m_timer->start(10);

}

/* resizeGL - override builtin method. automatically called when window resized */
void MyPanelOpenGL::resizeGL(int w, int h)
{
    m_width = w; m_height = h;
    glViewport(0,0,w, h);
    m_matrix.setColumn(0,QVector4D(2./w,0,0,0));
    m_matrix.setColumn(1,QVector4D(0,-2./h,0,0));
    update();
}

/* paintGL - automatically called by update() */
void MyPanelOpenGL::paintGL(){

    if(!m_shaderProgram){
        return; /* give up */
    }

    glClear(GL_COLOR_BUFFER_BIT);

    /* draw all the shapes */


    m_shaderProgram->bind();
    //m_matrix(0,0)=1;
    //m_matrix(1,1)=1;
    m_shaderProgram->setUniformValue("mview", m_matrix);


    for(int i=0; i<m_shapes.size(); i++){
      if (m_shapes[i]->isVisible()){ m_shapes[i]->draw(); }
    }

    glFlush();
    m_shaderProgram->release();
}

void MyPanelOpenGL::updateTime(){
    /* TODO: modify when animation loop and timer are connected */
    /* trigger a refresh */
    update();
    translate();
}

/* mousePressEvent - grab click and add it to live_clicks queue. Then, depending
 *  on current move, call helper method to deal with fall out. If helper method
 *  notes that there are not yet enough clicks in live_clicks, it will return
 *  and wait for the helper to be called again. Note that each time current mode
 *  is changed, live_clicks is cleared, so that start with clean space. Also,
 *  any time helper method actually completes action, live_clicks is cleared so
 *  that future calls to helper wont use old data. This means that live_clicks
 *  should never have more than 3 elements */
void MyPanelOpenGL::mousePressEvent(QMouseEvent *event){
    vec2 click(event->localPos().x(),event->localPos().y());
    QVector4D worldPoint(click, 0, 1);
    live_clicks.append(worldPoint.toVector2D());

    //switch to check current mode
    //hand info to helper funciton
    //grab second click in helper...?

    switch (m_currentMode)
    {
        case NONE:
            break;
        case MOVING:
            moving();
            break;
        case DELETING:
            deleting();
            break;
        case CHANGE_COLOR:
            changeColor();
            break;
        case ADD_CIRCLE:
            addCircle();
            break;
        /*case ADD_TRIANGLE:
            addTriangle();
            break;
        case ADD_RECTANGLE:
            addRectangle();
            break;
        case ADD_LINE:
            addLine();
            break;*/
    }
}

/* mouseMoveEvent - if mouse is moving and MOVING is selected. and first click
 *  has been noted, move 'current' with mouse. Update new location of click*/
void MyPanelOpenGL::mouseMoveEvent(QMouseEvent *event){
    vec2 click(event->pos().x(),event->pos().y());
    //if (m_currentMode == MOVING && live_clicks.size()==1){
    translate();
    //}
}

/*createShaders */
void MyPanelOpenGL::createShaders(){

    destroyShaders(); //get rid of any old shaders

    m_vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!m_vertexShader->compileSourceFile("shaders/vshader.glsl")){
        qWarning() << m_vertexShader->log();
    }

    m_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if(!m_fragmentShader->compileSourceFile("shaders/fshader.glsl")){
        qWarning() << m_fragmentShader->log();
    }

    m_shaderProgram = new QOpenGLShaderProgram();
    m_shaderProgram->addShader(m_vertexShader);
    m_shaderProgram->addShader(m_fragmentShader);

    if(!m_shaderProgram->link()){
        qWarning() << m_shaderProgram->log() << endl;
    }

}

/*destroyShaders */
void MyPanelOpenGL::destroyShaders(){

    delete m_vertexShader; m_vertexShader=NULL;
    delete m_fragmentShader; m_fragmentShader=NULL;

    if(m_shaderProgram){
        m_shaderProgram->release();
        delete m_shaderProgram; m_shaderProgram=NULL;
    }
}

void MyPanelOpenGL::translate() {
    for (int i = 0; i < m_shapes.size(); i++) {
        /*float dx = m_shapes[i]->translateX();
        float dy = m_shapes[i]->translateY();
        m_shapes[i]->move(dx, dy);
        std::cout << dx << " " << dy << " " << i << std::endl;*/
        m_shapes[i]->movePlus(m_width, m_height);
    }
}

/* moving - called from mousePressEvent
 *  grab first click with call from mousePressEvent. If one click is already
 *  grabbed, move new click to beginning and shorten live_clicks queue.
 *  When only one live_clicks is left, check if it is contained in a visible
 *  shape. If it is, note the index of that shape. If not, clear live_clicks
 *  (so that nothing will move.. see dependencies in mouseMoveEvent) */
void MyPanelOpenGL::moving(){
  if (live_clicks.size()==2){
    live_clicks[0] = live_clicks[1];
    live_clicks.pop_back();

  }
  for (int i=m_shapes.size()-1; i>=0; i--){
    if (m_shapes[i]->contains(live_clicks[0])&& m_shapes[i]->isVisible() ){
        movingShapeI = i;
        return;
    }
  }
  live_clicks.clear(); //if click not contained in shape
}

/* deleting - called from mousePressEvent
 *  called from mousePressEvent when DELETING is selected. Iterate through shapes,
 *  front to back, and see if any _visible_ shape contains current click */
void MyPanelOpenGL::deleting(){
  int size = m_shapes.size();
  for (int i=size-1; i>=0; i--){
    if (m_shapes[i]->contains(live_clicks[0])&& m_shapes[i]->isVisible() ){
      m_shapes[i]->hide();
      break;
    }
  }
  update();
  live_clicks.clear();

}

/* changeColor - called from mousePressEvent
 *  check for visible shape that contains click. If one exists, change color to
 *  curr_color (which is set in setColor methods) */
void MyPanelOpenGL::changeColor(){
    int size = m_shapes.size();
    for (int i=size-1; i>=0; i--){
      if (m_shapes[i]->contains(live_clicks[0])&& m_shapes[i]->isVisible() ){
        m_shapes[i]->setColor(curr_color);
        break;
      }
    }
    update();
    live_clicks.clear();
}

/* addCircle - called from mousePressEvent
 *  if only one click recorded, return in order to wait for second click.
 *  if two clicks recorded in live_clicks, first click is center, second
 *  indicates radius. Use that info to create circle, and then clear
 *  live_clicks */
void MyPanelOpenGL::addCircle(){
    if (live_clicks.size()<2){ return; }
    if (live_clicks.size()>2){ std::cout<<"NOOOOO!!!!\n"; return;} //this shouldnt happen!!

    Circle* circ;
    circ = new Circle(m_shaderProgram, live_clicks[0], live_clicks[1]);
    circ->setColor(curr_color);

    m_shapes.append(circ);
    update();

    //clear live_clicks
    live_clicks.clear();
}


/* setRandom - set random color. This called each time random (color) is
 *  selected, so random color only regenerated when random option re-selected */
void MyPanelOpenGL::setRandom(){
  const int numColors = 8;
  vec3 colors[numColors];
  colors[0] = vec3(1.,0,0);
  colors[1] = vec3(0,1.,0);
  colors[2] = vec3(0,0,1.);
  colors[3] = vec3(1.,1.,0);
  colors[4] = vec3(1.,0,1.);
  colors[5] = vec3(0,1.,1.);
  colors[6] = vec3(1.,1.,1.);
  colors[7] = vec3(0,0,0);

  int random = rand();
  random = random%numColors;
  curr_color = colors[random];
}
