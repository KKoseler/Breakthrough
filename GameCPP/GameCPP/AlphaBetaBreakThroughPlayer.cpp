#define NOMINMAX
#include "AlphaBetaBreakThroughPlayer.h"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <random>
#include <cfenv>
#include <climits>
#include <cmath>

AlphaBetaBreakThroughPlayer::AlphaBetaBreakThroughPlayer(std::string nickname, int d)
	: GamePlayer(nickname, "Breakthrough"), depthLimit(d) {
	//the ourSymbol field is simply the character of our current side
	//the home boolean variable is self-explanatory

	std::random_device random;
	std::mt19937 engine(random());
	double lower = std::pow(2, 61);
	double upper = std::pow(2, 62);
	std::uniform_int_distribution<long long>distribution(std::llround(lower), std::llround(upper));
	std::vector<std::vector<std::vector<long long> > > zobrist(2, std::vector<std::vector<long long> >(8, std::vector<long long>(64)));
	zobristkeys = zobrist;
	for (size_t k = 0; k < 2; k++) {
		for (size_t i = 0; i < 8; i++) {
			for (size_t j = 0; j < 8; j++) {
				zobristkeys[k][i][j] = distribution(engine);
			}
		}
	}
	table = TranspositionTable();
}

GameMove* 
AlphaBetaBreakThroughPlayer::getMove(GameState &state,
	const std::string &lastMv) {
	
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	
	BreakthroughState st = static_cast<BreakthroughState&>(state);
	numMoves = st.getNumMoves();
	
	//set our global variables
	ourSymbol = GameState::who2str(getSide()) == "HOME" ? 'W' : 'B';
	home = ourSymbol == 'W' ? true : false;
	
	BreakthroughMove* finalMove;
	int previous = 0;
	for (int i = 2; i <= depthLimit; i++) {
		nodesSearched = 0;
		lowerBoundCuts = 0;
		upperBoundCuts = 0;
		std::pair<int, BreakthroughMove> move = aspiration(st, i, previous);
		//std::pair<int, BreakthroughMove> move = negaMax(st, i, 0, INT_MIN, INT_MAX);
		previous = std::get<0>(move);
		finalMove =  new BreakthroughMove(std::get<1>(move));
		
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::cout << "NUMBER OF NODES SEARCHED AT DEPTH: " << i << "\n"
			<< nodesSearched << std::endl;
		
		/*std::cout << "NUMBER OF UPPER CUTS AT DEPTH: " << i << "\n"
			<< upperBoundCuts << std::endl;
		std::cout << "NUMBER OF LOWER CUTS AT DEPTH: " << i << "\n"
			<< lowerBoundCuts << std::endl;*/

		if (numMoves < 6 && elapsed_seconds.count() > 5)  //WE WILL TWEAK THESE VALUES
			return finalMove;
		else if (numMoves < 23 && elapsed_seconds.count() > 9)
			return finalMove;
		else if (numMoves >= 23 && elapsed_seconds.count() > 6)
			return finalMove;
	}
	numMoves++;
	std::cout << "NUM MOVES: " << numMoves << std::endl;
	return finalMove;
}

std::pair<int, BreakthroughMove> AlphaBetaBreakThroughPlayer::aspiration(BreakthroughState & brd, int maxDepth, int previous)
{
	std::pair<int, BreakthroughMove> toReturn;
	int alpha = previous - 1;
	int beta = previous + 1;
	while (true) {
		toReturn = negaMax(brd, maxDepth, 0, alpha, beta);
		if (toReturn.first <= alpha)
			alpha = INT_MIN;
		else if (toReturn.first >= beta)
			beta = INT_MAX;
		else
			return toReturn;
	}
}

