#ifndef SHAPECONTAINER_H
#define SHAPECONTAINER_H

#include <list>

#include "shape.h"

class ShapeContainer : public Shape {
    public:
        ShapeContainer();
        ~ShapeContainer();                     // der Destruktor

        void draw();
        void update();


        void addShape(Shape *instance);
        void deleteShape(int begin, int end);
        void clear();


    private:
        std::list<Shape*> shapeList;

};
#endif // SHAPECONTAINER_H
