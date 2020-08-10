// chess_self_coding_challenge.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Windows.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <locale>  
#include <stack>
#include <list>
#include <vector>
#include <array>
#include <set>
#include "main.h"


void draw()
{
	cout << xoffset_l << xoffset_l_big << " ";

	for (int i = 0; i < 8; i++)
		cout << (ui_cols[i]) << " ";

	byte markerout_x=-1, markerout_y=-1, markerin_x=-1, markerin_y=-1;

	if (marker_in < 0 || marker_out < 0 || marker_in >= board_size || marker_out >= board_size)
		;
	else {
		getXY(marker_in, markerin_x, markerin_y);
		getXY(marker_out, markerout_x, markerout_y);
	}


	cout << xoffset_r << ui_lines[0] << "\n";
	//byte& mx = markerout_x;
	
	{
		#define mx(i,k) (markerin_x==i?marker_in_h[k]:(markerout_x==i?marker_out_h[k]:ui_bar_h))
		char bar[17] = { mx(0,0), mx(0,1), mx(1,0), mx(1,1), mx(2,0), mx(2,1), mx(3,0), mx(3,1), mx(4,0), mx(4,1), mx(5,0), mx(5,1), mx(6,0), mx(6,1), mx(7,0), mx(7,1), '\0' };
		#undef mx
		cout << xoffset_l_big << /*" " <<*/ " " << ui_bar_ang << ui_bar_h /*<< xoffset_l*/ << bar << xoffset_r << ui_lines[1] << "\n";
	}

	for (int i = 0; i < 8; i++)
	{
		{
			#define mx(i) (markerin_y==i?marker_in_v:(markerout_y==i?marker_out_v:ui_bar_v))
			cout << xoffset_l_big << (ui_rows[7 - i]) << /*xoffset_l*/ mx(i) << " ";
			#undef mx
		}
		for (int j = 0; j < 8; j++)
		{
			bool empty = (board(j, i) == null);
			bool white = (board(j, i) <= king && !empty);
			char _postfix = white ? postfixW : postfixB;
			cout << ((empty ? ((j + i) % 2 ? ui_space : ui_square_graydark) : ui_cells[board(j, i)])) << ((j + i) % 2 ? (empty ? ' ' : _postfix) : (empty ? ui_square_graydark : _postfix));
		}
		cout << xoffset_r << ui_lines[i + 2] << "\n";
	}
	cout << "\n";
}

bool IsEmpty(byte pos)
{
	return (board[pos] == null);
}

int GetTeam(byte pos)
{
	if (!board[pos])return e_team::TEAM_NULL;
	bool black = (board[pos] > e_piece::king);
	//1 - white, 2 - black
	return black+1;
}

bool IsEnemy(byte move_from, byte move_to)
{
	return GetTeam(move_from) != GetTeam(move_to);
}

//#define board(i,j)   board.at((i)*8 + (j))

void getXY(byte pos, byte &x, byte &y)
{
	//pos = x * 8 + y;
	x = pos % 8;
	y = (pos - x) / 8;
}

int Trace(byte move_from, byte move_to)
{

	byte x1, y1, x2, y2;

	getXY(move_from, x1, y1);
	getXY(move_to, x2, y2);

	
	if (x1 == x2) //vertical trace
	{
		;
	}
	else
		if (y1 == y2) //horizontal trace
		{
			;
		}

	//0 - err, 1 - success, 2 - enemy eaten
	return 0;
}

