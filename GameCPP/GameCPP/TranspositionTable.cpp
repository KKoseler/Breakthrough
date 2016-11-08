#include <random>
#include <cmath>
#include "BreakthroughMove.h"
#include "TranspositionTable.h"

void
TranspositionTable::insert(TableEntry &entry) {
	
	int index = (int)(entry.getKey()%TABLE_SIZE);

	/*if (table[index] != NULL) {
		//we replace if the potential replacement is deeper than the current entry
		if (entry.getDepth() > table[index]->getDepth())
			table[index] = &entry;
	}
	else*/
	table[index] = entry;
	//std::cout << entry << std::endl;
}

TableEntry*
TranspositionTable::lookup(long long &zkey) {
	int index = (int) (zkey%TABLE_SIZE);
	//if (key != 0 && key == zkey) {
		/*if (table[index]->getKey() == zkey)
			return table[index];
		else {
			std::cout << "COLLISION! Key in the entry: " << table[index]->getKey() << "\n"
				<< "Key that was passed: " << zkey << "\n" << "Index of entry: " << index 
				<< "/n" << std::endl;
		}*/
	//std::cout << "HASHKEY: " << table[index].getKey() << std::endl;
		return &table[index];
	}
	//else {
		//return nullptr;
	//}


