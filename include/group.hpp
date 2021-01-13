#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>

class Group : public Object3D {

public:
    int size;// number of objects in the group
    std::vector<Object3D *> data; // objects in the group

    Group() {

    }

    explicit Group(int num_objects) {
        size = num_objects;
        for (int i = 0; i < size; i++) {// append null objects into the group
            Object3D *obj = nullptr;
            data.push_back(obj);
        }
    }

    ~Group() override {

    }

    Intersection intersect(const Ray &r, Hit &h, float tmin) override {
        // calculate the nearest intersection among all objects in group
        Intersection res = MISS;
        for (int i = 0; i < data.size(); i++) {
            if (data[i] == nullptr)
                continue;
            else {
                Intersection tmp= data[i]->intersect(r, h, tmin);// calculate intersection as possible for all objects
                if(tmp!=MISS)
                    res=tmp;
            }
        }
//        printf(" %d\n",res);
        return res;
    }

    void addObject(int index, Object3D *obj) {
        data[index] = obj;
        obj->id = index;
    }

    int getGroupSize() {
        return size;
    }

};

#endif


