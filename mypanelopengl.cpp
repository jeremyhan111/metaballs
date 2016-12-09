 #include "mypanelopengl.h"
#include <cmath>
#include <iostream>
#include <QVector4D>
#include "line.h"

#include "geomfun.h"

using namespace cs40;

/*constructor*/
MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
    QOpenGLWidget(parent) {

    /* not much to do here */
    m_shaderProgram=NULL;
    m_vertexShader=NULL;
    m_fragmentShader=NULL;
    curr_color=vec3(0,0,0);
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




    int option = 0;

    if(option==0){ //complete simulation
      printGrid =false;
      printCircles=false;
      m_fsize=100;
      test=false;
    } else if (option==1){ //showing balls
      printGrid =false;
      printCircles=true;
      m_fsize=100;
      test=false;
    } else if(option==2){ //showing fFunc calculations
      printGrid =true;
      printCircles=true;
      m_fsize=20;
      test=true;
    } else if(option==3){ //showing lerp
      printGrid =true;
      printCircles=false;
      m_fsize=20;
      test=true;
    } else if(option==4){ //showing lerp in action
      printGrid =true;
      printCircles=false;
      m_fsize=20;
      test=false;
    }

    if (!test){
      m_timer = new QTimer(this);
      connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTime()));
      m_timer->start(1);
    }

    //m_fsize = 100;
    vec2 p1(0,0);
    vec2 p2(0,0);

    m_Field = new Line**[(int)m_fsize];
    m_grid = new Line**[(int)m_fsize];
    for (int i = 0; i<m_fsize; i++){
      m_Field[i] = new Line*[(int)m_fsize];
      m_grid[i] = new Line*[(int)m_fsize];
      m_grid[i][0] = new Line(m_shaderProgram, vec2(0, i*m_height/m_fsize), vec2(m_width, i*m_height/m_fsize));
      m_grid[i][1] = new Line(m_shaderProgram, vec2(i*m_width/m_fsize, 0), vec2(i*m_width/m_fsize, m_height));
      for( int j = 0; j<m_fsize; j++){
        //make default lines by overriding each 'line'
        m_Field[i][j] = new Line(m_shaderProgram, p1, p2);
        m_Field[i][j]->hide();
      }
    }

    paintGL();


}


/* resizeGL - override builtin method. automatically called when window resized */
void MyPanelOpenGL::resizeGL(int w, int h)
{
    std::cout.flush();
    m_width = w; m_height = h;
    glViewport(0,0,w, h);
    m_matrix.setColumn(0,QVector4D(2./w,0,0,0));
    m_matrix.setColumn(1,QVector4D(0,-2./h,0,0));

    if (test){ //kinda jenky
      addCircle();
    }

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
    m_shaderProgram->setUniformValue("mview", m_matrix);

    //print circles
    for(int i=0; i<m_shapes.size(); i++){
      if(printCircles)
      if (m_shapes[i]->isVisible()){ m_shapes[i]->draw(); }
    }

    for(int i=0; i<m_fsize; i++){
      if(printGrid){
        m_grid[i][0]->draw();
        m_grid[i][1]->draw();
      }
      for(int j=0; j<m_fsize; j++){
        if (m_Field[i][j]->isVisible()){
          m_Field[i][j]->draw();
        }
      }
    }
    glFlush();
    m_shaderProgram->release();
}

void MyPanelOpenGL::updateTime(){
  //update corner values
  translate();
    for(int i=0;i<m_fsize; i++){
      m_grid[i][0]->changeLine(vec2(0, i*m_height/m_fsize), vec2(m_width, i*m_height/m_fsize));
      m_grid[i][1]->changeLine(vec2(i*m_width/m_fsize, 0), vec2(i*m_width/m_fsize, m_height));
      for(int j=0;j<m_fsize;j++){
        //grab 4 corners
         m_Field[i][j]->hide();
        int config = 0; //indicates which of 16 cases. think binary
        //corner 1:
        float x = (float)i*m_width/m_fsize;
        float y = (float)j*m_height/m_fsize;

        if (fFunc(x,y)>=1){ config+=8;} //note, 8=2^3

        //corner 2
        x+=m_width/m_fsize;
        if (fFunc(x,y)>=1){ config+=4; }//note, 4=2^2
        //corner 3
        y+=m_height/m_fsize;
        if (fFunc(x,y)>=1){ config+=2;}//note 2=2^1
        //corner 4
        x-=m_width/m_fsize;
        if (fFunc(x,y)>=1){ config+=1;}//note 1=2^0

        //send to case switch
        if (config!=0){calculateLine(i,j,config);}
      }
    }

    update();
}

