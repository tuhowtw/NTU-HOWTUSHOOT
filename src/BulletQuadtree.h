#include <vector>
#include <Bullet.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace std;

class BulletQuadtree;

class BulletQuadtree {
public:
	BulletQuadtree(float _x, float _y, float _width, float _height, int _level, int _maxLevel);
	~BulletQuadtree();
	void AddObject(Bullet *object);
	vector<Bullet *> GetObjectsAt(float x, float y);
	void clear();
private:
	float x;
	float y;
	float width;
	float height;
	int level;
	int maxLevel;
	vector<Bullet*> objects;


	BulletQuadtree * parent;
	BulletQuadtree * NW;
	BulletQuadtree * NE;
	BulletQuadtree * SW;
	BulletQuadtree * SE;

	sf::RectangleShape shape;

	bool contains(BulletQuadtree * child, Bullet * Bullet);
	FloatRect getGlobalBounds();
};