//Don't think the GameState from AlphaBetaPlayer is necessary here, no need to evaluate
//if move is acceptable
std::vector<BreakthroughMove> AlphaBetaBreakThroughPlayer::getPossibleMoves
	(BreakthroughState & st, char sideToMove, int depth)
{
	int rowDelta = sideToMove == 'W' ? +1 : -1;
	std::vector<BreakthroughMove> moves;
	char opponentSide = sideToMove == 'W' ? 'B' : 'W';
	BreakthroughMove toAdd;
	if (sideToMove == 'W') {
		for (int r = 0; r < st.ROWS; r++) {
			for (int c = 0; c < st.COLS; c++) {
				char current = st.getCell(r, c);
				if (current == sideToMove) { //only get moves if piece is of side to move
					for (int dc = -1; dc <= +1; dc++) {
						if (st.posOK(r + rowDelta, c + dc)) {
							int newRow = r + rowDelta;
							int newCol = c + dc;
							char potentialMove = st.getCell(newRow, newCol);
							//if the cell we can move to is empty
							if (potentialMove == '.') {
								toAdd = BreakthroughMove(r, c, newRow, newCol);
								if (newRow == 7)
									toAdd.moveOrderVal += 500;
								moves.push_back(toAdd);
							}
							//cell we can move to has opponent's pieces
							else if (potentialMove == opponentSide && dc != 0) {
								toAdd = BreakthroughMove(r, c, newRow, newCol);
								toAdd.isCaptureForW = true;
								toAdd.moveOrderVal += 100;
								if (newRow == 7)
									toAdd.moveOrderVal += 500;
								moves.push_back(toAdd);
							}
						}
					}
				}
			}
		}
	}
	//if we are getting black's moves
	else {
		for (int r = 7; r >= 0; r--) {
			for (int c = 7; c >= 0; c--) {
				char current = st.getCell(r, c);
				if (current == sideToMove) { //only get moves if piece is of side to move
					for (int dc = -1; dc <= +1; dc++) {
						if (st.posOK(r + rowDelta, c + dc)) {
							int newRow = r + rowDelta;
							int newCol = c + dc;
							char potentialMove = st.getCell(newRow, newCol);
							//if the cell we can move to is empty
							if (potentialMove == '.') {
								toAdd = BreakthroughMove(r, c, newRow, newCol);
								if (newRow == 0)
									toAdd.moveOrderVal += 1500;
								moves.push_back(toAdd);
							}
							//cell we can move to has opponent's pieces
							else if (potentialMove == opponentSide && dc != 0) {
								toAdd = BreakthroughMove(r, c, newRow, newCol);
								toAdd.isCaptureForW = true;
								toAdd.moveOrderVal += 100;
								if (newRow == 0)
									toAdd.moveOrderVal += 1500;
								moves.push_back(toAdd);
							}
						}
					}
				}
			}
		}
	}
	/*std::cout << "CURRENT DEPTH: " << depth << std::endl;
	std::cout << "SIDE TO MOVE: " << sideToMove << std::endl;
	std::cout << "NUMBER OF MOVES:" << moves.size() << "\n" << std::endl;*/
	return moves;
}

