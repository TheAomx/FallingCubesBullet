
#include "shapeContainer.h"


ShapeContainer::ShapeContainer() {


}

ShapeContainer::~ShapeContainer() {                     // der Destruktor


}

void ShapeContainer::addShape(Shape *instance) {
    shapeList.push_back(instance);
}

void ShapeContainer::deleteShape(int begin, int end) {
    size_t length = shapeList.size();
    if (begin > length || end > length) return; // macht keinen Sinn Element ausserhalb der Liste zu löschen
    if (begin > end) return;        // gibt auch keinen Sinn
    std::list<Shape*>::iterator range_begin = shapeList.begin();
    std::list<Shape*>::iterator range_end = shapeList.begin();
    std::advance(range_begin,begin);
    std::advance(range_end,end);

    shapeList.erase(range_begin, range_end);

}

void ShapeContainer::clear() {
    shapeList.clear();
}

void ShapeContainer::draw() {
    glPushMatrix();
    glTranslatef(getX(), getY(), getZ());
    glRotatef(getRotateValueZ(), 0.00, 0.00, 1.00);
    glRotatef(getRotateValueY(), 0.00f, 1.00f, 0.00f);
    glRotatef(getRotateValueX(),1.00f, 0.00, 0.00);

    for(std::list<Shape*>::iterator i=shapeList.begin(); i != shapeList.end(); ++i) {
        (*i)->draw();
    }

    glPopMatrix();
}

void ShapeContainer::update() {
    setX(getX()+getSpeedX());
    if (getX() > 10.00f) setSpeed(getSpeedX()*-1);
    if (getX() < -10.00f) setSpeed(getSpeedX()*-1);
    setY(getY()+getSpeedY());
    setZ(getZ()+getSpeedZ());
    setRotateValues(getRotateValueX()+getRotateSpeedValueX(), getRotateValueY()+getRotateSpeedValueY(), getRotateValueZ()+getRotateSpeedValueZ());


    for(std::list<Shape*>::iterator i=shapeList.begin(); i != shapeList.end(); ++i) {
        (*i)->update();
    }
}
