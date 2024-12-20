#include<bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
auto my_rand(long long l, long long r) {
    return uniform_int_distribution<long long>(l, r)(rng);
}

void SetColor(int textColor) { // Text Color
    cout << "\033[" << textColor << "m";
}
void ResetColor() { 
    cout << "\033[0m"; 
}

const int Rows = 3, Cols = 3, WinSz = 3, Points = 1000;
const char AI = 'X', You = 'O';

void printBoard(const vector<vector<char>> &board) {
    cout << "   ";
    for (int c = 1; c <= Cols; ++c) cout << c << " ";
    cout << "\n";
    for (int r = 0; r < Rows; ++r) {
        cout << setw(2) << r + 1 << " ";
        SetColor(91);
        for(int c = 0; c < Cols; c++) {
            if(board[r][c] == You) SetColor(92); // Player's color
            else if (board[r][c] == AI) SetColor(91); // AI's color
            else ResetColor();
            cout << board[r][c] << " \n" [c + 1 == Cols];
        }
        ResetColor();
    }
}

bool isValid(int r, int c) {
    return r >= 0 && r < Rows && c >= 0 && c < Cols;
}

bool isWinningMove(const vector<vector<char>> &board, int row, int col, char ch) {
    auto isValid = [&](int r, int c) {
        return r >= 0 && r < Rows && c >= 0 && c < Cols && board[r][c] == ch;
    };

    vector<pair<int, int>> directions = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    for(auto &[dr, dc] : directions) {
        int cnt = 1;
        for(int i = 1; i < WinSz && cnt < WinSz; i++) {
            if (isValid(row + i * dr, col + i * dc)) cnt++;
            else break;
        }
        for(int i = 1; i < WinSz && cnt < WinSz; i++) {
            if (isValid(row - i * dr, col - i * dc)) cnt++;
            else break;
        }
        if(cnt >= WinSz) return true;
    }
    return false;
}

bool isBoardFull(const vector<vector<char>> &board) {
    for(int r = 0; r < Rows; r++) {
        for(int c = 0; c < Cols; c++) {
            if(board[r][c] == '.') return false;
        }
    }
    return true;
}

int findWinningMove(const vector<vector<char>> &board) { // +Points for AI win, -Points for Human win, 0 otherwise
    for(int r = 0; r < Rows; r++) {
        for(int c = 0; c < Cols; c++) {
            if(board[r][c] != '.' && isWinningMove(board, r, c, board[r][c])) {
                return board[r][c] == AI ? +Points : -Points;
            }
        }
    }
    return 0;
}

int minimax(vector<vector<char>> &board, bool isMaximizing) {
    int score = findWinningMove(board);
    if(abs(score) == Points or isBoardFull(board)) return score;
    int bestScore = (isMaximizing ? INT_MIN : INT_MAX);
    for (int r = 0; r < Rows; r++) {
        for (int c = 0; c < Cols; c++) {
            if (board[r][c] == '.') {
                board[r][c] = isMaximizing ? AI: You;
                int score = minimax(board, !isMaximizing);
                board[r][c] = '.';
                if (isMaximizing) bestScore = max(bestScore, score);
                else bestScore = min(bestScore, score);
            }
        }
    }
    return bestScore;
}

pair<int, int> findBestMove(vector<vector<char>> &board) {
    map<int, vector<pair<int, int>>> scores;
    for(int r = 0; r < Rows; r++) {
        for(int c = 0; c < Cols; c++) {
            if(board[r][c] == '.') {
                if(isWinningMove(board, r, c, AI)) {
                    return {r, c};
                }
                board[r][c] = AI;
                int score = minimax(board, false);
                board[r][c] = '.';
                scores[score].push_back({r, c});
            }
        }
    }
    auto &bestMoves = prev(scores.end())->second;
    // for(auto &i: bestMoves) cout << i << " "; cout << endl;
    return bestMoves[my_rand(0, int(bestMoves.size()) - 1)];
}

void printAns(const vector<vector<char>> &board, int color) {
    for(int r = 0; r < Rows; r++) {
        for(int c = 0; c < Cols; c++) {
            if(board[r][c] != '.' && isWinningMove(board, r, c, board[r][c])) {
                SetColor(color);
                cout << board[r][c] << " ";
                ResetColor();
            }
            else {
                cout << board[r][c] << " ";
            }
        }
        cout << endl;
    }
}

void ticTacToc() {
    vector<vector<char>> board(Rows, vector<char> (Cols, '.'));
    bool isPlayerTurn = my_rand(0, 1);
    SetColor(92); cout << "--- Welcome to Connect Four! ---\n"; ResetColor();
    SetColor(35); cout << (isPlayerTurn ? "=> Your turn first!\n" : "=> AI starts first!\n"); ResetColor();

    while(true) {
        printBoard(board);
        if(isPlayerTurn) {
            int r, c;
            cout << "Enter your move(Row, Colnum): ";
            cin >> r >> c; --r, --c;
            if(isValid(r, c) == 0 or board[r][c] != '.') {
                SetColor(31); cout << "Invalid move. Try again.\n"; ResetColor();
                continue;
            }
            board[r][c] = You; // makeMove
            if(isWinningMove(board, r, c, You)) {
                // printBoard(board);
                cout << endl;
                printAns(board, 32);

                SetColor(35); cout << "==> Congratulations, You Win! <==\n\n"; ResetColor();
                break;
            }
            else cout << endl;
        }
        else {
            cout << "AI is making its move...\n";
            auto [r, c] = findBestMove(board);
            board[r][c] = AI;
            cout << "AI plays: " << r + 1 << " " << c + 1 << "\n";
            if(isWinningMove(board, r, c, AI)) {
                // printBoard(board);
                cout << endl;
                printAns(board, 32);

                SetColor(35); cout << "==> AI Wins! Better luck next time. <==\n\n"; ResetColor();
                break;
            }
            else cout << endl;
        }

        if(isBoardFull(board)) {
            printBoard(board);
            SetColor(94); cout << "\n=> It's a draw!\n\n"; ResetColor();
            break;
        }
        isPlayerTurn ^= 1;
    }
    return;
}

int32_t main() {
    ticTacToc();
    return 0;
}
