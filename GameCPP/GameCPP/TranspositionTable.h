#include <random>
#include <cmath>
#include "BreakthroughMove.h"
#include <iostream>

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
	TableEntry(long long zkey, BreakthroughMove move, int depth, int score, int min, int max){
		this->zkey = zkey;
		this->move = move;
		this->depth = depth;
		this->score = score;
		this->min = min;
		this->max = max;
	}

	inline long long getKey() const { return zkey; }
	inline BreakthroughMove getMove() const { return move; }
	inline int getDepth() const { return depth; }
	inline int getScore() const { return score; }
	inline int getMin() const { return min; }
	inline int getMax() const { return max; }

	/* For debugging purpose 
	 * "std::cout << entry << std::endl" or 
	 * "std::cout << table[entry]" << std::endl; 
	 */
	friend std::ostream& operator<<(std::ostream& os, TableEntry entry) {
		os  << "Key: " << entry.getKey() << ", "
			<< "Depth: " << entry.getDepth() << ", "
			<< "Score: " << entry.getScore() << ", "
			<< "Min: " << entry.getMin() << ", "
			<< "Max: " << entry.getMax() << " ";
		return os;
	}

private:
	long long zkey;
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
	void insert(long long zkey, BreakthroughMove move, int depth, int score, int min, int max);

	/**
	* Used to lookup entry in the table
	* @param zkey the zorbist key
	* @return the table entry
	*/
	TableEntry* lookup(long long zkey);

private:
	std::vector<TableEntry*> table;
};
