#pragma once

/* TODO: create, implement rectangle.cpp */

#include <drawable.h>
namespace cs40{

class Rectangle : public Drawable {

   public:
     //Construct a new Rectangle with given corners
		 //vertices will be ordered CCW order internally
     Rectangle(QOpenGLShaderProgram* const prog,
				 const vec2& corner1, const vec2& corner2);

     Rectangle(const Rectangle* const other);

     //No destructor necessary
     ~Rectangle() {/*do nothing*/};

     //Draw the Rectangle using openGL
     void draw();

		 // return true if shape contains point pt, false otherwise
     bool contains(const vec2 &pt) const;

   private:
     //Vertices of Rectangle in ccw order
     vec2 m_pts[4];

		 /* don't implement this */
		 Rectangle& operator=(const Rectangle&); //prevent rec2=rec1 assignment
};

}//namespace
