#include "triangle.h"
#include "geomfun.h"
#include <iostream>

using namespace cs40;

//constructor
Triangle::Triangle(QOpenGLShaderProgram* const prog,
                   const vec2& p1, const vec2& p2, const vec2& p3) {
    m_prog = prog;

    m_pts[0] = p1;
    m_pts[1] = p2;
    m_pts[2] = p3;


    //checks if p3 is to the leftOf p1 and p2
    //if not, swap p2 and p3 so that they triangle is CCW
    if (!leftOf(p1, p2, p3)) {
        m_pts[1] = p3;
        m_pts[2] = p2;
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
        m_vbo->allocate(m_pts, 3*sizeof(vec2)); //3=numVertices
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
Triangle::Triangle(const Triangle *other){
    /* implement copy constructor */
    //TODO :: not tested all the way:
    m_prog = other->m_prog; //CAN I DO THIS!?

    for (int i=0; i<3; i++){
      m_pts[i] = other->m_pts[i];
    }

    if (initVBO()) {
        /* IDs created by initVBO() */
        //copied from other constructor:
        m_vao->bind();
        m_prog->bind();
        m_vbo->bind();
        m_vbo->allocate(m_pts, 3*sizeof(vec2)); //3=numVertices

        m_prog->enableAttributeArray("vPosition");
        m_prog->setAttributeBuffer("vPosition", GL_FLOAT, 0, 2, 0);

        m_prog->release();
        m_vao->release();
    }

    //update color and displacement
    setColor(other->m_color);
    move(other->m_displacement[0], other->m_displacement[1]);
}

void Triangle::draw(){
     drawHelper(GL_TRIANGLES, 3);
 }

bool Triangle::contains(const vec2& pt) const {
    // check if pt is left of all line segments defineing triangle
    // note: have to take into accound displacement of object since cant change
    //  points that triangle was defined with.
    if (leftOf(m_pts[0]+m_displacement, m_pts[1]+m_displacement, pt) &&
        leftOf(m_pts[1]+m_displacement, m_pts[2]+m_displacement, pt)
        && leftOf(m_pts[2]+m_displacement, m_pts[0]+m_displacement, pt)){
      return true;
    }
    return false;
}
