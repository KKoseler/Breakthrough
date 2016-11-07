//
//  BreakthroughStateState.h
//  CSE486AIProject
//
//  Created by Yunlong Nick Liu on 5/29/15.
//  Copyright (c) 2015 Yunlong Nick Liu. All rights reserved.
//

#ifndef __CSE486AIProject__BreakthroughState__
#define __CSE486AIProject__BreakthroughState__

#include "Params.h"
#include "BoardGameState.h"
#include "BreakthroughMove.h"
#include <stdio.h>
#include <vector>

class BreakthroughState : public BoardGameState {
public:
    
	static Params& getBreakthroughParams();
    
    static GameState* create();
    
    bool moveOK(const GameMove &gm) const override;

	virtual bool makeMove(const BreakthroughMove &gm, 
		std::vector<std::vector<std::vector<long long>>> & zobrist);
    
	virtual bool makeMove(const BreakthroughMove &gm);

	BreakthroughState();

	long long hashVal;


private:
        
    void thisGameMakeMove(const GameMove &gm) override;
    
    Status thisGameCheckTerminalUpdateStatus() override;
	
};


#endif /* defined(__CSE486AIProject__BreakthroughStateState__) */
