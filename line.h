#pragma once

#include <drawable.h>
namespace cs40{

class Line : public Drawable {

   public:
     //Construct a new Line with given corners
		 //vertices will be ordered CCW order internally
     Line();

     Line(QOpenGLShaderProgram* const prog,
				 vec2 pt1, vec2 pt2);

     Line(const Line* const other);

     //No destructor necessary
     ~Line() {/*do nothing*/};

     //Draw the Line using openGL
     void draw();

     void setShaderProg(QOpenGLShaderProgram* const prog);

	 // return true if shape contains point pt, false otherwise
     bool contains(const vec2 &pt) const;

     void addSecond(vec2 pt1, vec2 pt2);

     void changeLine(vec2 pt1, vec2 pt2);

   private:
     //Vertices of Line in ccw order
     vec2 m_pts[2];

     bool second;
     Line *m_other;

		 /* don't implement this */
		 //Line& operator=(const Line&); //prevent rec2=rec1 assignment
};

}//namespace
