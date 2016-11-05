#include <random>
#include <cmath>
#include "BreakthroughMove.h"

class TableEntry{

public:

	TableEntry(){ ; }

	/**
	* Used to create an entry into the transposition table
	* @param zkey the zorbist key.
	* @param move the move.
	* @param depth the current search depth.
	* @param score the score.
	* @param scoreType the scoreType.
	*/
	TableEntry(long zkey, BreakthroughMove move, int depth, int score, int min, int max){
		this->zkey = zkey;
		this->move = move;
		this->depth = depth;
		this->score = score;
		this->min = min;
		this->max = max;
	}

	inline long  getKey() { return zkey; }
	inline BreakthroughMove  getMove() { return move; }
	inline int getDepth() { return depth; }
	inline int getScore() { return score; }
	inline int getMin(){ return min; }
	inline int getMax(){ return max; }

private:
	long zkey;
	BreakthroughMove move;
	int depth;
	int score;
	int min;
	int max;
};

const int TABLE_SIZE = 174583;

class TranspositionTable {

public:

	/**
	* Used to create a transposition table
	*/
	TranspositionTable() { table.resize(TABLE_SIZE);}

	/**
	* Used to insert move positions in the table
	* @param zkey the zorbist key
	* @param move the move
	* @param depth the current search depth
	* @param score the score
	* @param min the min
	* @param max the max score
	*/
	void insert(long zkey, BreakthroughMove move, int depth, int score, int min, int max);

	/**
	* Used to lookup entry in the table
	* @param zkey the zorbist key
	* @return the table entry
	*/
	TableEntry& lookup(long zkey);

private:
	std::vector<TableEntry> table;
};
