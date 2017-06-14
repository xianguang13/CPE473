#ifndef SDS_H
#define SDS_H

#include "data.h"

void recursiveTreeBuild(std::vector<Object *> objs, int axis, bvh_node *bn);
void testHit(glm::vec3 dir, glm::vec3 origin, bvh_node *bn, std::vector<Object*> *closest);
void calculateParentBBox(bvh_node *bn);
void printTree(bvh_node *bn, int depth);

#endif
