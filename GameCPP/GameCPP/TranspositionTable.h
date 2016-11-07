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
	TableEntry(long long zkey, BreakthroughMove move, int depth, int min, int max){
		this->zkey = zkey;
		this->move = move;
		this->depth = depth;
		this->min = min;
		this->max = max;
	}

	inline long long getKey() const { return zkey; }
	inline BreakthroughMove getMove() const { return move; }
	inline int getDepth() const { return depth; }
	inline int getMin() const { return min; }
	inline int getMax() const { return max; }
	inline void setKey(long long zkey) { this->zkey = zkey; }
	inline void setMove(BreakthroughMove moveSet) { this->move = moveSet; }
	inline void setDepth(int newDepth) { this->depth = newDepth; }
	inline void setMin(int newMin) { this->min = newMin; }
	inline void setMax(int newMax) { this->max = newMax; }

	/* For debugging purpose 
	 * "std::cout << entry << std::endl" or 
	 * "std::cout << table[entry]" << std::endl; 
	 */
	friend std::ostream& operator<<(std::ostream& os, TableEntry entry) {
		os  << "Key: " << entry.getKey() << ", "
			<< "Depth: " << entry.getDepth() << ", "
			<< "Min: " << entry.getMin() << ", "
			<< "Max: " << entry.getMax() << " ";
		return os;
	}

private:
	long long zkey;
	BreakthroughMove move;
	int depth;
	int min;
	int max;
};

const int TABLE_SIZE = 17458;

class TranspositionTable {

public:

	/**
	* Used to create a transposition table
	*/
	TranspositionTable() { table.resize(TABLE_SIZE);}

	/**
	* Used to insert move positions in the table
	* @param entry the entry to be inserted
	*/
	void insert(TableEntry & entry);

	/**
	* Used to lookup entry in the table
	* @param zkey the zorbist key
	* @return the table entry
	*/
	TableEntry* lookup(long long zkey);

private:
	std::vector<TableEntry*> table;
};