int
AlphaBetaBreakThroughPlayer::diagonalPath(BreakthroughState &brd, char who, int row, int col) {
	int leftDiagPath = 0; int rightDiagPath = 0; int twoDiagPath = 0;
	bool isTwoDiagPath = false; bool isLeftDiagPath = false; bool isRightDiagPath = false;
	// Evaluate piece for home system.
	if (who == brd.HOMESYM) {
		// There exists only a left diagonal path in one step.
		if ((brd.posOK(row + 1, col - 1) && brd.getCell(row + 1, col - 1) == brd.EMPTYSYM) &&
			((brd.posOK(row + 1, col + 1) && brd.getCell(row + 1, col + 1) != brd.EMPTYSYM) ||
			!brd.posOK(row + 1, col + 1))) {
			leftDiagPath = 7;
			isLeftDiagPath = true;
		}
		// There exists only a left diagonal path with two steps.
		if ((brd.posOK(row + 2, col - 2) && brd.getCell(row + 2, col - 2) == brd.EMPTYSYM) && (isLeftDiagPath)) {
			leftDiagPath = 9;
		}
		// There exists only a right diagonal path in one step.
		if ((brd.posOK(row + 1, col + 1) && brd.getCell(row + 1, col + 1) == brd.EMPTYSYM) &&
			((brd.posOK(row + 1, col - 1) && brd.getCell(row + 1, col - 1) != brd.EMPTYSYM) ||
			!brd.posOK(row + 1, col - 1))) {
			rightDiagPath = 7;
			isRightDiagPath = true;
		}
		// There exists only a right diagonal path with two steps.
		if ((brd.posOK(row + 2, col + 2) && brd.getCell(row + 2, col + 2) == brd.EMPTYSYM) && (isRightDiagPath)) {
			rightDiagPath = 9;
		}
		// There exists a left and right diagonal path.
		if ((brd.posOK(row + 1, col - 1) && brd.getCell(row + 1, col - 1) == brd.EMPTYSYM) ||
			(brd.posOK(row + 1, col + 1) && brd.getCell(row + 1, col + 1) == brd.EMPTYSYM)) {
			twoDiagPath = 10;
			isTwoDiagPath = true;
		}
		// There exists a left and right diagonal path with two steps.
		if (((brd.posOK(row + 2, col - 2) && brd.getCell(row + 2, col - 2) == brd.EMPTYSYM) &&
			(brd.posOK(row + 2, col + 2) && brd.getCell(row + 2, col + 2) == brd.EMPTYSYM)) && (isTwoDiagPath)) {
			twoDiagPath = 15;
		}
	}
	else { // Evaluate piece for away system.
		// There exists only a left diagonal path.
		if ((brd.posOK(row - 1, col - 1) && brd.getCell(row - 1, col - 1) == brd.EMPTYSYM) &&
			((brd.posOK(row - 1, col + 1) && brd.getCell(row - 1, col + 1) != brd.EMPTYSYM) ||
			!brd.posOK(row - 1, col + 1))) {
			leftDiagPath = 7;
			isLeftDiagPath = true;
		}
		// There exists only a left diagonal path with two steps.
		if ((brd.posOK(row - 2, col - 2) && brd.getCell(row - 2, col - 2) == brd.EMPTYSYM) && (isLeftDiagPath)) {
			leftDiagPath = 9;
		}
		// There exists only a right diagonal path.
		if ((brd.posOK(row - 1, col + 1) && brd.getCell(row - 1, col + 1) == brd.EMPTYSYM) &&
			((brd.posOK(row - 1, col - 1) && brd.getCell(row - 1, col - 1) != brd.EMPTYSYM) ||
			!brd.posOK(row - 1, col - 1))) {
			rightDiagPath = 7;
			isRightDiagPath = true;
		}
		// There exists only a right diagonal path with two steps.
		if ((brd.posOK(row - 2, col + 2) && brd.getCell(row - 2, col + 2) == brd.EMPTYSYM) && (isRightDiagPath)) {
			rightDiagPath = 9;
		}
		// There exists a left and right diagonal path.
		if ((brd.posOK(row - 1, col - 1) && brd.getCell(row - 1, col - 1) == brd.EMPTYSYM) ||
			(brd.posOK(row - 1, col + 1) && brd.getCell(row - 1, col + 1) == brd.EMPTYSYM)) {
			twoDiagPath = 10;
			isTwoDiagPath = true;
		}
		// There exists a left and right diagonal path with two steps.
		if (((brd.posOK(row - 2, col - 2) && brd.getCell(row - 2, col - 2) == brd.EMPTYSYM) &&
			(brd.posOK(row - 2, col + 2) && brd.getCell(row - 2, col + 2) == brd.EMPTYSYM)) && (isTwoDiagPath)) {
			twoDiagPath = 15;
		}
	}
	return (leftDiagPath + rightDiagPath + twoDiagPath);
}

int
AlphaBetaBreakThroughPlayer::numberOfEmptyColumns(BreakthroughState &brd, char who) {
	int counter = brd.COLS;
	// Count the nubmer of empty columns. Counter starts at the board's number of columns
	// and assumes all columns are filled. The counter decrements when it finds
	// the current player in the column.
	for (int col = 0; col < brd.COLS; col++) {
		for (int row = 0; row < brd.ROWS; row++) {
			if (brd.getCell(row, col) == who) {
				counter--;
				break;
			}
		}
	}
	return counter;
}