void MyPanelOpenGL::calculateLine(int i, int j, int config){

  float qx, qy, px, py;
  bool twoLines = false;
  float ax=i*m_width/m_fsize;
  float ay=j*m_height/m_fsize;
  float dx=(i+1)*m_width/m_fsize;
  float dy=(j+1)*m_height/m_fsize;
  //NOTE: (bx, by)=(dx, ay) and (cx, cy)=(ax, dy)

  if (config==0||config==15){
    m_Field[i][j]->hide();
    return;
  } else if(config==1||config==14){
    px= ax; //left side x
    qy= dy; //bottom y
    py=lerpFunc(ax, ay, ax, dy, true); //a, c, ydirection
    qx=lerpFunc(dx, dy, ax, dy, false); //d, c, x direction
  } else if(config==2||config==13){
    py=dy; qx=dx;
    px=lerpFunc(ax, dy, dx, dy, false); //c, d, xdir
    qy=lerpFunc(dx, ay, dx, dy, true); //b, d, ydir
  } else if(config ==3 ||config==12){
    px=ax; qx=dx;
    py= lerpFunc(ax, ay, ax, dy, true); //a, c, ydir
    qy= lerpFunc(dx, ay, dx, dy, true); //b, d, ydir
  } else if(config==4 || config==11){
    py=ay; qx=dx;
    px=lerpFunc(ax, ay, dx, ay, false);//a, b, xdir
    qy=lerpFunc(dx, dy, dx, ay, true);//d, b, ydir
  }  else if(config==6||config==9){
    py=ay; qy=dy;
    px= lerpFunc(ax, ay, dx, ay, false); //a, b, xdir
    qx= lerpFunc(ax, dy, dx, dy, false); //c, d, xdir
  } else if(config==7||config==8){
    px=ax; qy=ay;
    py=lerpFunc(ax, ay, ax, dy, true);//a, c, ydir
    qx=lerpFunc(ax, ay, dx, ay, false);//a, b xdir
  } else if(config==5){
    //two lines in this case!
    twoLines=true;
    //first line -- like 8
    px=ax; qy=ay;
    py=lerpFunc(ax, ay, ax, dy, true); //a, c, ydir
    qx=lerpFunc(ax, ay, dx, ay, false);//a, b xdir
    m_Field[i][j]->changeLine(vec2(px,py), vec2(qx,qy));
    //create extra line -- like 13
    py=dy; qx=dx;
    px=lerpFunc(ax, dy, dx, dy, false); //c, d, xdir
    qy=lerpFunc(dx, ay, dx, dy, true); //b, d, ydir
  } else if(config==10){
    //two lines in this case!
    twoLines=true;
    //first line -- like 1
    px= ax; //left side x
    qy= dy; //bottom y
    py=lerpFunc(ax, ay, ax, dy, true); //a, c, ydirection
    qx=lerpFunc(dx, dy, ax, dy, false); //d, c, x direction
    m_Field[i][j]->changeLine(vec2(px,py), vec2(qx,qy));
    //create extra line -- like 4
    py=ay; qx=dx;
    px=lerpFunc(ax, ay, dx, ay, false);//a, b, xdir
    qy=lerpFunc(dx, dy, dx, ay, true);//d, b, ydir
  } else {return;} //dummy case, never called


  if(twoLines){
    //draw _second_ lines
    m_Field[i][j]->addSecond(vec2(px,py), vec2(qx,qy));
  } else{
    m_Field[i][j]->changeLine(vec2(px,py), vec2(qx,qy));
  }
  m_Field[i][j]->unhide();
  //update line
}



/* mousePressEvent */
void MyPanelOpenGL::mousePressEvent(QMouseEvent *event){
    //init random circles
    if (test) { updateTime();}
    else { addCircle(); }
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



/* addCircle - called from mousePressEvent
 *  if only one click recorded, return in order to wait for second click.
 *  if two clicks recorded in live_clicks, first click is center, second
 *  indicates radius. Use that info to create circle, and then clear
 *  live_clicks */
void MyPanelOpenGL::addCircle(){
  int x=rand()%(int)m_width;
  int y = rand()%(int)m_height;
  int rad = rand()%30+30;
  //ensure entire circle is within window
  if (x+rad>=m_width){ x = x-rad-1;}
  if (x-rad<0){ x = rad;}
  if (y+rad>=m_height){ y = y-rad-1;}
  if (y-rad<0){ y = rad;}

  vec2 pt(x,y);
  QVector4D worldPoint(pt, 0, 1);

  Circle* circ;
  circ = new Circle(m_shaderProgram, pt, rad);
  circ->setColor(vec3(0,0,1));

  m_shapes.append(circ);
  update();
  return;
}

float MyPanelOpenGL::fFunc(float x, float y){
  float ret = 0;
  for(int k =0; k<m_shapes.size(); k++){
    float r_squared = m_shapes[k]->getRadius() * m_shapes[k]->getRadius();
    float x_comp = x - m_shapes[k]->getX();
    x_comp *= x_comp;
    float y_comp = y- m_shapes[k]->getY();
    y_comp *= y_comp;
    ret += r_squared/(x_comp+y_comp);

  }
  return ret;
}

//basically computing q_y
float MyPanelOpenGL::lerpFunc(float b_x, float b_y, float d_x, float d_y,
  bool yDir){
  float ret;
  ret = 1-fFunc(b_x, b_y);
  ret/= (fFunc(d_x, d_y)-fFunc(b_x, b_y));
  if(yDir){
    ret *= (d_y-b_y);
    ret = b_y + ret;
  } else{
    ret *= (d_x-b_x);
    ret = b_x + ret;
  }
  return ret;
}
