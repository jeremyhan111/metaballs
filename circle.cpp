#include "circle.h"
#include "geomfun.h"
#include <iostream>

using namespace cs40;

//constructor
Circle::Circle(QOpenGLShaderProgram* const prog,
              const vec2& pt, const float rad) {
    m_prog = prog;
    numVertices = 100000; //We could change this to a function of radius length
    m_pts = new vec2[numVertices];

    m_pts[0] = pt; //center point


    //circle is implemented in a manner such that the points
    //go in a CCW fashion.
    for(int i=1; i<numVertices-1; i++){
      float angle = 2*3.14159/(numVertices-2)*(i-1);
      float xval = pt.x()+rad*cos(angle);
      float yval = pt.y()+rad*sin(angle);
      m_pts[i] = QVector2D(xval, yval);
    }
    float xval = pt.x()+rad*cos(0);
    float yval = pt.y()+rad*sin(0);
    m_pts[numVertices-1] = QVector2D(xval, yval);

    //tested to see if the first triangle created for circle
    //is CCW just as a sanity check
    if (!leftOf(m_pts[0], m_pts[1], m_pts[2])) {
        std::cout << "not CCW" << std::endl;
    }

    m_rad = rad;
    m_velocity[0] = 10;
    m_velocity[1] = -10;
    timestep = 1;


    if (initVBO()) {
        /* IDs created by initVBO() */

        /*
             * bind VAO
             * bind program
             * bind VBO
             * allocate space on VBO and write points
             * enable attribute array for vPosition input in vertex shader
             * describe layout of VBO for attribute array
             * release program
             * release VAO
             */
        m_vao->bind();
        m_prog->bind();
        m_vbo->bind();
        m_vbo->allocate(m_pts, numVertices*sizeof(vec2));
        //set vPosition as input to vertex shader and specify that data should
        //be read from current vbo, and read 2 floats as one vPosition. Start
        //reading at position 0 and don't skip anything between reading
        //vPositions from attribute buffer
        m_prog->enableAttributeArray("vPosition");
        m_prog->setAttributeBuffer("vPosition", GL_FLOAT, 0, 2, 0);

        m_prog->release();
        m_vao->release();
    }
}

//constructor for defining circle based on two pts rather than pt and rad
//pt is center, pt2 indicates radius
Circle::Circle(QOpenGLShaderProgram* const prog,
                  const vec2& pt, const vec2& pt2) {
    m_prog = prog;
    numVertices = 100000; //We could change this to a function of radius length
    m_pts = new vec2[numVertices];

    m_pts[0] = pt; //center point

    timestep = 1;
    m_velocity[0] = 1;
    m_velocity[1] = -1;


    float rad = sqrt(pow(pt2.x()-pt.x(),2) + pow(pt2.y()-pt.y(),2));


    //circle is implemented in a manner such that the points
    //go in a CCW fashion.
    for(int i=1; i<numVertices-1; i++){
      float angle = 2*3.14159/(numVertices-2)*(i-1);
      float xval = pt.x()+rad*cos(angle);
      float yval = pt.y()+rad*sin(angle);
      m_pts[i] = QVector2D(xval, yval);
    }
    float xval = pt.x()+rad*cos(0);
    float yval = pt.y()+rad*sin(0);
    m_pts[numVertices-1] = QVector2D(xval, yval);

    //tested to see if the first triangle created for circle
    //is CCW just as a sanity check
    if (!leftOf(m_pts[0], m_pts[1], m_pts[2])) {
        std::cout << "not CCW" << std::endl;
    }


    if (initVBO()) {
        /* IDs created by initVBO() */

        /*
             * bind VAO
             * bind program
             * bind VBO
             * allocate space on VBO and write points
             * enable attribute array for vPosition input in vertex shader
             * describe layout of VBO for attribute array
             * release program
             * release VAO
             */
        m_vao->bind();
        m_prog->bind();
        m_vbo->bind();
        m_vbo->allocate(m_pts, numVertices*sizeof(vec2));
        //set vPosition as input to vertex shader and specify that data should
        //be read from current vbo, and read 2 floats as one vPosition. Start
        //reading at position 0 and don't skip anything between reading
        //vPositions from attribute buffer
        m_prog->enableAttributeArray("vPosition");
        m_prog->setAttributeBuffer("vPosition", GL_FLOAT, 0, 2, 0);

        m_prog->release();
        m_vao->release();
    }

}

//copy constructor
Circle::Circle(const Circle *other){
    /* implement copy constructor */
    m_prog = other->m_prog;
    numVertices = other->numVertices;
    m_pts = new vec2[numVertices];

    for (int i=0; i<numVertices; i++){
      m_pts[i] = other->m_pts[i];
    }

    if (initVBO()) {
        /* IDs created by initVBO() */
        //copied from other constructor:
        m_vao->bind();
        m_prog->bind();
        m_vbo->bind();
        m_vbo->allocate(m_pts, numVertices*sizeof(vec2));

        m_prog->enableAttributeArray("vPosition");
        m_prog->setAttributeBuffer("vPosition", GL_FLOAT, 0, 2, 0);

        m_prog->release();
        m_vao->release();
    }

    //update color and displacement
    setColor(other->m_color);
    move(other->m_displacement[0], other->m_displacement[1]);
}

void Circle::draw(){
     drawHelper(GL_TRIANGLE_FAN, numVertices);
}

bool Circle::contains(const vec2& pt) const {
  // check if pt inside radius
  // note: have to take into accound displacement of object since cant change
  //  points that circle was defined with.
    float radius = (m_pts[0]-m_pts[1]).length();
    if ((m_pts[0]+m_displacement-pt).length()<radius){ return true;}
    return false;
}

float Circle::translateX() {
  return timestep*m_velocity[0];
}

float Circle::translateY() {
  return timestep*m_velocity[1];
}