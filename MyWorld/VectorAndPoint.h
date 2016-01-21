#ifndef Vector3D_H
#define Vector3D_H
#include <math.h>
class Vector3D
{
public:
    float x;
    float y;
    float z;
	Vector3D(void):x(0.0f), y(0.0f), z(0.0f){}
	Vector3D(float inputX, float inputY, float inputZ):x(inputX), y(inputY), z(inputZ){}
	Vector3D(const Vector3D & input):x(input.x), y(input.y), z(input.z){}
	void Set(float inputX, float inputY, float inputZ)
	{
        x=inputX;
        y=inputY;
        z=inputZ;
    }
	void LoadZero(void)
	{
        x = 0;
        y = 0;
        z = 0;
    }
	Vector3D CrossProduct(const Vector3D & input) const
	{
        return Vector3D(y*input.z - z*input.y, z*input.x - x*input.z, x*input.y - y*input.x);
    }

	float DotProduct(const Vector3D & input) const
	{
        return x*input.x + y*input.y + z*input.z;
    }
	void Normalize()
	{
		float norm = sqrt(x * x + y * y + z * z);
        x /= norm;
        y /= norm;
        z /= norm;
	}
	float GetLength() const
	{
        return (float)sqrt((x*x)+(y*y)+(z*z));
    }
	
	float GetQuaddLength() const
	{
        return (x*x)+(y*y)+(z*z);
    }
	Vector3D operator+(Vector3D & input) const
	{
        return Vector3D(x + input.x, y + input.y, z + input.z);
    }
	
	Vector3D operator-(Vector3D & input) const
	{
        return Vector3D(x - input.x, y - input.y, z - input.z);
    }
    Vector3D operator-(void) const
    {
        return Vector3D(-x, -y, -z);
    }
    Vector3D operator+(void) const
    {
        //return Vector3D(x,y,z);
        return *this;
    }
	Vector3D operator*(float input) const
	{
        return Vector3D(x*input, y*input, z*input);
    }
	//Vector3D operator*(const ve)
	Vector3D operator/(float input) const
	{
        return (input==0.0f) ? Vector3D(0.0f, 0.0f, 0.0f) : Vector3D(x / input, y / input, z / input);
    }


	void operator+=(Vector3D & input)
	{
        x+=input.x;
        y+=input.y;
        z+=input.z;
    }

	void operator-=(Vector3D & input)
	{
        x-=input.x;
        y-=input.y;
        z-=input.z;
    }

	void operator*=(float input)
	{
        x*=input;
        y*=input;
        z*=input;
    }
	
	void operator/=(float input)
	{
        if(input == 0)
			return;
		else
		{
            x/=input;
            y/=input;
            z/=input;
        }
	}
};
class Point2D {
public:
    //constructors
    Point2D(void)	:	x(0.0f), y(0.0f)
    {}
    
    Point2D(float newX, float newY):x(newX), y(newY)
    {}
    
    float x, y;
};

#endif	//Vector3D_H