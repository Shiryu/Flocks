#ifndef PIECE_H
#define PIECE_H

#include "shapes.h"

enum {O, I, S, Z, L, J, T};

enum { CYAN = 1, BLUE, ORANGE, YELLOW, GREEN, PURPLE, RED, GHOST };

const int PIVOT_X = 1;
const int PIVOT_Y = 2;

class Piece
{
	
private:
	int kind;
	int orientation;
	int color;
	
	int posX;
	int posY;
	
public:
	Piece() { }
	Piece(int k, int o) { kind = k; orientation = o; }
	Piece(const Piece &p) { kind = p.kind; orientation = p.orientation; color = p.color; posX = p.posX; posY = p.posY; }
	
	void setKind(int k) { kind = k; }
	void setOrientation(int o) { orientation = o; }
	
	int getKind() { return kind; }
	int getOrientation() { return orientation; }
	
	void setColor(int c) { color = c; }
	int getColor() { return color; }
	
	void setPosX(int x) { posX = x; }
	int getPosX() { return posX; }
	
	void setPosY(int y) { posY = y; }
	int getPosY() { return posY; }
};

#endif
