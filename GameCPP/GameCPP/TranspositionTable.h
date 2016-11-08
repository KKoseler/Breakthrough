#include <random>
#include <cmath>
#include "BreakthroughMove.h"
#include <iostream>

class TableEntry{

public:

	TableEntry()
	{ 
		zkey = 0; 
		move = BreakthroughMove(-1, -1, -1, -1);
	}

	/**
	* Used to create an entry into the transposition table
	* @param zkey the zorbist key.
	* @param move the move.
	* @param depth the current search depth.
	* @param score the score.
	* @param scoreType the scoreType.
	*/
	TableEntry(long long zkey, BreakthroughMove & move, int depth, int score, short type){
		this->zkey = zkey;
		this->move = move;
		this->depth = depth;
		this->score = score;
		this->type = type;
	}

	inline long long getKey() const { return zkey; }
	inline BreakthroughMove getMove() const { return move; }
	inline int getDepth() const { return depth; }
	inline int getScore() const { return score; }
	inline short getType() const { return type; }
	inline void setKey(long long & newkey) { this->zkey = newkey; }
	inline void setMove(BreakthroughMove & moveSet) { this->move = moveSet; }
	inline void setDepth(int newDepth) { this->depth = newDepth; }
	inline void setScore(int newScore) { this->score = newScore; }
	inline void setType(int newType) { this->type = newType; }

	/* For debugging purpose 
	 * "std::cout << entry << std::endl" or 
	 * "std::cout << table[entry]" << std::endl; 
	 */
	/*friend std::ostream& operator<<(std::ostream& os, TableEntry entry) {
		os  << "Key: " << entry.getKey() << ", "
			<< "Depth: " << entry.getDepth() << ", "
			<< "Min: " << entry.getMin() << ", "
			<< "Max: " << entry.getMax() << " ";
		return os;
	}*/

private:
	long long zkey;
	BreakthroughMove move;
	int depth;
	int score;
	short type;
};

const int TABLE_SIZE = 147453;

class TranspositionTable {

public:

	/**
	* Used to create a transposition table
	*/
	TranspositionTable() 
	{ 
		table = std::vector<TableEntry>(TABLE_SIZE);
		for (unsigned int i = 0; i < table.size(); i++) {
			table[i] = TableEntry();
		}
	}

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
	TableEntry* lookup(long long &zkey);

	int size() { return table.size(); }

private:
	std::vector<TableEntry> table;
};
