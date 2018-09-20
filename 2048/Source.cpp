#include<iostream>
using namespace std;

struct Point {
	Point() :x(0), y(0) {}
	Point(int x, int y) :x(x), y(y) {}
	int x;
	int y;
};
class Cell
{
public:
	explicit Cell(int i = 0) :value(i) {}
	Cell operator+(Cell const & other) {
		return Cell(value + other.value);
	}
	Cell & operator+=(Cell const & other) {
		value += other.value;
		return *this;
	}
	Cell & operator=(Cell const & other) {
		if (&other != this) {
			value = other.value;
		}
		return *this;
	}
	bool operator==(Cell const & other) const { return value == other.value; }
	bool is_free() const { return value == 0; }
	void free() { value = 0; }
	int get_value() const { return value; }
	void set_value(int value) { this->value = value; }
	~Cell() {}

private:
	int value;
};
class Board
{
public:
	explicit Board(size_t size = 4) : size(size) {
		board = new Cell*[size];
		for (size_t i = 0; i < size; i++) {
			board[i] = new Cell[size];
		}
		Point random;
		for (int i = 0; i < 2; i++) {
			PutIntoRandomPlace();
		}
	}
	~Board() {
		for (size_t i = 0; i < size; i++) {
			delete[] board[i];
		}
		delete board;
	}
	void ViewBoard() const {
		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				cout << board[i][j].get_value() << " ";
			}
			cout << endl;
		}
	}
	void Right() {
		
		if (!AllowedRight()) return;

		for (size_t i = 0; i < size; i++) {
			for (int j = size - 2, k = size - 1; j >= 0; j--) {
				if (board[i][j].is_free()) {
					continue;
				}
				if (board[i][k].is_free()) {
					board[i][k] += board[i][j];
					board[i][j].free();
					continue;
				}
				if (board[i][j] == board[i][k]) {
					board[i][k] += board[i][j];
					board[i][j].free();
					k--;
				}
				else {
					if (j != k - 1) {
						board[i][k - 1] = board[i][j];
						board[i][j].free();
					}
					k--;
				}
			}
		}
		PutIntoRandomPlace();
	}
	void Left() {
		
		if (!AllowedLeft()) return;

		for (size_t i = 0; i < size; i++) {
			for (int j = 1, k = 0; j < size; j++) {
				if (board[i][j].is_free()) {
					continue;
				}
				if (board[i][k].is_free()) {
					board[i][k] += board[i][j];
					board[i][j].free();
					continue;
				}
				if (board[i][j] == board[i][k]) {
					board[i][k] += board[i][j];
					board[i][j].free();
					k++;
				}
				else {
					if (j != k + 1) {
						board[i][k + 1] = board[i][j];
						board[i][j].free();
					}
					k++;
				}
			}
		}
		PutIntoRandomPlace();
	}
	void Up() {
		if (!AllowedUp()) return;
		for (size_t j = 0; j < size; j++) {
			for (int i = 1, k = 0; i < size; i++) {
				if (board[i][j].is_free()) {
					continue;
				}
				if (board[k][j].is_free()) {
					board[k][j] += board[i][j];
					board[i][j].free();
					continue;
				}
				if (board[i][j] == board[k][j]) {
					board[k][j] += board[i][j];
					board[i][j].free();
					k++;
				}
				else {
					if (i != k + 1) {
						board[k + 1][j] = board[i][j];
						board[i][j].free();
					}
					k++;
				}
			}
		}
		PutIntoRandomPlace();
	}
	void Down() {
		if (!AllowedDown()) return;
		for (size_t j = 0; j < size; j++) {
			for (int i = size - 2, k = size - 1; i >= 0; i--) {
				if (board[i][j].is_free()) {
					continue;
				}
				if (board[k][j].is_free()) {
					board[k][j] += board[i][j];
					board[i][j].free();
					continue;
				}
				if (board[i][j] == board[k][j]) {
					board[k][j] += board[i][j];
					board[i][j].free();
					k--;
				}
				else {
					if (i != k - 1) {
						board[k - 1][j] = board[i][j];
						board[i][j].free();
					}
					k--;
				}
			}
		}
		PutIntoRandomPlace();
	}
	bool IsOver() const {
		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				if (board[i][j].is_free()) return false;
			}
		}
		if (AllowedLeft()) return false;
		if (AllowedRight()) return false;
		if (AllowedUp()) return false;
		if (AllowedDown()) return false;
		return true;
	}
	int Score() const {
		int score = 0;
		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				if (board[i][j].get_value() > score) score = board[i][j].get_value();
			}
		}
		return score;
	}

