#include <string>
#include <iostream>
#include "xpclexer.h"
#include <iostream>
#include <regex>
using namespace std;

List<Token> Lexer::Lex() {
	bool ERR = false;
	std::string code = this->code;
	List<Token> tokens;
	auto newToken = [&](TokenType type, std::string val) {
		Token token;
		token.type = type;
		token.val = val;
		tokens.push_back(token);
	};
	int pos = 0;
	int col = 0;
	int line = 0;
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

