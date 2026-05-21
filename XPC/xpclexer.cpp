#include <string>
#include <iostream>
#include "xpclexer.h"
#include <iostream>
#include <regex>
using namespace std;

std::ostream& operator<<(std::ostream& os, const Token& t) {
	os << "Token(";
	switch (t.type) {
		case Identifier: os << "Identifier"; break;
		case Literal: os << "Literal"; break;
		case Operator: os << "Operator"; break;
		case Delimiter: os << "Delimiter"; break;
		case Type: os << "Type"; break;
		case Modifier: os << "Modifier"; break;
		case otherKeyword: os << "otherKeyword"; break;
		case Semi: os << "Semi"; break;
		case EndOfFile: os << "EndOfFile"; break;
	}
	os << ", \"" << t.val << "\")\n";
	return os;
}
List<Token> Lexer::Lex() {
	bool ERR = false;
	std::string code = this->code;
	List<Token> tokens;
	int pos = 0;
	int col = 0;
	int line = 0;
	auto newToken = [&](TokenType type, std::string val) {
		Token token;
		token.type = type;
		token.val = val;
		token.line = line;
		token.col = col;
		tokens.push_back(token);
	};
	List<std::string> types = {
		"dynam",
		"int",
		"float",
		"long",
		"double",
		"char",
		"str",
		"struct"
	};
	List<std::string> modifiers = {
		"static",
		"const",
		"func",
		"inline",
		"ptr"
	};
	List<std::string> otherKeywords = {
		"if",
		"while",
		"break",
		"continue",
		"return"
	};
	string punc = "()[]{},";
	List<std::string> opps = {"+","-","*","/","%","^","&","&&","|","||","~","~~", "!", "=","+=","-=","*=", "/=", "^=", "==", ">=", "<=", "!=", ">", "<", "->", ".", "rt"}; //tilda: bitwise xor: root. ex, a~b
	std::sort(opps.begin(), opps.end(), [](const std::string& a, const std::string& b) {
		return a.length() > b.length();
	});
	while (pos < code.length()) {
		char c = code[pos];
		if(std::isspace(c)) {
			if(c == '\n') {
				col = 0;
				line ++;
			}
			pos++;
			continue;
		}
		{
			bool found = false;
			for (auto& opp : opps) {
				if(code.substr(pos, opp.length()) == opp) {
					if (opPrecidence[opp].first == -INF) {
						newToken(Literal, "");
					} else if(opPrecidence[opp].second == -INF) {
						newToken(Literal, "");
					}
					newToken(Operator, opp);
					newToken(Operator, std::string(opp));
					pos += opp.length();
					col += opp.length();
					found = true;
					break;
				}
			}
			if(found == true) {
				continue;
			}
		}
		if(std::isalpha(c)) {
			int start = pos;
			while(pos < code.length() && std::isalnum(code[pos])) {
				pos++;
				col++;
			}
			std::string subStr = code.substr(start,pos-start);
			if (types.contains(subStr)) {
				newToken(Type, subStr);
			} else if (modifiers.contains(subStr)) {
				newToken(Modifier, subStr);
			} else if (otherKeywords.contains(subStr)) {
				newToken(otherKeyword, subStr);
			} else {
				newToken(Identifier, subStr);
			}
			continue;
		}
		if(std::isdigit(c)) {
			int start = pos;
			while(pos < code.length() && std::isdigit(code[pos])) {
				pos++;
				col++;
			}
			std::string subStr = code.substr(start, pos-start);
			newToken(Literal, subStr);
			continue;
		}
		if(punc.find(c) != string::npos) {
			newToken(Delimiter,string(1,c));
			pos++;
			col++;
			continue;
		}
		if(c == ';'){
			pos++;
			col++;
			newToken(Semi, string(1,c));
			continue;
		}
		if(c == '\''){
			newToken(Literal, code.substr(pos, 3));
			pos += 3;
			col += 3;
			continue;
		}
		std::cout << "Unidentified Token \"" << c << "\"! line " << line << ", col " << col << "\n";
		ERR = true;
		pos++;
		col++;
	}
	if (ERR) {
		return {};
	}
	
	newToken(EndOfFile, "");
	return tokens;
}

