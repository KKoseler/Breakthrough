//
//  BreakthroughStateState.cpp
//  CSE486AIProject
//
//  Created by Yunlong Nick Liu on 5/29/15.
//  Copyright (c) 2015 Yunlong Nick Liu. All rights reserved.
//

#include "BreakthroughState.h"
#include <cstdlib>
#include <algorithm>

GameState* BreakthroughState::create() {
	return new BreakthroughState();
}

Params& BreakthroughState::getBreakthroughParams() {
	static Params* breakthroughPrms = new Params(std::string("config") + Params::separatorChar + "breakthrough.txt");
	return *breakthroughPrms;
}

BreakthroughState::BreakthroughState() : BoardGameState
    (getBreakthroughParams().intValue("ROWS"),
	getBreakthroughParams().intValue("COLS"),
	getBreakthroughParams().charValue("HOMESYM"),
	getBreakthroughParams().charValue("AWAYSYM"),
	getBreakthroughParams().charValue("EMPTYSYM")) 
{
	hashVal = 0;
}

bool BreakthroughState::moveOK(const GameMove &gm) const {
	
	const BreakthroughMove mv = static_cast<const BreakthroughMove&>(gm);
    int rowDiff = mv.row2() - mv.row1();
    int colDiff = mv.col2() - mv.col1();
	char playerSymbol = getWho() == Who::HOME ? HOMESYM : AWAYSYM;
	char oppSymbol = getWho() == Who::HOME ? AWAYSYM : HOMESYM;

	bool a = getStatus() == Status::GAME_ON;
	bool b = posOK(mv.row1(), mv.col1()) && posOK(mv.row2(), mv.col2());
	bool c = ((getWho() == Who::HOME && rowDiff == +1) || (getWho() == Who::AWAY && rowDiff == -1));
	bool d = board[mv.row1()*COLS + mv.col1()] == playerSymbol;

	if (getStatus() == Status::GAME_ON &&
		posOK(mv.row1(), mv.col1()) && posOK(mv.row2(), mv.col2()) &&
		((getWho() == Who::HOME && rowDiff == +1) || (getWho() == Who::AWAY && rowDiff == -1)) &&
		board[mv.row1()*COLS + mv.col1()] == playerSymbol) {
			return (colDiff == 0 && board[mv.row2()*COLS + mv.col2()] == EMPTYSYM) ||
				(std::abs(colDiff) == 1 && board[mv.row2()*COLS + mv.col2()] != playerSymbol);
	}
    return false;
}

bool BreakthroughState::makeMove(const BreakthroughMove & gm, std::vector<std::vector<std::vector<long long>>>& zobrist)
{
	//if we've already calculated a hash value update it incrementally
	//otherwise, it will be calculated at another point
	if (hashVal != 0) {
		int oldRow = gm.row1();
		int newRow = gm.row2();
		int oldCol = gm.col1();
		int newCol = gm.col2();
		int pieceIndex = this->getCurPlayerSym() == 'W' ? 0 : 1;
		hashVal ^= zobrist[pieceIndex][oldRow][oldCol];
		hashVal ^= zobrist[pieceIndex][newRow][newCol];
		//if it's a capture for black, remove the white piece
		if (gm.isCaptureForB) {
			hashVal ^= zobrist[0][newRow][newCol];
		}
		//if it's a capture for white, remove the black piece
		else if (gm.isCaptureForW) {
			hashVal ^= zobrist[1][newRow][newCol];
		}
	}
	return GameState::makeMove(gm, false, false);
}

bool BreakthroughState::makeMove(const BreakthroughMove & gm)
{
	return GameState::makeMove(gm);
}

void BreakthroughState::thisGameMakeMove(const GameMove &gm) {
	const BreakthroughMove &mv = static_cast<const BreakthroughMove&>(gm);
    char playerSymbol = getWho() == Who::HOME ? HOMESYM : AWAYSYM;
	board[mv.row1()*COLS+mv.col1()] = EMPTYSYM;
    board[mv.row2()*COLS+mv.col2()] = playerSymbol;
}

Status BreakthroughState::thisGameCheckTerminalUpdateStatus() {
	char playerSymbol = getWho() == Who::HOME ? HOMESYM : AWAYSYM;
	char oppSym = getWho() == Who::HOME ? AWAYSYM : HOMESYM;

	if (std::count(board.begin(), board.end(), HOMESYM) == 0) return Status::AWAY_WIN;
	else if (std::count(board.begin(), board.end(), AWAYSYM) == 0) return Status::HOME_WIN;

	if (getWho() == Who::HOME) {
		return std::any_of(&board[0], &board[COLS], [&](const char &ch) { return ch == oppSym; }) ? Status::AWAY_WIN : Status::GAME_ON;
    } else {
		return std::any_of(&board[(ROWS - 1)*COLS], &board[(ROWS - 1)*COLS]+COLS, [&](const char &ch) { return ch == oppSym; }) ? Status::HOME_WIN : Status::GAME_ON;
	}
}



