#include "geomfun.h"
#include <iostream>
using namespace cs40;

//this function finds is p3 is the to the leftOf p1 and p2
bool cs40::leftOf(const vec2& p1, const vec2& p2, const vec2& p3) {
	//finds vector from p1 to p2
	vec2 vector1 = p2 - p1;
	//finds vector from p1 to p3
	vec2 vector2 = p3 - p1;
	//finds the cross proudct between these two
	vec3 res = vec3::crossProduct(vector1, vector2);
	vec3 zhat = vec3(0,0,1);
	//if its dotted with zhat, and the number is positive, that means it is CCW due to math described
	//in class
	float end = vec3::dotProduct(zhat,res);
	if (end>0){
		return true;
	} else {
		return false;
	}
}

/*distToSegment - return float indicating distance of p3 from line defined by
 *	p1 and p2. Two cases: p3 withing perpendicular lines of ends, or outside
 *	those lines. Use dot product to deside case, and then cross product to
 *	compute stuff */
float cs40::distToSegment(const vec2& p1, const vec2& p2, const vec2& p3){
	//p1 and p2 are points on the line segment
	//p3 is the point we are trying to determine if close enough

	//we want to see if p3 is inside the "slab" created by perpendicular
	//lines of vector 1
	vec2 vector1 = p2 - p1;
	vec2 vector2 = p3 - p1;

	//we take the dot product and divide the scalar by the magnitude of
	//vector 1 squared, because this will give us the ratio of the
	//magnitude of the vector component of vector 2 parallel to vector 1
	//to the magnitude of vector 1.
	float result = vec3::dotProduct(vector1, vector2);
	float magnitude = vector1.length();

	float fraction = result/(magnitude*magnitude);
	//NOTE: result could be negative
	
	//If this ratio is greater than 0 and less than 1,
	if (fraction < 1 && fraction > 0) { //p3 'between' endpoints
		vector1 *= fraction; //rescale
		//than we can use the cross product to determine the distance from the line segment. 
		return vec3::crossProduct(vector1, vector2).length()/vector1.length();
	} else {
		//else, we can compute the distance from each of the distances and can take the lesser value
		float min = (p3-p1).length();
		if ((p3-p2).length()<min){ min = (p3-p2).length(); }
		return min;
	}
}

/* helpful debugging ostream operators */
std::ostream& cs40::operator<<(std::ostream& str, const QVector2D& vec){
    str << "(" <<(float) vec.x() << ", " << (float) vec.y() <<")";
    return str;
}

std::ostream& cs40::operator<<(std::ostream& str, const QVector3D& vec){
    str << "(" << (float) vec.x() << ", " << (float) vec.y() << ", ";
    str << (float) vec.z() << ")";
    return str;
}

std::ostream& cs40::operator<<(std::ostream& str, const QVector4D& vec){
    str << "(" << (float) vec.x() << ", " << (float) vec.y() << ", ";
    str << (float) vec.z() << ", " << (float) vec.w() << ")";
    return str;
}
