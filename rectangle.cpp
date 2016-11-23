#include "rectangle.h"
#include "geomfun.h"
#include <iostream>

using namespace cs40;

Rectangle::Rectangle(QOpenGLShaderProgram* const prog,
    const vec2& corner1, const vec2& corner2) {
    m_prog = prog;
    int numVertices = 4;

    //implemented so that corner1 is top left corner
    //and corner 2 is bottom right corner
    m_pts[0] = corner1;
    m_pts[1] = QVector2D(corner1[0], corner2[1]);
    m_pts[2] = QVector2D(corner2[0], corner1[1]);
    m_pts[3] = corner2;

    //Checks if the rectangle is counter clockwise.
    //Points are entered into leftOf function so that
    //either both triangles are clockwise or counterclockwise
    //Thus, we only need to make one swap if a swap is necessary.
    if (!leftOf(m_pts[0], m_pts[1], m_pts[2])) {
        vec2 temp = m_pts[2];
        m_pts[2] = m_pts[1];
        m_pts[1] = temp;
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

Rectangle::Rectangle(const Rectangle *other){
    /* implement copy constructor */
    m_prog = other->m_prog;
    int numVertices = 4;

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

void Rectangle::draw(){
    /* bind active vao
     * set displacement uniform value in vertex shader
     * set color uniform value in fragment shader
     * glDrawArrays
     * release vao
     */
     drawHelper(GL_TRIANGLE_STRIP, 4);
}

bool Rectangle::contains(const vec2& pt) const {
  // check if pt is inside either of the triangles that define the rectangle
  // note: have to take into accound displacement of object since cant change
  //  points that rectangle was defined with.

  bool firstTri = leftOf(m_pts[0]+m_displacement, m_pts[1]+m_displacement, pt)
      && leftOf(m_pts[1]+m_displacement, m_pts[2]+m_displacement, pt)
      && leftOf(m_pts[2]+m_displacement, m_pts[0]+m_displacement, pt);
  bool secondTri = leftOf(m_pts[2]+m_displacement, m_pts[1]+m_displacement, pt)
      && leftOf(m_pts[1]+m_displacement, m_pts[3]+m_displacement, pt)
      && leftOf(m_pts[3]+m_displacement, m_pts[2]+m_displacement, pt);

  if (firstTri || secondTri){
    return true;
  }
  return false;
}
