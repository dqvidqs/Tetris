#include <SFML/Graphics.hpp>
#include <time.h>
//#include <windows.h>
//#include <string>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;;

/*////////////////
 *      METADATA
 
	const int _size_px = 40;
	const int _heigth_px = 640;
	const int _width_px = 400;
	const int _heigth = 16;
	const int _width = 10;
	const int _q = 4;
*/
/*////////////////
 *     POINT
 */
struct Point
{
	int x, y;
};//a[4], b[4];
/*////////////////
 *     GRID
 */
class Grid
{
private:
	int h = 16;
	int w = 10;

public:
	int field[16][10] = { 0 };
	bool check(Point points[]) {
		for (int i = 0; i < 4; i++) {
			if (points[i].x < 0 || points[i].x >= 10 || points[i].y >= 16) return 0;
			else if (field[points[i].y][points[i].x]) return 0;
		}
		return 1;
	}
	bool Set(Point points[], int color) {
		for (int i = 0; i < 4; i++) {
			field[points[i].y][points[i].x] = color;
		}
		return 0;
	}
	void Line() {
		for (int i = 0; i < 16; i++) {
			int count = 0;
			for (int j = 0; j < 10; j++) {
				if (field[i][j] != 0)
					count++;
			}
			if (count == 10) {
				for (int j = 0; j < 10; j++)
					for (int z = i; z > 0; z--)
						field[z][j] = field[z - 1][j];
			}
			count = 0;
		}
	}
};
/*////////////////
 *      Figure
 */
class Figure {
public:
	int ColorNum;
	bool lock = 0;
	int dx = 0;
	bool dy = 0;
	bool rotate = 0;
	Point point[4];
	int figure[7][4] = {
		1,3,5,7,
		2,4,5,7,
		3,5,4,6,
		3,5,4,7,
		2,3,5,7,
		3,5,7,6,
		2,3,4,5,
	};
	Figure() {
		int n = rand() % 7;//taking figure
		ColorNum = rand() % 5 + 1;
		//point = new Point[4];
		for (int i = 0; i < 4; i++) {
			point[i].x = figure[n][i] % 2;
			point[i].y = figure[n][i] / 2;
		}
	}
	void Update(Grid grid) {
		Move(grid);
		Rotate(grid);
	}
	void Gravity(Grid grid) {
		for (int i = 0; i < 4; i++) {
			prepare[i] = point[i];
			point[i].y += 1;
		}
		if (Check(grid)) {
			lock = 1;
		}
	}
private:
	~Figure() {
		delete this;
	}
	Point prepare[4];
	void Move(Grid grid) {
		for (int i = 0; i < 4; i++) {
			prepare[i] = point[i];
			point[i].x += dx;
		}
		Check(grid);
	}
	void Rotate(Grid grid) {
		if (rotate) {
			Point p = point[1];
			for (int i = 0; i < 4; i++) {
				int x = point[i].y - p.y;
				int y = point[i].x - p.x;
				point[i].y = p.y + y;
				point[i].x = p.x - x;
			}
			Check(grid);
		}
	}
	int Check(Grid grid) {
		if (!grid.check(point)) {
			for (int i = 0; i < 4; i++) {
				point[i] = prepare[i];
			}
		}
		else return 0;
		return 1;
	}
};

/*////////////////
 *      MAIN
 */
int main() {
	int _size = 40;
	srand(time(0));
	Grid grid;
	Figure *figure =  new Figure;
	//Figure figure;
	RenderWindow window(VideoMode(400, 640), "Tetris");
	//TEST
	//
	Texture texture;
	if (!texture.loadFromFile("images/blocks.png"))
	{
		string str;
		getline(cin, str);
		return 1;
	}

	Sprite sprite(texture);
	


	bool created = 0;
	float timer = 0, delay = 0.4;
	Clock clock;
	while (window.isOpen()) {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		if (figure->lock) {
			grid.Set(figure->point, figure->ColorNum);
			figure = new Figure;
		}
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (event.type == Event::KeyPressed) {
				switch (event.key.code) {
				case Keyboard::Up:
					figure->rotate = 1;
					break;
				case Keyboard::Left:
					figure->dx = -1;
					break; 
				case Keyboard::Right:
					figure->dx = 1;
					break; 
				case Keyboard::Down:
					figure->dy = 1;
					break; 
				}

			}
		}
		grid.Line();
		figure->Update(grid);
		if (timer >= delay || figure->dy) {
			figure->Gravity(grid);
			timer = 0;
		}

		figure->dx = 0; figure->rotate = 0; figure->dy = 0;
		window.clear(Color::White);
		for (int i = 0; i < 4; i++) {
			sprite.setTextureRect(IntRect(figure->ColorNum*_size -_size, 0, 40, 40));
			sprite.setPosition(figure->point[i].x*_size, figure->point[i].y*_size);
			window.draw(sprite);
		}
		for (int i = 0; i< 10; i++) {
			for (int j = 0; j < 16; j++) {
				if (grid.field[j][i] == 0) continue;
				sprite.setTextureRect(IntRect(grid.field[j][i]*_size-_size, 0, 40, 40));
				sprite.setPosition(i*_size, j*_size);
				window.draw(sprite);
			}
		}
		
		window.display();
	}


	return 0;
}
