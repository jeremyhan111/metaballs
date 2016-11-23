#pragma once

#include <drawable.h>
namespace cs40{

class Line : public Drawable {

   public:
     //Construct a new Line with given corners
		 //vertices will be ordered CCW order internally
     Line(QOpenGLShaderProgram* const prog,
				 const vec2& pt1, const vec2& pt2);

     Line(const Line* const other);

     //No destructor necessary
     ~Line() {/*do nothing*/};

     //Draw the Line using openGL
     void draw();

	 // return true if shape contains point pt, false otherwise
     bool contains(const vec2 &pt) const;

   private:
     //Vertices of Line in ccw order
     vec2 m_pts[2];

		 /* don't implement this */
		 Line& operator=(const Line&); //prevent rec2=rec1 assignment
};

}//namespace
