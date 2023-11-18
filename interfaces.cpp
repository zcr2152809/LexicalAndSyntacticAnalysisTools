#include "interfaces.h"

/*LexcalResult方法实现*/
void LexcalResult::addOneToken(Token token) {
	this->tokens.push_back(token);
}

vector<Token> LexcalResult::getLexcalResult() {
	return this->tokens;
}

/*AnalysisTable方法实现*/
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
void Generics::addOneGeneric(Generic generic) {
	this->generics.push_back(generic);
}

Generic Generics::findGeneric(int genericId) {
	return this->generics[genericId];
}

vector<Generic> Generics::getGenerics() {
	return this->generics;
}