int
AlphaBetaBreakThroughPlayer::threatPosition(BreakthroughState &brd, char who, int row, int col) {
	int threatScore = 0;
	// Evaluate piece for home system.
	if (who == brd.HOMESYM) {
		// Determine if there is an opponent piece in the left diagonal or right diagonal positions.
		if ((brd.posOK(row + 1, col + 1) && brd.getCell(row + 1, col - 1) == brd.AWAYSYM)
			|| (brd.posOK(row + 1, col + 1) && brd.getCell(row + 1, col + 1) == brd.AWAYSYM)) {
			threatScore = 20;
		}
	} else { // Evaluate piece for aways system.
		// Determine if there is an opponent piece in the left diagonal or right diagonal positions.
		if ((brd.posOK(row + 1, col + 1) && brd.getCell(row + 1, col + 1) == brd.HOMESYM)
			|| (brd.posOK(row + 1, col - 1) && brd.getCell(row + 1, col - 1) == brd.HOMESYM)) {
			threatScore = 20;
		}
	}
	return threatScore;
}

int
AlphaBetaBreakThroughPlayer::defensivePosition(BreakthroughState &brd, char who, int row, int col) {
	int threePieceBlockScore = 0; 
	// Evaluate piece for home system.
	if (who == brd.HOMESYM) {
		// Determine if there is a three piece triangle block.
		if ( (brd.posOK(row - 1, col - 1) && brd.getCell(row - 1, col - 1) == brd.HOMESYM)
			&& (brd.posOK(row - 1, col + 1) && brd.getCell(row - 1, col + 1) == brd.HOMESYM) ) {
			threePieceBlockScore = 8;
		}
	} else { // Evaluate piece for away system.
		// Determine if there is a three piece triangle block.
		if ((brd.posOK(row + 1, col - 1) && brd.getCell(row + 1, col - 1) == brd.AWAYSYM)
			&& (brd.posOK(row + 1, col + 1) && brd.getCell(row + 1, col + 1) == brd.AWAYSYM)) {
			threePieceBlockScore = 8 * 20;
		}
	}
	return threePieceBlockScore;
}

int
AlphaBetaBreakThroughPlayer::pieceNearEndPosition(BreakthroughState &brd, char who, int row, int col) {
	int dangerousPosition = 0;
	// add danger for when 2-3 rows away as well. Already pretty late when one away.
	// Evaluate piece for home system.
	if (who == brd.HOMESYM) {
		if ((brd.getCell(row, col) == brd.AWAYSYM) && (row <= 2)) {
			dangerousPosition = 100;
		}
	} else { // Evaluate piece for away system.
		if ((brd.getCell(row, col) == brd.HOMESYM) && (row >= brd.ROWS - 3)) {
			dangerousPosition = 100;
		}
	}
	return dangerousPosition;
}

int
AlphaBetaBreakThroughPlayer::evaluatePiece(BreakthroughState &brd, char who, int row, int col) {
	int score = 0; int distanceScore = 0; int pathScore = 0;
	int oneStepToWin = 0; int blockingScore = 0; 
	int threatScore = 0; int dangerScore = 0;
	pathScore = diagonalPath(brd, who, row, col);
	blockingScore = defensivePosition(brd, who, row, col);
	threatScore = -threatPosition(brd, who, row, col);
	dangerScore = -pieceNearEndPosition(brd, who, row, col);
	// Evaluate piece for home system.
	if (who == brd.HOMESYM) {
		// Assign value based on distance from goal.
		distanceScore = row + 1;
		if (row == (brd.ROWS - 2)) {
			oneStepToWin = 75;
		}
	} else { // Evaluate piece for away system.
		// Assign value based on distance from goal.
		distanceScore = ((brd.ROWS - 1) - row) + 1;
		if (row == (brd.ROWS - brd.ROWS) + 1) {
			oneStepToWin = 75;
		}
	}
	score = pathScore * 10 + blockingScore * 30 + threatScore * 20 + dangerScore * 50 + distanceScore * 5 + oneStepToWin;
	return score;
}

