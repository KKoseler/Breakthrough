#define NOMINMAX
#include "AlphaBetaBreakThroughPlayer.h"
#include <algorithm>

AlphaBetaBreakThroughPlayer::AlphaBetaBreakThroughPlayer(std::string nickname, int d)
	: GamePlayer(nickname, "Breakthrough"), depthLimit(d) {
	//the ourSymbol field is simply the character of our current side
	//the home boolean variable is self-explanatory
}

GameMove* 
AlphaBetaBreakThroughPlayer::getMove(GameState &state,
	const std::string &lastMv) {
	
	BreakthroughState st = static_cast<BreakthroughState&>(state);
	
	//set our global variables
	ourSymbol = GameState::who2str(getSide()) == "HOME" ? 'W' : 'B';
	home = ourSymbol == 'W' ? true : false;
	
	BreakthroughMove* finalMove;

	std::pair<int, BreakthroughMove> move = negaMax(st, depthLimit, 0, INT_MIN, INT_MAX, true);
	/*if (state.moveOK(std::get<1>(move)))
		std::cout << "MOVE IS OK\n";

	std::cout << "RIGHT BEFORE RETURN OF FINAL MOVE\n";*/
	finalMove = new BreakthroughMove(std::get<1>(move));
	//std::cout << finalMove->toString();
	return finalMove;
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

	for (int r = 0; r < st.ROWS; r++) {
		for (int c = 0; c < st.COLS; c++) {
			char current = st.getCell(r, c);
			if (current == sideToMove) { //only get moves if piece is of side to move
				for (int dc = -1; dc <= +1; dc++) {
					if (st.posOK(r + rowDelta, c + dc)) {
						char potentialMove = st.getCell(r + rowDelta, c + dc);
						//if the cell we can move to is empty or has opponent's pieces
						if (potentialMove == '.' || (potentialMove == opponentSide && dc != 0)) {
							toAdd = BreakthroughMove(r, c, r + rowDelta, c + dc);
							moves.push_back(toAdd);
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
		if ((brd.posOK(row + 1, col - 1) && brd.getCell(row + 1, col - 1) == brd.EMPTYSYM) &&
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
		if ((brd.posOK(row - 1, col - 1) && brd.getCell(row - 1, col - 1) == brd.EMPTYSYM) &&
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
			threePieceBlockScore = 8;
		}
	}
	return threePieceBlockScore;
}

int
AlphaBetaBreakThroughPlayer::pieceNearEndPosition(BreakthroughState &brd, char who, int row, int col) {
	int dangerousPosition = 0;
	// Evaluate piece for home system.
	if (who == brd.HOMESYM) {
		if ((brd.getCell(row, col) == brd.AWAYSYM) && (row == 1)) {
			dangerousPosition = 100;
		}
	} else { // Evaluate piece for away system.
		if ((brd.getCell(row, col) == brd.HOMESYM) && (row == brd.ROWS - 2)) {
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
	score = pathScore + blockingScore + threatScore + dangerScore + distanceScore + oneStepToWin;
	return score;
}

int
AlphaBetaBreakThroughPlayer::evaluateBoard(BreakthroughState &brd) {
	int total = 0; int evalScore1 = 0; int evalScore2 = 0; int homeWinScore = 0;
	int awayWinScore = 0; int homeEmptyColumns = 0; int awayEmptyColumns = 0; 
	bool homeHasWinPiece = false; bool awayHasWinPiece = false;
	int homePieces = 0; int awayPieces = 0;
	// Loop through the board and evaluate each piece on the board.
	// Assign a value to the board based on the evaluation of Home and Away pieces.
	// Count the number of Home and Away pieces.
	for (int row = 0; row < brd.ROWS; row++) {
		for (int col = 0; col < brd.COLS; col++) {
			char current = brd.getCell(row, col); 
			if (current == brd.HOMESYM) { //if it's our piece
				evalScore1 += evaluatePiece(brd, current, row, col);
				homePieces++;
				if ( (current == brd.HOMESYM) && (row == brd.ROWS-1) ) { // if home has a win
					homeHasWinPiece = true;
				} 
			}
			else if(current == brd.AWAYSYM) {
				evalScore2 += evaluatePiece(brd, current, row, col);
				awayPieces++;
				if ( (current == brd.AWAYSYM) && (row == 0)) { //if away has a win
					awayHasWinPiece = true;
				}
			}
		}
	}
	// A high score is assigned when a side has the winning piece or the their opponent has no pieces remaining. 
	if (homeHasWinPiece || (awayPieces == 0)) {
		homeWinScore = 2000;
	}
	if (awayHasWinPiece || (homePieces == 0)) {
		awayWinScore = 2000;
	}
	// Count the number of empty columns from Home perspective.
	homeEmptyColumns = -numberOfEmptyColumns(brd, brd.HOMESYM)*10;
	// Count the number of empty columns from Away perspective.
	awayEmptyColumns = -numberOfEmptyColumns(brd, brd.AWAYSYM)*10;
	total = (evalScore1 + homeWinScore + homeEmptyColumns) - (evalScore2 + awayWinScore + awayEmptyColumns);
	return home ? total : -total;
}

std::pair<int, BreakthroughMove>
AlphaBetaBreakThroughPlayer::negaMax(BreakthroughState &brd, int maxDepth, int currDepth, int alpha, int beta, bool max) {
	//check if we're done recursing
	if (brd.checkTerminalUpdateStatus() || currDepth == maxDepth) {
		/*std::cout << "END OF RECURSION \n" << brd.toDisplayStr() << "\nSCORE OF ABOVE: " 
			<< evaluateBoard(brd) << "\n" << std::endl;*/
		if (max)
			return std::make_pair(evaluateBoard(brd), BreakthroughMove(-1, -1, -1, -1));
		else
			return std::make_pair(-evaluateBoard(brd), BreakthroughMove(-1, -1, -1, -1));
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
	for (BreakthroughMove move : moves) {
		BreakthroughState newBoard = brd;
		newBoard.makeMove(move);
		std::pair<int, BreakthroughMove> scoreAndMove = negaMax(newBoard, maxDepth, 
			currDepth + 1, -beta, -std::max(alpha, bestScore), !max);
		int currentScore = -scoreAndMove.first;

		if (currentScore > bestScore) {
			bestScore = currentScore;
			bestMove = move;
			
			//the pruning condition
			if (bestScore >= beta)
				return std::make_pair(bestScore, bestMove);
		}
	}

	return std::make_pair(bestScore, bestMove);
}