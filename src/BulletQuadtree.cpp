#include "BulletQuadtree.h"
#include <iostream>
#include <sstream>

using namespace std;

BulletQuadtree::BulletQuadtree(float _x, float _y, float _width, float _height, int _level, int _maxLevel) :
	x		(_x),
	y		(_y),
	width	(_width),
	height	(_height),
	level	(_level),
	maxLevel(_maxLevel)
{
	shape.setPosition(x, y);
	shape.setSize(sf::Vector2f(width, height));

    if (level == maxLevel) {
		return;
	}

	NW = new BulletQuadtree(x, y, width / 2.0f, height / 2.0f, level + 1, maxLevel);
	NE = new BulletQuadtree(x + width / 2.0f, y, width / 2.0f, height / 2.0f, level + 1, maxLevel);
	SW = new BulletQuadtree(x, y + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, maxLevel);
	SE = new BulletQuadtree(x + width / 2.0f, y + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, maxLevel);
}

BulletQuadtree::~BulletQuadtree()
{
	if(level != maxLevel){
		delete NW;
		delete NE;
		delete SW;
		delete SE;
	}
}

void BulletQuadtree::AddObject(Bullet * object){

    //no more division
    if(level == maxLevel){
        objects.push_back(object);
		return;
    }

	if (contains(NW, object)) {
		NW->AddObject(object); return;
	} else if (contains(NE, object)) {
		NE->AddObject(object); return;
	} else if (contains(SW, object)) {
		SW->AddObject(object); return;
	} else if (contains(SE, object)) {
		SE->AddObject(object); return;
	}

	if (contains(this, object)) {
		objects.push_back(object);
	}
}

vector<Bullet *> BulletQuadtree::GetObjectsAt(float _x, float _y){
    /* cout << "LET's SEE " << this->level << " &" << this->maxLevel << endl;
    if(level == maxLevel) std::cout << "OKEE\n"; */
    if(level == maxLevel) return objects;

	vector<Bullet *> returnObjects, childReturnObjects;
	if (!this->objects.empty()) {
        returnObjects = objects;
	}
	if (_x > x + width / 2.0f && _x < x + width) {
		if (_y > y + height / 2.0f && _y < y + height) {
            if(SE != nullptr){
                childReturnObjects = SE->GetObjectsAt(_x, _y);
			    returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
            }
			return returnObjects;
		} else if (_y > y && _y <= y + height / 2.0f) {
            if(NE != nullptr){
                childReturnObjects = NE->GetObjectsAt(_x, _y);
			    returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
            }

			return returnObjects;
		}
	} else if (_x > x && _x <= x + width / 2.0f) {
		if (_y > y + height / 2.0f && _y < y + height) {
			if(SW != nullptr){
                childReturnObjects = SW->GetObjectsAt(_x, _y);
			    returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
            }
			return returnObjects;
		} else if (_y > y && _y <= y + height / 2.0f) {
			if(NW != nullptr){
                childReturnObjects = NW->GetObjectsAt(_x, _y);
			    returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
            }
			return returnObjects;
		}
	}
	return returnObjects;
}

bool BulletQuadtree::contains(BulletQuadtree * child ,Bullet *object) {
	if(child == nullptr) return false;
    return child->getGlobalBounds().intersects(object->getGlobalBounds());
}

FloatRect BulletQuadtree::getGlobalBounds(){
	return shape.getGlobalBounds();
}

void BulletQuadtree::clear() {
	if(level != maxLevel){
		NW->clear();
    	NE->clear();
    	SW->clear();
    	SE->clear();
	}

	if (!objects.empty()) {
		objects.clear();
	}
}