int
AlphaBetaBreakThroughPlayer::evaluateBoard(BreakthroughState &brd) {
	//Let's check for a winning piece before we do anything else,
	//very expensive to loop through board and evaluate all pieces. 
	//If piece is winning, return right away. 
	if (brd.checkTerminalUpdateStatus()) {
		//char winningSide;
		char current;
		//loop through end rows and determine if there is a piece
		for (int i = 0; i < brd.COLS; i++) {
			//check home's row
			current = brd.getCell(0, i);
			//black has a victory
			if (current == 'B')
				return brd.getCurPlayerSym() == 'W' ? -50000 : 50000;
			
			//check away's row
			current = brd.getCell(7, i);
			//white has a victory
			if (current == 'W')
				return brd.getCurPlayerSym() == 'W' ? 50000 : -50000;
		}
	}

	int evalScore1 = 0; int evalScore2 = 0;
	int homePieces = 0; int awayPieces = 0;
	// Loop through the board and evaluate each piece on the board.
	// Assign a value to the board based on the evaluation of Home and Away pieces.
	// Count the number of Home and Away pieces.
	for (int row = 0; row < brd.ROWS; row++) {
		for (int col = 0; col < brd.COLS; col++) {
			char current = brd.getCell(row, col); 
			if (current == brd.HOMESYM) { //if it's our piece
				int scoreHome = evaluatePiece(brd, current, row, col);
				evalScore1 += scoreHome;
				homePieces++;
			}
			else if(current == brd.AWAYSYM) {
				int awayScore = evaluatePiece(brd, current, row, col);
				evalScore2 += awayScore;
				awayPieces++;
			}
		}
	}
	// Count the number of empty columns from Home perspective.
	//int homeEmptyColumns = 0;
	int homeEmptyColumns = -numberOfEmptyColumns(brd, brd.HOMESYM)*10;
	// Count the number of empty columns from Away perspective.
	//int awayEmptyColumns = 0;
	int awayEmptyColumns = -numberOfEmptyColumns(brd, brd.AWAYSYM)*10;
	int total = (evalScore1 + homeEmptyColumns + homePieces * 500) - (evalScore2 + awayEmptyColumns + awayPieces * 500);
	//total *= 20;
	return brd.getCurPlayerSym() == 'W' ? total : -total;
}

long long
AlphaBetaBreakThroughPlayer::zobristHash(BreakthroughState &brd) {
	//only do this if we don't already have a hash for this position
	if (brd.hashVal == 0) {
		long long hash = 0;
		for (int i = 0; i < brd.ROWS; i++) {
			for (int j = 0; j < brd.COLS; j++) {
				if (brd.getCell(i, j) == 'W') {
					hash ^= zobristkeys[0][i][j];
				}
				else if (brd.getCell(i, j) == 'B') {
					hash ^= zobristkeys[1][i][j];
				}
			}
		}
		brd.hashVal = hash;
		return hash;
	}
	//return what we already have in the board
	else
		return brd.hashVal;
}

