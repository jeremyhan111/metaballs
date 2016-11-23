#pragma once

#include <drawable.h>
namespace cs40{

class Triangle : public Drawable {

public:
    /* Construct a new triangle with given endpoints
            * Triangle should be oriented CCW, even if
            * p1, p2, p3 are not
            * Shader program is assumed to have the following variables
            *   vec2 vPosition            - VBO geometry
            *   uniform vec2 displacement - an amount to shift each vertex
            *   uniform vec3 color        - color of object */
    Triangle(QOpenGLShaderProgram* const prog,
             const vec2& p1, const vec2& p2, const vec2& p3);

    /* Construct a new triangle from an existing triangle */
    Triangle(const Triangle* const other);

    //No destructor necessary
    ~Triangle() {/*do nothing*/};

    //Draw the Triangle using openGL
    void draw();

    /* return true if shape contains point pt, false otherwise*/
    bool contains(const vec2 &pt) const;

private:
    //Endpoints of Triangle in ccw order
    vec2 m_pts[3];

    /* don't implement this */
    Triangle& operator=(const Triangle&); //prevent tri2=tri1 assignment
};

}//namespace

