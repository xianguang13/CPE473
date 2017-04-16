#pragma once
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
   public:
      glm::vec3 location;
      glm::vec3 up;
      glm::vec3 right;
      glm::vec3 lookat;

		void set_location(float, float, float);
		void set_up(float, float, float);
		void set_right(float, float, float);
		void set_lookat(float, float, float);
};

class Light {
   public:
      glm::vec3 location;
      glm::vec3 color;

		void set_location(float, float, float);
		void set_color(float, float, float);
};

class Object {
	public:
		virtual void dummy() {};
	   int type;
};

class Sphere: public Object{
   public:
      glm::vec3 center;
      float radius;
      glm::vec3 color;
      float ambient;
      float diffuse;
		glm::vec3 translate;

		void set_center(float, float, float);
		void set_color(float, float, float);
		void set_radius(float);
		void set_ambient(float);
		void set_diffuse(float);
		void set_translate(float, float, float);
};

class Plane: public Object {
   public:
      glm::vec3 normal;
      float distance;
      glm::vec3 color;
      float ambient;
      float diffuse;

		void set_normal(float, float, float);
		void set_color(float, float, float);
		void set_distance(float);
		void set_ambient(float);
		void set_diffuse(float);
};
