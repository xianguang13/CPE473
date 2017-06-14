#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include "pixelray.h"
#include "data.h"

using namespace std;
using namespace glm;

void checkMinMax(vec3 min, vec3 max, bvh_node *bn) {
   if (min.x < bn->min.x)
      bn->min.x = min.x;
   if (max.x > bn->max.x)
      bn->max.x = max.x;
   if (min.y < bn->min.y)
      bn->min.y = min.y;
   if (max.y > bn->max.y)
      bn->max.y = max.y;
   if (min.z < bn->min.z)
      bn->min.z = min.z;
   if (max.z > bn->max.z)
      bn->max.z = max.z;
}

void getBBoxSphere(Sphere *s, bvh_node *bn) {
   vec3 min, max;

   min.x = s->center.x - s->radius;
   max.x = s->center.x + s->radius;
   min.y = s->center.y - s->radius;
   max.y = s->center.y + s->radius;
   min.z = s->center.z - s->radius;
   max.z = s->center.z + s->radius;

   checkMinMax(min, max, bn);
}

void getBBoxTriangle(Triangle *t, bvh_node *bn) {
   vec3 min, max;

   min.x = std::min(t->p1.x, std::min(t->p2.x, t->p3.x));
   max.x = std::max(t->p1.x, std::max(t->p2.x, t->p3.x));
   min.y = std::min(t->p1.y, std::min(t->p2.y, t->p3.y));
   max.y = std::max(t->p1.y, std::max(t->p2.y, t->p3.y));
   min.z = std::min(t->p1.z, std::min(t->p2.z, t->p3.z));
   max.z = std::max(t->p1.z, std::max(t->p2.z, t->p3.z));

   checkMinMax(min, max, bn);
}

void createParentObjVec(bvh_node *bn) {
   //Create parent node obj vector
   for (unsigned int i = 0; i < bn->left->o.size(); i++)
      bn->o.push_back(bn->left->o.at(i));
   for (unsigned int i = 0; i < bn->right->o.size(); i++)
      bn->o.push_back(bn->right->o.at(i));
}

void calculateBBox(bvh_node *bn) {
   bn->min = vec3(numeric_limits<float>::max());
   bn->max = vec3(-99999999);

   if (bn->left != NULL && bn->right != NULL)
      createParentObjVec(bn);

   for (unsigned int i = 0; i < bn->o.size(); i++) {
		//Sphere
      if (bn->o.at(i)->type == 1)
         getBBoxSphere(dynamic_cast<Sphere*>(bn->o.at(i)), bn);
		//Triangle
      else if (bn->o.at(i)->type == 3)
         getBBoxTriangle(dynamic_cast<Triangle*>(bn->o.at(i)), bn);
   }
}

vector<Object*> buildLeftTree(vector<Object*> objs, int size) {
   vector <Object*> left;

   left.assign(objs.begin(), objs.begin() + (unsigned int) ceil(objs.size() / 2.f));

   return left;
}

vector<Object*> buildRightTree(vector<Object*> objs, int size) {
   vector <Object*> right;

   right.assign(objs.begin() + (unsigned int) ceil(objs.size() / 2.f), objs.end());

   return right;
}

vec3 getCenter(Object *o) {
   if (o->type == 1)
      return dynamic_cast<Sphere*>(o)->center;
   else {
      Triangle *t = dynamic_cast<Triangle*>(o);
      float avgX = (t->p1.x + t->p2.x + t->p3.x) / 3.f;
      float avgY = (t->p1.y + t->p2.y + t->p3.y) / 3.f;
      float avgZ = (t->p1.z + t->p2.z + t->p3.z) / 3.f;

      return vec3(avgX, avgY, avgZ);
   }
}

bool compareX(Object *o1, Object *o2) { 
   vec3 center1, center2;

   center1 = getCenter(o1);
   center2 = getCenter(o2);

   return center1.x < center2.x;
}

bool compareY(Object *o1, Object *o2) { 
   vec3 center1, center2;

   center1 = getCenter(o1);
   center2 = getCenter(o2);

   return center1.y < center2.y;
}

bool compareZ(Object *o1, Object *o2) { 
   vec3 center1, center2;

   center1 = getCenter(o1);
   center2 = getCenter(o2);

   return center1.z < center2.z;
}

void sortTree(bvh_node *bn, int axis) {
   //X axis
   if (axis == 0)
      sort(bn->o.begin(), bn->o.end(), compareX);

   //Y axis
   else if (axis == 1)
      sort(bn->o.begin(), bn->o.end(), compareY);

   //Z axis
   else if (axis == 2)
      sort(bn->o.begin(), bn->o.end(), compareZ);
}

bvh_node* createNewNode() {
   bvh_node *temp = new bvh_node;
   temp->left = temp->right = NULL;
   return temp;
}

void recursiveTreeBuild(vector<Object*> objs, int axis, bvh_node *bn) {
   if (objs.size() <= 1) {
      bn->o = objs;
      calculateBBox(bn);
      return;
   }

   sortTree(bn, axis);

   bn->left = createNewNode();
   bn->right = createNewNode();

   vector<Object*> leftObjs = buildLeftTree(objs, objs.size());
   vector<Object*> rightObjs = buildRightTree(objs, objs.size());

   recursiveTreeBuild(leftObjs, (axis + 1) % 3, bn->left);
   recursiveTreeBuild(rightObjs, (axis + 1) % 3, bn->right);

   calculateBBox(bn);
}



void testHit(vec3 dir, vec3 origin, bvh_node *bn, vector<Object*> *closest) {
   int index = -1;
   int ind = 0;
   float t = numeric_limits<float>::max();

   boxFirstHit(origin, bn, dir, &t, ind, &index);

   //If ray hits
   if (t != numeric_limits<float>::max()) {
      //Check for leaf
      if (bn->left == NULL && bn->right == NULL)
         closest->push_back(bn->o.at(0));
      //Not leaf, keep descending
      else {
         testHit(dir, origin, bn->left, closest);
         testHit(dir, origin, bn->right, closest);
      }
   }
   
   //No hit
   else
      return;
}

void printTree(bvh_node *bn, int depth) {
   cout << depth << endl;
   cout << "MIN: " << bn->min.x << " " << bn->min.y << " " << bn->min.z << endl;
   cout << "MAX: " << bn->max.x << " " << bn->max.y << " " << bn->max.z << endl;
   cout << "SIZE: " << bn->o.size() << endl;

   for (unsigned int i = 0; i < bn->o.size(); i++)
      cout << "TYPE: " << bn->o.at(i)->type << endl;

   if (bn->left != NULL) {
      cout << "\nLEFT" << endl;
      printTree(bn->left, depth + 1);
   }
   if (bn->right != NULL) {
      cout << "\nRIGHT" << endl;
      printTree(bn->right, depth + 1);
   }
}
