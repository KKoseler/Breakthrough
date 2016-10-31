#include "AlphaBetaBreakThroughPlayer.h"

AlphaBetaBreakThroughPlayer::AlphaBetaBreakThroughPlayer(std::string nickname, int d)
	: GamePlayer(nickname, "Breakthrough"), depthLimit(d) {}

GameMove* 
AlphaBetaBreakThroughPlayer::getMove(GameState &state,
	const std::string &lastMv) {
	BreakthroughState st = static_cast<BreakthroughState&>(state);
	//Zmuda's code collects all  possible moves and then chooses
	//from among them at random
	std::vector<BreakthroughMove> mvArray;
	for (int r = 0; r < st.ROWS; r++) {
		for (int c = 0; c < st.COLS; c++) {
			int rowDelta = state.getWho() == Who::HOME ? +1 : -1;
			for (int dc = -1; dc <= +1; dc++) {
				BreakthroughMove mv(r, c, r + rowDelta, c + dc);
				if (state.moveOK(mv)) {
					mvArray.push_back(BreakthroughMove(mv));
				}
			}
		}
	}
	return new BreakthroughMove(mvArray[rand() % mvArray.size()]);
}

int
AlphaBetaBreakThroughPlayer::diagonalPath(BreakthroughState &brd, int row, int col) {
	int leftDiagPath = 0; int rightDiagPath = 0; int twoDiagPath = 0;
	bool isTwoDiagPath = false; bool isLeftDiagPath = false; bool isRightDiagPath = false;
	// Evaluate piece for home system.
	if (brd.getCurPlayerSym() == brd.HOMESYM) {
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
			(brd.posOK(row + 2, col - 2) && brd.getCell(row + 2, col + 2) == brd.EMPTYSYM)) && (isTwoDiagPath)) {
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
AlphaBetaBreakThroughPlayer::numberOfEmptyColumns(BreakthroughState &brd) {
	int counter = brd.COLS;
	// Count the nubmer of empty columns. Counter starts at the board's number of columns
	// and assumes all columns are filled. The counter decrements when it finds
	// the current player in the column.
	for (int col = 0; col < brd.COLS; col++) {
		for (int row = 0; row < brd.ROWS; row++) {
			if (brd.getCell(row, col) == brd.getCurPlayerSym()) {
				counter--;
				break;
			}
		}
	}
	return counter;
}

int
AlphaBetaBreakThroughPlayer::threatPosition(BreakthroughState &brd, int row, int col) {
	int threatScore = 0;
	// Evaluate piece for home system.
	if (brd.getCurPlayerSym() == brd.HOMESYM) {
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
AlphaBetaBreakThroughPlayer::defensivePosition(BreakthroughState &brd, int row, int col) {
	int threePieceBlockScore = 0; 
	// Evaluate piece for home system.
	if (brd.getCurPlayerSym() == brd.HOMESYM) {
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
AlphaBetaBreakThroughPlayer::pieceNearEndPosition(BreakthroughState &brd, int row, int col) {
	int dangerousPosition = 0;
	// Evaluate piece for home system.
	if (brd.getCurPlayerSym() == brd.HOMESYM) {
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
AlphaBetaBreakThroughPlayer::evaluatePiece(BreakthroughState &brd, int row, int col) {
	int score = 0; int distanceScore = 0; int pathScore = 0;
	int oneStepToWin = 0; int blockingScore = 0; 
	int threatScore = 0; int dangerScore = 0;
	pathScore = diagonalPath(brd, row, col);
	blockingScore = defensivePosition(brd, row, col);
	threatScore = -threatPosition(brd, row, col);
	dangerScore = -pieceNearEndPosition(brd, row, col);
	// Evaluate piece for home system.
	if (brd.getCurPlayerSym() == brd.HOMESYM) {
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
	int total = 0; int score = 0; int winScore = 0; int emptyColumns = 0;
	bool hasWinPiece = false;
	// Loop through the board and evaluate each piece on the board.
	// Assign a value to the board based on the current state.
	for (int row = 0; row < brd.ROWS; row++) {
		for (int col = 0; col < brd.COLS; col++) {
			if (brd.getCell(row, col) == brd.getCurPlayerSym()) {
				score += evaluatePiece(brd, row, col);
				if ( (brd.getCurPlayerSym() == brd.HOMESYM) && (row == brd.ROWS-1) ) {
					hasWinPiece = true;
				} 
				//I think this can be an else, not sure how much this actually saves us,
				//but I think every little bit of efficiency counts on this one
				else if( (brd.getCurPlayerSym() == brd.AWAYSYM) && (row == 0) ) {
					hasWinPiece = true;
				}
			}
		}
	}
	// The state with a winning piece is assigned a high score.
	if (hasWinPiece) {
		winScore = 1000;
	}
	// Count the number of empty columns.
	emptyColumns = -numberOfEmptyColumns(brd)*10;
	total = score + winScore + emptyColumns;
	return total;
}

int
AlphaBetaBreakThroughPlayer::negaMax(BreakthroughState brd, int currDepth, int alpha, int beta) {

	return 0;
}