int ProcRules(byte move_from, byte move_to)
{
	if (!board[move_from])return ERR_RULE;

	byte x1, y1, x2, y2;
	getXY(move_from, x1, y1);
 	getXY(move_to, x2, y2);

	
	int piece_from = board[move_from];
	if(piece_from > e_piece::pawnB)piece_from-=(e_piece::knightB - e_piece::knight);

	//0 - err, 1 - success, 2 - enemy eaten
	
	//move_from
	switch (piece_from)
	{
		case e_piece::pawn:
		{
			int yd = y1 - y2;
			bool firstrow = (y1 == 6);

			if (abs(x1 - x2) == 1 && yd == 1)
			{
				if (board(x2, y2) && IsEnemy(move_from, move_to))
					if(y2==0)
						return EAT_ADV;
					else
						return EAT;
			}
			if (yd > (1 + firstrow) || yd <= 0 || (x1 != x2))
				return ERR_RULE;

			if (yd == 1)
			{
				if (board(x2, y2) != e_piece::null)
					return ERR_RULE;
			}
			else
				if (yd == 2)
				{
					if (board(x2, y2 + 1) != e_piece::null)
						return ERR_RULE;
					if (board(x2, y2) != e_piece::null)
						return ERR_RULE;
				}
			return MOV;
		}
		break;

		case e_piece::pawnB:
		{
			int yd = y2 - y1;
			bool firstrow = (y1 == 1);

			if (abs(x1 - x2) == 1 && yd == 1)
			{
				if (board(x2, y2) && IsEnemy(move_from, move_to))
					if (y2 == 0)
						return EAT_ADV;
					else
						return EAT;
			}
			if (yd > (1 + firstrow) || yd <= 0 || (x1 != x2))
				return ERR_RULE;

			if (yd == 1)
			{
				if (board(x2, y2) != e_piece::null)
					return ERR_RULE;
			}
			else
				if (yd == 2)
				{
					if (board(x2, y2 + 1) != e_piece::null)
						return ERR_RULE;
					if (board(x2, y2) != e_piece::null)
						return ERR_RULE;
				}
			return MOV;
		}
		break;

		case e_piece::knight:
		{
			int yd = y1 - y2;
			int xd = x1 - x2;

			if ((abs(xd) == 1 && abs(yd) == 2) || (abs(xd) == 2 && abs(yd) == 1))
			{
				if (board(x2, y2) && IsEnemy(move_from, move_to))
					return EAT;
				else
					if (!board(x2, y2))
						return MOV;
			}
		}
		break;

		case e_piece::king:
		{
			int yd = y1 - y2;
			int xd = x1 - x2;

			if (abs(xd) <= 1 && abs(yd) <= 1)
			{
				if (board(x2, y2) && IsEnemy(move_from, move_to))
					return EAT;
				else
					if (!board(x2, y2))
						return MOV;
			}
		}
		break;

		case e_piece::ladya:
		{
			int yd = y1 - y2;
			int xd = x1 - x2;
			
			if (abs(xd) > 0 && yd == 0)
			{
				int sign = (x1 < x2) * 2 - 1;

				for (int x = x1; x != (x2); )
				{
					x += sign;
					bool last = ((x2-x)==0);
					if(last)
					{
						if (board(x, y1))
							if (IsEnemy(move_from, move_to))
								return EAT;
							else
								return ERR_RULE;
						else return MOV;
					}
					if (board(x, y1))
						return ERR_RULE;
				}
				return ERR_RULE;
			}
			if (abs(yd) > 0 && xd == 0)
			{
				int sign = (y1 < y2) * 2 - 1;

				for (int y = y1; y != (y2);)
				{
					y += sign;
					bool last = ((y2 - y) == 0);
					if (last)
					{
						if (board(x1, y))
							if (IsEnemy(move_from, move_to))
								return EAT;
							else
								return ERR_RULE;
						else return MOV;
					}
					if (board(x1, y))
						return ERR_RULE;
				}
				return ERR_RULE;
			}
		}
		break;

		case e_piece::bishop:
		{
			int yd = y1 - y2;
			int xd = x1 - x2;

			if (abs(xd)==abs(yd))
			{
				int signx = (x1 < x2) * 2 - 1;
				int signy = (y1 < y2) * 2 - 1;
				
				int y = y1;

				for (int x = x1; x != (x2); )
				{
					x += signx;
					y += signy;
					bool last = ((x2 - x) == 0);
					if (last)
					{
						if (board(x, y))
							if (IsEnemy(move_from, move_to))
								return EAT;
							else
								return ERR_RULE;
						else return MOV;
					}
					if (board(x, y))
						return ERR_RULE;
				}
				return ERR_RULE;
			}
		}
		break;

		case e_piece::queen:
		{
			int yd = y1 - y2;
			int xd = x1 - x2;
			
			if (abs(xd) > 0 && yd == 0)
			{
				int sign = (x1 < x2) * 2 - 1;

				for (int x = x1; x != (x2); )
				{
					x += sign;
					bool last = ((x2 - x) == 0);
					if (last)
					{
						if (board(x, y1))
							if (IsEnemy(move_from, move_to))
								return EAT;
							else
								return ERR_RULE;
						else return MOV;
					}
					if (board(x, y1))
						return ERR_RULE;
				}
				return ERR_RULE;
			}
			if (abs(yd) > 0 && xd == 0)
			{
				int sign = (y1 < y2) * 2 - 1;

				for (int y = y1; y != (y2);)
				{
					y += sign;
					bool last = ((y2 - y) == 0);
					if (last)
					{
						if (board(x1, y))
							if (IsEnemy(move_from, move_to))
								return EAT;
							else
								return ERR_RULE;
						else return MOV;
					}
					if (board(x1, y))
						return ERR_RULE;
				}
				return ERR_RULE;
			}
			
			if (abs(xd) == abs(yd))
			{
				int signx = (x1 < x2) * 2 - 1;
				int signy = (y1 < y2) * 2 - 1;

				int y = y1;

				for (int x = x1; x != (x2); )
				{
					x += signx;
					y += signy;
					bool last = ((x2 - x) == 0);
					if (last)
					{
						if (board(x, y))
							if (IsEnemy(move_from, move_to))
								return EAT;
							else
								return ERR_RULE;
						else return MOV;
					}
					if (board(x, y))
						return ERR_RULE;
				}
				return ERR_RULE;
			}
		}
		break;
	}

	return ERR_RULE;

}