private:
	Cell * *board;
	size_t size;
	Point RandomFreePoint() const {
		int free_zone_qt = 0;
		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				if (board[i][j].is_free()) free_zone_qt++;
			}
		}
		Point* points = new Point[free_zone_qt];
		for (size_t i = 0, k = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				if (board[i][j].is_free()) {
					points[k].x = i;
					points[k].y = j;
					k++;
				}
			}
		}
		Point random_free_point = points[rand() % free_zone_qt];
		delete[] points;
		return random_free_point;
	}
	void PutIntoRandomPlace() {
		int number;
		number = 4 ? number = 2 : rand() % 2;
		Point random;
		random = RandomFreePoint();
		board[random.x][random.y].set_value(number);
	}
	bool AllowedRight() const{
		bool allowed = false;
		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				if (board[i][j].is_free()) { continue; }
				for (int k = j + 1; k < size; k++) {
					if (board[i][k].is_free()) {
						allowed = true;
						break;
					}
					else
					{
						if (board[i][j] == board[i][k]) {
							allowed = true;
							break;
						}
						else {
							j = k;
						}
					}
				}
				break;
			}
			if (allowed) break;
		}
		return allowed;
	}
	bool AllowedLeft() const {
		bool allowed = false;
		for (size_t i = 0; i < size; i++) {
			for (int j = size - 1; j >= 0; j--) {
				if (board[i][j].is_free()) { continue; }
				for (int k = j - 1; k >= 0; k--) {
					if (board[i][k].is_free()) {
						allowed = true;
						break;
					}
					else {
						if (board[i][k] == board[i][j]) {
							allowed = true;
							break;
						}
						else {
							j = k;
						}
					}
				}
				break;
			}
			if (allowed) break;
		}
		return allowed;
	}
	bool AllowedUp() const {
		bool allowed = false;
		for (size_t j = 0; j < size; j++) {
			for (int i = size - 1; i >= 0; i--) {
				if (board[i][j].is_free()) { continue; }
				for (int k = i - 1; k >= 0; k--) {
					if (board[k][j].is_free()) {
						allowed = true;
						break;
					}
					else {
						if (board[i][j] == board[k][j]) {
							allowed = true;
							break;
						}
						else {
							i = k;
						}
					}
				}
				break;
			}
			if (allowed) break;
		}
		return allowed;
	}
	bool AllowedDown() const {
		bool allowed = false;
		for (size_t j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				if (board[i][j].is_free()) { continue; }
				for (int k = i + 1; k < size; k++) {
					if (board[k][j].is_free()) {
						allowed = true;
						break;
					}
					else {
						if (board[i][j] == board[k][j]) {
							allowed = true;
							break;
						}
						else {
							i = k;
						}
					}
				}
				break;
			}
			if (allowed) break;
		}
		return allowed;
	}
};

int main()
{
	Board a(2);
	a.ViewBoard();
	int s;
	while (!a.IsOver())
	{
		cin >> s;
		switch (s)
		{
		case 6:
			a.Right();
			break;
		case 4:
			a.Left();
			break;
		case 8:
			a.Up();
			break;
		case 2:
			a.Down();
			break;
		default:
			break;
		}
		a.ViewBoard();
	}
	cout << "Game Over" << endl;
	cout << "Your score is " << a.Score() << endl;
	system("pause");
}