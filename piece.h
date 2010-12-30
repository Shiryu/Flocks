#ifndef PIECE_H
#define PIECE_H

#include "shapes.h"

enum
{
	CYAN = 1,
	BLUE,
	ORANGE,
	YELLOW,
	GREEN,
	PURPLE,
	RED,
	GHOST
};

const int PIVOT_X = 1;
const int PIVOT_Y = 2;

class Piece
{
	
private:
	int kind;
	int orientation;
	int color;
	
public:
	Piece() { }
	Piece(int k, int o) { kind = k; orientation = o; }
	Piece(const Piece &p) { kind = p.kind; orientation = p.orientation; color = p.color; }
	
	void setKind(int k) { kind = k; }
	void setOrientation(int o) { orientation = o; }
	
	int getKind() { return kind; }
	int getOrientation() { return orientation; }
	
	void setColor(int c) { color = c; }
	int getColor() { return color; }
	
	bool willOverflowBoard()
	{
		for(int j = 0; j < SIZE; ++j)
		{
			if(PIECES[kind][orientation][0][j] == 1)
				return true;
		}
		
		return false;
	}
};

#endif
