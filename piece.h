// !!!:Feysal:20101220 
// !!!:Feysal:20101221 
// !!!:Feysal:20101226 
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
	Piece();
	Piece(int k, int o);
	Piece(const Piece &);
	
	void setKind(int k);
	void setOrientation(int o);
	
	int getKind();
	int getOrientation();
	
	void setColor(int c);
	int getColor();
	
	bool willOverflowBoard();
};

#endif
