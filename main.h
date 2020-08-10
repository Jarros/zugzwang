#pragma once



using namespace std;

typedef unsigned char byte;
#define board(i,j)   board.at((j)*8 + (i))
#define skip if(false)

const int board_size = 8 * 8;
array<byte, board_size> board;

enum e_piece {
	null, pawn, ladya, knight, bishop, queen, king,
	pawnB, ladyaB, knightB, bishopB, queenB, kingB,
};
enum e_rule_result {
	ERR_RULE, MOV, EAT, EAT_ADV
};
enum e_move_result {
	ERR_MOVE, UPD, PASS, UPD_NORULES
};
enum e_team {
	TEAM_NULL, TEAM_WHITE, TEAM_BLACK
};

char ui_cells[13] =
{
	' ', 'o', 'D', 'L', 'X', 'Q', 142/*154 232 'K'*/,
		 'o', 'D', 'L', 'X', 'Q', 142//154/*232 'K'*/,
};

char ui_square_white = 178;
char ui_square_gray = 177;
char ui_square_graydark = 176;
char ui_brackets = 175;
char ui_bracketsB = ')';
char ui_space = ' ';
char ui_bar_ang = 201;
char ui_bar_v = 186;
char ui_bar_h = 205;
char ui_bar_vl = 221;

const char* ui_rows[8] = { "1", "2", "3", "4", "5", "6", "7", "8" };
const char* ui_cols[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };

const char* xoffset_l = "  ";
const char* xoffset_l_big = " ";
const char* xoffset_r = "   ";
string ui_lines[10];

const char postfixW = 223; //254;//223 - white square uphalf
const char postfixB = ui_space;// 217; //'\'';// 252;// ui_space; //191
const char marker_out_v = '<';
const char marker_in_v = '>';
const char marker_0_v = ':';
const char marker_out_h[3] = { '^', ui_bar_h, '\0' };//{ '/', 92, '\0' }; //"ab";//
const char marker_in_h[3] = { 251, ui_bar_h, '\0' };//{ 92, '/', '\0' };
const char* marker_0_h = ".."; //"cd";// 

int teamactive = e_team::TEAM_WHITE;
//0 - no game, 1 - white, 2 - black

byte marker_out = -1;
byte marker_in = -1;

byte start_row_white_front[8] = { pawn, pawn, pawn, pawn, pawn, pawn, pawn, pawn };
byte start_row_white_back[8] = { ladya, knight, bishop, queen, king, bishop, knight, ladya };

byte start_row_black_front[8] = { pawnB, pawnB, pawnB, pawnB, pawnB, pawnB, pawnB, pawnB };
byte start_row_black_back[8] = { ladyaB, knightB, bishopB, queenB, kingB, bishopB, knightB, ladyaB };

array<byte, board_size> startpos;

void getXY(byte pos, byte& x, byte& y);

int kingpos = -1, kingBpos = -1;
int checkorigin = -1, checkoriginB = -1;

