// !!!:Feysal:20101220 
// !!!:Feysal:20101221 
#include "piece.h"

Piece::Piece()
{
	
}

Piece::Piece(int k, int o)
{
	kind = k;
	orientation = o;
}

Piece::Piece(const Piece &p)
{
	kind = p.kind;
	orientation = p.orientation;
	color = p.color;
}

void Piece::setKind(int k)
{
	kind = k;
}

void Piece::setOrientation(int o)
{
	orientation = o;
}

int Piece::getKind()
{
	return kind;
}

int Piece::getOrientation()
{
	return orientation;
}

void Piece::setColor(int c)
{
	color = c;
}

int Piece::getColor()
{
	return color;
}

bool Piece::willOverflowBoard()
{
	for(int j = 0; j < SIZE; ++j)
	{
		if(PIECES[kind][orientation][0][j] == 1)
			return true;
	}
	
	return false;
}