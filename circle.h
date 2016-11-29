#pragma once

/* TODO: create, implement circle.cpp */

#include <drawable.h>
#include <iostream>

namespace cs40{

class Circle : public Drawable {

   public:
     //Construct a new Circle with given corners
		 //vertices will be ordered CCW order internally
     Circle(QOpenGLShaderProgram* const prog,
				 const vec2& pt, const float rad);

     Circle(QOpenGLShaderProgram* const prog,
                  const vec2& pt, const vec2& pt2);

     Circle(const Circle* const other);

     //No destructor necessary
     ~Circle() {delete [] m_pts;}

     //Draw the Circle using openGL
     void draw();

		 // return true if shape contains point pt, false otherwise
     bool contains(const vec2 &pt) const;

     float translateX();

     float translateY();


     void movePlus(int width, int height);

   private:
      
     //Vertices of Circle in ccw order
     vec2* m_pts;
     int numVertices;
     float m_rad;
     vec2 m_velocity;
     int timestep;


		 /* don't implement this */
		 Circle& operator=(const Circle&); //prevent rec2=rec1 assignment

};

}//namespace
