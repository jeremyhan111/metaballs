#include "line.h"
#include "geomfun.h"

using namespace cs40;

Line::Line(){}
Line::Line(QOpenGLShaderProgram* const prog, vec2 pt1, vec2 pt2) {
    m_prog = prog;
    int numVertices = 2;

    m_pts[0] = pt1;
    m_pts[1] = pt2;

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


Line::Line(const Line *other){

    m_prog = other->m_prog;
    int numVertices = 2;

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

void Line::draw(){
     drawHelper(GL_LINES, 2);
}

bool Line::contains(const vec2& pt) const {
  // check if pt within buffer around line (since hard to hit line exactly)
  // note: have to take into accound displacement of object since cant change
  //  points that line was defined with.
    float epsilon = 5;
    if (distToSegment(m_pts[0]+m_displacement, m_pts[1]+m_displacement,
        pt)<epsilon) {
      return true;
    }
    return false;
}