int ProcMove(string &move)
{


	marker_out = -1; marker_in = -1;
	if (move == "restart" || move == "reload" || move == "reset" || move == "re" || move == "new")
	{
		board = startpos;
		teamactive = e_team::TEAM_WHITE;
		return e_move_result::UPD_NORULES;
	}
	//skip
	if (move == "chars")// || move == "ui" || move == "ascii")
	{
		for (byte ch = 0; ch < 255; ch++)
		{
			cout << (ch);
		}

		return e_move_result::PASS;
	}
	if (move == "char")// || move.substr(0, 2) == "ui" || (move.substr(0,5) == "ascii"))
	{
		//if (move.length() > 5)
		{

			//int arg = strtol(move.substr(5).c_str(), 0, 10);
			//cout << "32";// arg;
			
			int arg;
			cin >> arg;
			cout << (char)arg;
		}

		return e_move_result::PASS;
	}

	if (move == "rnd")// || move.substr(0, 2) == "ui" || (move.substr(0,5) == "ascii"))
	{
		//todo normalized ('from', den 'to', distinctly)
		//todo auto rnd mvs
		skip
		{
			int arg;
			cin >> arg;
			cout << (char)arg;
		}

		//"a1a1";// mv[4] = '\0';// = "a1a1";
		int result;
		std::string s_mv;
			do {
				const char mv[5] = {
					'a' + rand() % 8,
					'1' + rand() % 8,
					'a' + rand() % 8,
					'1' + rand() % 8,
					'\0'
				};
				s_mv = mv;
			result = ProcMove(s_mv);
		} while (result == e_move_result::ERR_MOVE);

		move=s_mv.append("(rnd)");
		return e_move_result::UPD;
	}

	bool skiprules = false;
	bool clone = false;
	if (move.substr(0, 1) == "_")
	{
		skiprules = true;
		if (move.length() == 5)
			move = move.substr(1, 4);
		else
			return 0;
	}
	if (move.substr(0, 1) == "+")
	{
		skiprules = true;
		clone = true;
		if (move.length() == 5)
			move = move.substr(1, 4);
		else
			return 0;
	}

	if (move.length() != 4)return 0;

	byte move_from, move_to;
	move_from = (7 - (move[1] - '0')) * 8 + ((move[0] - 'a')) + 8;
	move_to = (7 - (move[3] - '0')) * 8 + ((move[2] - 'a')) + 8;

	if (move_from < 0 || move_to < 0 || move_from >= board_size || move_to >= board_size)
		return 0;

	if (skiprules)
	{
		board[move_to] = board[move_from];
		if(!clone)
			board[move_from] = null;
		marker_out = move_from; marker_in = move_to;
		return e_move_result::UPD_NORULES;
	}
	else {

		if (IsEmpty(move_from))return e_move_result::ERR_MOVE;

		else if (teamactive == e_team::TEAM_WHITE && GetTeam(move_from) == e_team::TEAM_BLACK)return e_move_result::ERR_MOVE;
		else if (teamactive == e_team::TEAM_BLACK && GetTeam(move_from) == e_team::TEAM_WHITE)return e_move_result::ERR_MOVE;


		switch (ProcRules(move_from, move_to))
		{
		case e_rule_result::ERR_RULE:
			return e_move_result::ERR_MOVE;
			break;
		case e_rule_result::MOV:
			board[move_to] = board[move_from];
			board[move_from] = null;

			//ui_lines[1] = string();
			marker_out = move_from; marker_in = move_to;

			return e_move_result::UPD;
			break;
		case e_rule_result::EAT:
			board[move_to] = board[move_from];
			board[move_from] = null;

			ui_lines[1] = "eaten";
			marker_out = move_from; marker_in = move_to;
			return e_move_result::UPD;
			break;
		case e_rule_result::EAT_ADV:
			board[move_to] = e_piece::queen; //TODO: choice
			board[move_from] = null;

			ui_lines[1] = "eaten";
			marker_out = move_from; marker_in = move_to;
			return e_move_result::UPD;
			break;
		}
	}
};

