#include "gen_analysis_tool.h"

/*Item类方法的实现*/
Item::Item(int genericId, int pointLocation, int terminal) {
	this->genericId = genericId;
	this->pointLocation = pointLocation;
	this->terminal = terminal;
}

int Item::getGenericId() {
	return this->genericId;
}

int Item::getPointLocation() {
	return this->pointLocation;
}

int Item::getTerminal() {
	return this->terminal;
}

bool Item::operator<(const Item& other) const {
	if (this->genericId < other.genericId) {
		return true;
	}
	else if (this->genericId == other.genericId) {
		if (this->pointLocation < other.pointLocation) {
			return true;
		}
		else if (this->pointLocation == other.pointLocation) {
			if (this->terminal < other.terminal) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

/*ItemSet类方法实现*/
void ItemSet::addOneItem(Item item) {
	this->itemsets.insert(item);
}

bool ItemSet::isInSet(Item item) {
	return this->itemsets.count(item);
}

bool ItemSet::operator==(const ItemSet& other) const {
	if (this->itemsets.size() != other.itemsets.size()) {
		return false;
	}
	set<Item>::iterator it;
	set<Item>::iterator itOther;
	for (it = this->itemsets.begin(), itOther = other.itemsets.begin(); it != this->itemsets.end(); it++, itOther++) {
		if (*it < *itOther || *itOther < *it) {
			return false;
		}
	}
	return true;
}

/*GenAnalysisTool类方法实现*/
GenAnalysisTool::~GenAnalysisTool() {
	for (int i = 0; i < this->itemSetsFamily.size(); i++) {
		delete this->itemSetsFamily[i];
	}
}

void GenAnalysisTool::readRuleFile(string lexFileName, string grammarFileName) {
	//产生单词类型对应关系
	this->wordLabel = new WordTypeCorrespondenceTable(lexFileName, false);
	this->dividingLine = this->wordLabel->getWordLabelSize();
	ifstream grammerIn;
	string nonterminal;
	grammerIn.open("./input_files/" + grammarFileName, ios::in);
	if (!grammerIn) {
		cout << "can't open file!" << endl;
		exit(-1);
	}
	grammerIn >> nonterminal;
	int nonterminalType = this->dividingLine;
	while (true) {
		grammerIn >> nonterminal;
		if (nonterminal == "@end") {
			break;
		}
		this->wordLabel->insertOneRelation(nonterminal, nonterminalType++);
	}
	ofstream wordLabelOut;
	wordLabelOut.open("./tool_files/word_label.txt", ios::out);
	if (!wordLabelOut) {
		cout << "can't open file!" << endl;
		exit(-1);
	}
	wordLabelOut << this->wordLabel->getWordLabelSize() << endl;
	for (int i = 0; i < this->wordLabel->getWordLabelSize(); i++) {
		wordLabelOut << this->wordLabel->getWord(i) << " " << i << endl;
	}
	wordLabelOut.close();

	//生成产生式集合
	this->generics = new Generics;
	string symbol;
	grammerIn >> symbol;
	while (true) {
		grammerIn >> symbol;
		if (symbol == "@end") {
			break;
		}
		Generic generic;
		generic.left = this->wordLabel->getWordType(symbol);
		grammerIn >> symbol;
		while (true) {
			grammerIn >> symbol;
			if (symbol == "jing033") {
				break;
			}
			generic.right.push_back(this->wordLabel->getWordType(symbol));
		}
		this->generics->addOneGeneric(generic);
	}
	grammerIn.close();
	ofstream genericsOut;
	genericsOut.open("./tool_files/generics.txt", ios::out);
	if (!genericsOut) {
		cout << "can't open file!" << endl;
		exit(-1);
	}
	genericsOut << this->generics->getGenericsNum() << endl;
	for (int i = 0; i < this->generics->getGenericsNum(); i++) {
		Generic generic = this->generics->findGeneric(i);
		genericsOut << this->wordLabel->getWord(generic.left) << " -> ";
		for (int j = 0; j < generic.right.size(); j++) {
			genericsOut << this->wordLabel->getWord(generic.right[j]) << " ";
		}
		genericsOut << "end2326" << endl;
	}
	genericsOut.close();
}

void GenAnalysisTool::caculateSymbolFIRST() {
	//初始化非终结符的FIRST集
	for (int i = dividingLine; i < this->wordLabel->getWordLabelSize(); i++) {
		set<int> FIRST;
		this->FIRSTsets[i] = FIRST;
	}
	for (int i = 0; i < this->generics->getGenericsNum(); i++) {
		Generic generic = this->generics->findGeneric(i);
		if (generic.right.size() == 0) {
			if (!this->FIRSTsets[generic.left].count(-1)) {
				this->FIRSTsets[generic.left].insert(-1);
			}
		}
		else {
			if (generic.right[0] < this->dividingLine) {
				if (!this->FIRSTsets[generic.left].count(generic.right[0])) {
					this->FIRSTsets[generic.left].insert(generic.right[0]);
				}
			}
		}
	}
	//循环计算直到不再变化为止
	while (true) {
		bool isChange = false;
		for (int i = 0; i < this->generics->getGenericsNum(); i++) {
			Generic generic = this->generics->findGeneric(i);
			if (generic.right.size() != 0 && generic.right[0] >= this->dividingLine) {
				int j = 0;
				for (; j < generic.right.size(); j++) {
					bool isBreak = true;
					if (generic.right[j] >= this->dividingLine) {
						for (set<int>::iterator it = this->FIRSTsets[generic.right[j]].begin(); it != this->FIRSTsets[generic.right[j]].end(); it++) {
							if (!this->FIRSTsets[generic.left].count(*it) && *it != -1) {
								this->FIRSTsets[generic.left].insert(*it);
								isChange = true;
							}
							if (*it == -1) {
								isBreak = false;
							}
						}
					}
					else {
						if (!this->FIRSTsets[generic.left].count(generic.right[j])) {
							this->FIRSTsets[generic.left].insert(generic.right[j]);
							isChange = true;
						}
					}
					if (isBreak) {
						break;
					}
				}
				if (j == generic.right.size()) {
					if (!this->FIRSTsets[generic.left].count(-1)) {
						this->FIRSTsets[generic.left].insert(-1);
						isChange = true;
					}
				}
			}
		}
		if (!isChange) {
			break;
		}
	}
}