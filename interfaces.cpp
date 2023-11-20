#include "interfaces.h"

/*WordTypeCorrespondenceTable方法实现*/
WordTypeCorrespondenceTable::WordTypeCorrespondenceTable(string wordLabelFileName, bool isAll) {
	ifstream wordLabelIn;
	if (isAll) {
		wordLabelIn.open("./tool_files/" + wordLabelFileName, ios::in);
	}
	else {
		wordLabelIn.open("./input_files/" + wordLabelFileName, ios::in);
	}
	
	if (!wordLabelIn) {
		cout << "can't open file!" << endl;
		exit(-1);
	}
	int num;
	wordLabelIn >> num;
	for (int i = 0; i < num; i++) {
		string value;
		int type;
		wordLabelIn >> value;
		wordLabelIn >> type;
		this->wordLabelForward[value] = type;
		this->wordLabelReverse[type] = value;
	}
	wordLabelIn.close();
}

void WordTypeCorrespondenceTable::insertOneRelation(string word, int type) {
	this->wordLabelForward[word] = type;
	this->wordLabelReverse[type] = word;
}

int WordTypeCorrespondenceTable::getWordType(string word) {
	return this->wordLabelForward[word];
}

string WordTypeCorrespondenceTable::getWord(int type) {
	return this->wordLabelReverse[type];
}

int WordTypeCorrespondenceTable::getWordLabelSize() {
	return this->wordLabelForward.size();
}

/*LexcalResult方法实现*/
LexcalResult::LexcalResult(string lexResultFileName) {
	ifstream lexResultIn;
	lexResultIn.open("./tool_files/" + lexResultFileName, ios::in);
	if (!lexResultIn) {
		cout << "can't open file!" << endl;
		exit(-1);
	}
	int num;
	lexResultIn >> num;
	for (int i = 0; i < num; i++) {
		Token token;
		lexResultIn >> token.type;
		lexResultIn >> token.value;
		this->addOneToken(token);
	}
	lexResultIn.close();
}

void LexcalResult::addOneToken(Token token) {
	this->tokens.push_back(token);
}

vector<Token> LexcalResult::getLexcalResult() {
	return this->tokens;
}

/*AnalysisTable方法实现*/
AnalysisTable::AnalysisTable(string analysisTableFileName, WordTypeCorrespondenceTable* wordLabel) {
	ifstream analysisTableIn;
	vector<int> heading;
	int row, col;
	analysisTableIn.open("./tool_files/" + analysisTableFileName, ios::in);
	if (!analysisTableIn) {
		cout << "can't open file!" << endl;
		exit(-1);
	}
	analysisTableIn >> row;
	analysisTableIn >> col;
	for (int i = 0; i < col; i++) {
		string symbol;
		analysisTableIn >> symbol;
		heading.push_back(wordLabel->getWordType(symbol));
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			string blockContent;
			analysisTableIn >> blockContent;
			if (blockContent != "no") {
				vector<int> operation;
				if (blockContent[0] == 's') {
					operation.push_back(SHIFT);
					operation.push_back(blockContent[1] - 48);
				}
				else if (blockContent[0] == 'r') {
					operation.push_back(REDUCE);
					operation.push_back(blockContent[1] - 48);
				}
				else if (blockContent == "acc") {
					operation.push_back(ACCEPT);
					operation.push_back(-1);
				}
				else if (blockContent.size() == 1) {
					operation.push_back(GOTO);
					operation.push_back(blockContent[0] - 48);
				}
				vector<int> location;
				location.push_back(i);
				location.push_back(heading[j]);
				this->addOneBlock(location, operation);
			}
		}
	}
	analysisTableIn.close();
}


void AnalysisTable::addOneBlock(vector<int> location, vector<int> operation) {
	this->analysisTable[location] = operation;
}

int AnalysisTable::findOperation(vector<int> location, vector<int>& operation) {
	if (this->analysisTable.count(location)) {
		operation = this->analysisTable[location];
		return 0;
	}
	else {
		return GRAMMER_ANALYSIS_WRONG;
	}
}

map<vector<int>, vector<int>> AnalysisTable::getAnalysisTable() {
	return this->analysisTable;
}

/*Generics方法实现*/
Generics::Generics(string genericsFileName, WordTypeCorrespondenceTable* wordLabel) {
	ifstream genericsIn;
	genericsIn.open("./tool_files/" + genericsFileName, ios::in);
	if (!genericsIn) {
		cout << "can't open file!" << endl;
		exit(-1);
	}
	int gener_num;
	genericsIn >> gener_num;
	for (int i = 0; i < gener_num; i++) {
		string tmp;
		Generic generic;
		genericsIn >> tmp;
		generic.left = wordLabel->getWordType(tmp);
		genericsIn >> tmp;
		while (true) {
			genericsIn >> tmp;
			if (tmp == "end2326") {
				break;
			}
			else {
				generic.right.push_back(wordLabel->getWordType(tmp));
			}
		}
		this->addOneGeneric(generic);
	}
	genericsIn.close();
}

void Generics::addOneGeneric(Generic generic) {
	this->generics.push_back(generic);
}

Generic Generics::findGeneric(int genericId) {
	return this->generics[genericId];
}

vector<Generic> Generics::getGenerics() {
	return this->generics;
}

int Generics::getGenericsNum() {
	return this->generics.size();
}