void CalcCheck()
{
	checkorigin = -1;
	checkoriginB = -1;
	for (int i = 0; i < board_size; i++)
	{
		ProcRules(i, kingpos) == e_rule_result::EAT ? checkorigin = i : 0;
		ProcRules(i, kingBpos) == e_rule_result::EAT ? checkoriginB = i : 0;
	}
}

int main()
{

	//teamactive = e_team::TEAM_WHITE;

	for (int i = 0; i < 8; i++)
	{
		startpos.at(8*0 + i) = start_row_black_back[i];
		startpos.at(8*1 + i) = start_row_black_front[i];

		startpos.at(8 * 6 + i) = start_row_white_front[i];
		startpos.at(8 * 7 + i) = start_row_white_back[i];
	}
	board = startpos;

	ui_lines[0] = "chess";// "CHESS offline auto challenge";
	for(int i=1;i<9;i++)
		ui_lines[i] = "";

	//array board[8][8];

	//board(0,0)

	//string("A");


	//skip
	/*
	║186
	╔201
	═205
	*/

	skip
		cout << byte(186) << byte(201) << byte(205);
	skip
		for (byte ch = 0; ch < 240; ch++)
		{
			//	char ch = 'z' + 1;
			cout << (ch);
		}

	ui_lines[4] = teamactive == 1 ? "white to move" : (teamactive == 2 ? "black to move" : "");


	while (true)
	{

		draw();
		for(int i=0;i<10;i++)
			ui_lines[i].clear();

		bool move_successful = false;


		string move;

		while (!move_successful) {

			cout << "move: ";
			cin >> move;

			switch (ProcMove(move))
			{
			case e_move_result::ERR_MOVE:
				cout << "err" << "\n";
				break;
			case e_move_result::PASS:
				cout << "\n";
				break; 
			case e_move_result::UPD:
				cout << "\n";
				move_successful = true;
				if(teamactive == e_team::TEAM_WHITE)teamactive= e_team::TEAM_BLACK;
				else if(teamactive == e_team::TEAM_BLACK)teamactive = e_team::TEAM_WHITE;

				break;
			case e_move_result::UPD_NORULES:
				cout << "\n";
				move_successful = true;
				break;
			}
		}


		for (int i = 0; i < board_size; i++)
		{
			if (board[i] == e_piece::king)kingpos = i;
			if (board[i] == e_piece::kingB)kingBpos = i;
		}

		CalcCheck();

		ui_lines[0] = "";
		ui_lines[2] = "whites: " + move;
		ui_lines[3] = ""; // todo expanded move desc
		ui_lines[4] = teamactive == 1 ? "white to move" : (teamactive == 2 ? "black to move" : "");
		ui_lines[5] = checkorigin>=0 ? "check the white king" : (checkoriginB>=0 ? "check the black king" : "");
	}
}