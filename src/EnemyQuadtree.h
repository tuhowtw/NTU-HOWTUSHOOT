#include <vector>
#include <Enemy.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace std;

class EnemyQuadtree;

class EnemyQuadtree {
public:
	EnemyQuadtree(float _x, float _y, float _width, float _height, int _level, int _maxLevel);
	~EnemyQuadtree();
	void AddObject(Enemy *object);
	vector<Enemy *> GetObjectsAt(float x, float y);
private:
	float x;
	float y;
	float width;
	float height;
	int level;
	int maxLevel;
	vector<Enemy*> objects;


	EnemyQuadtree * parent;
	EnemyQuadtree * NW;
	EnemyQuadtree * NE;
	EnemyQuadtree * SW;
	EnemyQuadtree * SE;

	sf::RectangleShape shape;

	bool contains(EnemyQuadtree * child, Enemy * enemy);
	FloatRect getGlobalBounds();
};


