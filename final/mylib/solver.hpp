#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <iostream>
#include <string>
#include <algorithm>

// String Rubik Functions

namespace solver {

using namespace std;

string format(const string& s) {
	string formatted;
	for (int i=0; i<s.length(); ++i) {
		if (s[i] == '\'') {
			formatted += s[i-1];
			formatted += s[i-1];
		}
		else if (s[i] == '2') {
		    formatted += s[i-1];
		}
		else if (s[i] == ' ') {}
		else {
		    formatted += s[i];
		}
	}
	return formatted;
}

void applyM(std::string& s, int start=0) {
	for (int i=start; i<s.size(); ++i) {
		switch (s[i]) {
		case 'U':
			s[i]='F';
			break;
		case 'F':
			s[i]='D';
			break;
		case 'D':
			s[i]='B';
			break;
		case 'B':
			s[i]='U';
			break;
		case 'S':
			s[i]='E';
			break;
		case 'E':
			s[i]='S';
			break;
		default:
			break;
		}
	}
}

void applyMI(std::string& s, int start=0) {
	for (int i=start; i<s.size(); ++i) {
		switch (s[i]) {
		case 'U':
			s[i]='B';
			break;
		case 'B':
			s[i]='D';
			break;
		case 'D':
			s[i]='F';
			break;
		case 'F':
			s[i]='U';
			break;
		case 'S':
			s[i]='E';
			break;
		case 'E':
			s[i]='S';
			break;
		default:
			break;
		}
	}
}

void applyS(std::string& s, int start=0) {
	for (int i=start; i<s.size(); ++i) {
		switch (s[i]) {
		case 'U':
			s[i]='R';
			break;
		case 'R':
			s[i]='D';
			break;
		case 'D':
			s[i]='L';
			break;
		case 'L':
			s[i]='U';
			break;
		case 'M':
			s[i]='E';
			break;
		case 'E':
			s[i]='M';
			break;
		default:
			break;
		}
	}
}

void applySI(std::string& s, int start=0) {
	for (int i=start; i<s.size(); ++i) {
		switch (s[i]) {
		case 'U':
			s[i]='L';
			break;
		case 'L':
			s[i]='D';
			break;
		case 'D':
			s[i]='R';
			break;
		case 'R':
			s[i]='U';
			break;
		case 'M':
			s[i]='E';
			break;
		case 'E':
			s[i]='M';
			break;
		default:
			break;
		}
	}
}

void applyE(std::string& s, int start=0) {
	for (int i=start; i<s.size(); ++i) {
		switch (s[i]) {
		case 'L':
			s[i]='F';
			break;
		case 'F':
			s[i]='R';
			break;
		case 'R':
			s[i]='B';
			break;
		case 'B':
			s[i]='L';
			break;
		case 'M':
			s[i]='S';
			break;
		case 'S':
			s[i]='M';
			break;
		default:
			break;
		}
	}
}

void applyEI(std::string& s, int start=0) {
	for (int i=start; i<s.size(); ++i) {
		switch (s[i]) {
		case 'L':
			s[i]='B';
			break;
		case 'B':
			s[i]='R';
			break;
		case 'R':
			s[i]='F';
			break;
		case 'F':
			s[i]='L';
			break;
		case 'M':
			s[i]='S';
			break;
		case 'S':
			s[i]='M';
			break;
		default:
			break;
		}
	}
}

// actualiza los movimientos inplace
// y retorna los movimiento del centro (M, S o E)
std::string format_center(std::string& s) {
	cout<<s<<"\n";
	string res, centers;
	for (int i=0; i<s.size(); ++i) {
		if (s[i]=='M') {
 			res.push_back('R');
			res += "LLL";
			applyMI(s, i+1);
			centers.push_back('M');
		} else if (s[i]=='E') {
			res.push_back('D');
			res += "UUU";
			applyE(s, i+1);
			centers.push_back('E');
		} else if (s[i]=='S') {
			res.push_back('B');
			res += "FFF";
			applySI(s, i+1);
			centers.push_back('S');
		} else {
			res.push_back(s[i]);
		}
	}
	s=res;
	cout<<s<<"\n";
	// reverse(centers.begin(), centers.end());
	return centers;
}

std::vector<std::string> split(const std::string &txt) {
	std::vector<std::string> strs;
	char ch = ' ';
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }
    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );
	if (strs.back().empty())
		strs.pop_back();
    return strs;
}

} // namespace solver

#endif