std::pair<int, BreakthroughMove>
AlphaBetaBreakThroughPlayer::negaMax(BreakthroughState &brd, int maxDepth, int currDepth, int alpha, int beta) {
	nodesSearched++;
	TableEntry *entry;
	long long boardHashValue = zobristHash(brd);
	entry = table.lookup(boardHashValue);
	//if entry exists
	if (entry->getKey() != 0) 
	{
		//std::cout << "ENTRY EXISTS!!" << std::endl;
		//std::cout << "ENTRY DEPTH: " << entry.getDepth() << "\n"
			//<< "CURRENT DEPTH: " << currDepth << std::endl;
		if (entry->getDepth() >= (maxDepth - currDepth)) {
			//std::cout << "MET THE CUTTING CONDITION!" << std::endl;
			//We leave early for stored positions

			if (entry->getType() == 0) //exact 
				return std::make_pair(entry->getScore(), entry->getMove());
			else if (entry->getType() == 1) //lower bound
				alpha = std::max(alpha, entry->getScore());
			else //upper bound
				beta = std::min(beta, entry->getScore());
		}
		if (alpha >= beta)
			return std::make_pair(entry->getScore(), entry->getMove());
	}
	else {
		//we must create the entry, at first with an invalid move
		//std::cout << "HAD TO CREATE ENTRY";
		entry->setKey(boardHashValue);
		entry->setDepth(maxDepth - currDepth);
	}

	//check if we're done recursing
	bool terminalState = brd.checkTerminalUpdateStatus();
	if (terminalState || currDepth == maxDepth) {
		int eval = evaluateBoard(brd);
		
		//PENALIZE DEEPER WINS
		if (terminalState) {
			if ((currDepth - 1) % 2 == 0)
				eval += (currDepth * 26);
			else
				eval -= (currDepth * 26);
		}

		/*entry->setMin(eval);
		entry->setMax(eval);
		table.insert(*entry);*/
		return std::make_pair(eval, BreakthroughMove(-1, -1, -1, -1));
	}

	BreakthroughMove bestMove;
	int bestScore = INT_MIN;
	char sideToMove;

	//check which side's moves we need to obtain

	//if it's our turn to move
	if (currDepth % 2 == 0) {
		if (ourSymbol == 'W')
			sideToMove = 'W';
		else //we are the away side
			sideToMove = 'B';
	}
	
	else { //it's opponent's turn to move
		if (ourSymbol == 'W')
			sideToMove = 'B';
		else // we are the away side
			sideToMove = 'W';
	}

	std::vector<BreakthroughMove> moves = getPossibleMoves(brd, sideToMove, currDepth);
	moves = orderMoves(moves, *entry, sideToMove);
	/*if (currDepth == 0 && !home) {
		for (auto move : moves) {
			BreakthroughState testBoard = brd;
			testBoard.makeMove(move);
			std::cout << "BOARD AT DEPTH 0: \n" << testBoard.toDisplayStr() << "\n"
				<< "EVALUATION OF ABOVE BOARD: " << evaluateBoard(testBoard) << "\n"
				<< std::endl;
		}
	}*/
	for (BreakthroughMove move : moves) {
		BreakthroughState newBoard = brd;
		newBoard.makeMove(move, zobristkeys);
		//newBoard.makeMove(move);

		//recurse
		std::pair<int, BreakthroughMove> scoreAndMove = negaMax(newBoard, maxDepth, currDepth + 1, -beta, 
			-std::max(alpha, bestScore));
		int currentScore = -scoreAndMove.first;

		/*std::cout << "WHO MADE THE MOVE: " << sideToMove << "\n" << "CURRENT DEPTH: "
			<< currDepth << std::endl;
		std::cout << "MOVE BEING EXAMINED: " << move.toString() << std::endl;
		std::cout << "SCORE OF ABOVE MOVE: " << currentScore << "\n" << std::endl;*/

		if (currentScore > bestScore) {
			entry->setMove(move);
			bestScore = currentScore;
			bestMove = move;
		}
		/*if (currDepth == 0 && numMoves <= 2) {
			std::cout << "BOARD AT DEPTH 0: \n" << newBoard.toDisplayStr() << "\n"
				<< "EVALUATION OF ABOVE BOARD: " << evaluateBoard(newBoard) << "\n"
				<< "SCORE OF CURRENT BOARD: " << currentScore << "\n" << std::endl;
		}*/
		if (bestScore >= beta)
			//return std::make_pair(bestScore, bestMove);
			break;
	}

	entry->setScore(bestScore);

	if (bestScore <= alpha)
		entry->setType(2); //set upperBound
	else if (bestScore >= beta)
		entry->setType(1); //set lowerBound
	else
		entry->setType(0); //set exact

	//debug stuff
	/*std::cout << "CURRENT DEPTH: " << currDepth << std::endl;
	std::cout << "CURRENT PLAYER: " << sideToMove << std::endl;
	std::cout << "BEST MOVE: " << bestMove.toString() << std::endl;
	std::cout << "SCORE OF ABOVE MOVE: " << bestScore << "\n" << std::endl;*/

	table.insert(*entry);
	return std::make_pair(bestScore, bestMove);
}

std::vector<BreakthroughMove>
AlphaBetaBreakThroughPlayer::orderMoves(std::vector<BreakthroughMove> & moves, 
	TableEntry & entry, char sideToMove) {
	BreakthroughMove hashMove = entry.getMove();
	//if we have a valid hashMove
	if (hashMove.col1() != -1) {
		for (int i = 0; i < moves.size(); i++) {
			BreakthroughMove move = moves[i];
			if (move.col1() == hashMove.col1() && move.col2() == hashMove.col2()
				&& move.row1() == hashMove.row1() && move.row2() == hashMove.row2())
				move.moveOrderVal += 1000;
		}
	}
	std::sort(moves.begin(), moves.end(), [](BreakthroughMove & a, BreakthroughMove & b)
	{ return a.moveOrderVal > b.moveOrderVal; });
	return moves;
}