#include "GamePlayer.h"
#include "GameMove.h"
#include "GameState.h"
#include "BreakthroughState.h"
#include "BreakthroughMove.h"
#include "BoardGameState.h"
#include "GameState.h"
#include "TranspositionTable.h"
#include <stdio.h>
#include <utility>
#include <random>
#include <cmath>

class AlphaBetaBreakThroughPlayer : public GamePlayer {
public:
	/**
	* The constructor that is used to create an an alpha-beta Breakthrough player
	* @param nickname the name of the player
	* @param d the depth
	*/
	AlphaBetaBreakThroughPlayer(std::string nickname, int d);

	/**
	* 
	* @param state the game state
	* @param lastMv the last move
	* @return the game move
	*/
	GameMove *getMove(GameState &state, const std::string &lastMv) override;

	std::vector<BreakthroughMove> getPossibleMoves(BreakthroughState &st, char sideToMove, int currentDepth);

	/**
	* Assigns a value to the diagonal paths
	* @param brd the game state
	* @param who the who
	* @param row the row
	* @param col the column
	* @return the assigned values
	*/
	int diagonalPath(BreakthroughState & brd, char who, int row, int col);

	/**
	* Computes the number of columns where the player
	* does not have any pieces.
	* @param brd the game state
	* @param who the who
	* @return the number of empty columns on the board
	*/
	int numberOfEmptyColumns(BreakthroughState & brd, char who);

	/**
	* Assigns a score for a piece that is threatened
	* @param brd the game state
	* @param row the row
	* @param col the column
	* @return the value assigned
	*/
	int threatPosition(BreakthroughState & brd, char who, int row, int col);

	/**
	* Assigns a score for a piece with a defensive posture
	* @param brd the game state
	* @param who the who
	* @param row the row
	* @param col the column
	* @return the value assigned
	*/
	int defensivePosition(BreakthroughState & brd, char who, int row, int col);

	/**
	* Assigns a score for a piece that is close to ending the game
	* @param brd the game state
	* @param who the who
	* @param row the row
	* @param col the column
	* @return the value assigned
	*/
	int pieceNearEndPosition(BreakthroughState & brd, char who, int row, int col);

	/**
	* Computes a score for a piece on the board
	* @param brd the game state
	* @param who the who
	* @param row the row
	* @param col the column
	* @return the value assigned to that piece
	*/
	int evaluatePiece(BreakthroughState & brd, char who, int row, int col);

	/**
	* Computes a score for the state
	* @param brd the game state
	* @return the value assigned to the current state of the board
	*/
	int evaluateBoard(BreakthroughState & brd);

	/**
	* A hash function that generates a Zobrist key for moves
	* @param brd the board state
	* @return Zobrist key
	*/
	long long zobristHash(BreakthroughState & brd);

	/**
	* Computes the best value of a position 
	* @param brd the game state
	* @param currDepth the current depth
	* @param gamma the gamma value
	* @return the best move, and moves have values attached as member fields
	*/
	std::pair<int, BreakthroughMove> test(BreakthroughState & brd, int maxDepth, int currDepth, int gamma);

private:
	std::vector<BreakthroughMove> mvStack;
	int depthLimit;
	char ourSymbol;
	bool home;

	int numMoves;

	std::vector<std::vector<std::vector<long long> > > zobristkeys;
	TranspositionTable transTable;
	TableEntry entry;

};