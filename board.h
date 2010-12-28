// !!!:Feysal:20101220 
// !!!:Feysal:20101221 
// !!!:Feysal:20101226 
#ifndef BOARD_H
#define BOARD_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "piece.h"

enum 
{
	FREE = 0, 
	FILLED = 1
};

const int BOARD_HEIGHT = 20;
const int BOARD_WIDTH = 10;

const int ORIGIN_X = 0;
const int ORIGIN_Y = 5;

class BPiece : public Piece
{
private:
	int posX;
	int posY;
	
public:
	BPiece() { }
	BPiece(Piece parent) : Piece(parent) { }
	BPiece(const BPiece &p) { (*this) = p; posX = p.posX; posY = p.posY; }
	
	void setPosX(int x) { posX = x; }
	int getPosX() { return posX; }
	
	void setPosY(int y) { posY = y; }
	int getPosY() { return posY; }
};
	
class Board
{
private:
	BPiece currentPiece;
	BPiece ghostPiece;
	
	void flood(int i, int j, int px, int py, int k, int o, int value, bool visited[][SIZE]);
	void flood(int i, int j, int px, int py, int k, int o, bool &flag, bool visited[][SIZE]);
	void floodFill(int i, int j, int px, int py, int k, int o, int value);
	
public:
	int area[BOARD_WIDTH][BOARD_HEIGHT];
	
	Board();
	
	void setCurrentPiece(BPiece p) { currentPiece = p; }
	BPiece getCurrentPiece() {return currentPiece; }
	
	void setCurrentGhostPiece(BPiece p) { ghostPiece = p; }
	BPiece getCurrentGhostPiece() { return ghostPiece; }
	
	void drawPiece(BPiece p);
	void clearPiece(BPiece p);
	
	void newPiece(BPiece p);
	
	bool isCurrentPieceMovable(int x, int y);
	bool isCurrentPieceRotable(int o);
	
	void moveCurrentPieceDown();
	void moveCurrentPieceLeft();
	void moveCurrentPieceRight();
	
	void rotateCurrentPieceLeft();
	void rotateCurrentPieceRight();
	
	void drawGhostPiece();
	void moveGhostPiece();
	
	void deleteLine(int y);
	int deletePossibleLines();
	
	void dropCurrentPiece();
	bool isCurrentPieceFallen();
	
	bool isGameOver();
};

#endif