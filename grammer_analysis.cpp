#include "grammer_analysis.h"

/*AnalysisTreeNode方法实现*/
int AnalysisTreeNode::nodeNum = 0;
AnalysisTreeNode::AnalysisTreeNode(int type, int childrenNum) {
	this->id = AnalysisTreeNode::nodeNum;
	AnalysisTreeNode::nodeNum++;
	this->type = type;
	this->childrenNum = childrenNum;
	this->children = (AnalysisTreeNode**)malloc(childrenNum * sizeof(AnalysisTable*));
	if (!this->children) {
		cout << "Memory allocation failed!" << endl;
		exit(-1);
	}
}

AnalysisTreeNode::~AnalysisTreeNode() {
	for (int i = 0; i < this->childrenNum; i++) {
		delete* (this->children + i);
	}
	free(this->children);
}

void AnalysisTreeNode::setChildren(int i, AnalysisTreeNode* child) {
	*(this->children + i) = child;
}

void AnalysisTreeNode::visitAnalysisTree(ofstream& treeVisitResultOut, WordTypeCorrespondenceTable* wordLabel) {
	treeVisitResultOut << "#" << wordLabel->getWord(this->type) << "# ";
	for (int i = this->childrenNum - 1; i >= 0; i--) {
		(*(this->children + i))->visitAnalysisTree(treeVisitResultOut, wordLabel);
	}
}

void AnalysisTreeNode::genDotFile(ofstream& dotFileOut, WordTypeCorrespondenceTable* wordLabel) {
	string defineSentence, relationSentence, color;
	if (this->childrenNum == 0) {
		color = "red";
	}
	else {
		color = "black";
	}
	defineSentence = "n" + to_string(this->id) + "[label=\"" + wordLabel->getWord(this->type) + "\",color=" + color + "];";
	dotFileOut << defineSentence << endl;
	for (int i = this->childrenNum - 1; i >= 0; i--) {
		relationSentence = "n" + to_string(this->id) + " -> n" + to_string((*(this->children + i))->id) + ";";
		dotFileOut << relationSentence << endl;
		(*(this->children + i))->genDotFile(dotFileOut, wordLabel);
	}
}

/*GrammerAnalysis方法实现*/
GrammerAnalysis::GrammerAnalysis(string wordLabelFileName, string lexResultFileName, string analysisTableFileName, string genericsFileName) {
	//初始化wordLabel
	this->wordLabel = new WordTypeCorrespondenceTable(wordLabelFileName);

	//初始化lexcalResult
	this->lexcalResult = new LexcalResult(lexResultFileName);

	//初始化analysisTable
	this->analysisTable = new AnalysisTable(analysisTableFileName, this->wordLabel);

	//初始化generics
	this->generics = new Generics(genericsFileName, this->wordLabel);
}

GrammerAnalysis::GrammerAnalysis(WordTypeCorrespondenceTable* wordLabel, LexcalResult* lexcalResult, AnalysisTable* analysisTable, Generics* generics) {
	//初始化词法结果、分析表、产生式
	this->wordLabel = wordLabel;
	this->lexcalResult = lexcalResult;
	this->analysisTable = analysisTable;
	this->generics = generics;
}

GrammerAnalysis::~GrammerAnalysis() {
	delete this->wordLabel;
	delete this->lexcalResult;
	delete this->analysisTable;
	delete this->generics;
	delete this->rootNode;
}

void GrammerAnalysis::initAnalysisMachine() {
	//初始化步数，状态栈，符号栈，节点栈，输入串，初始步骤
	this->states.push_back(0);
	this->symbols.push_back(0);
	AnalysisTreeNode* analysisTreeNode = new AnalysisTreeNode(0, 0);
	this->analysisTreeNodes.push_back(analysisTreeNode);
	vector<Token> inputSymbols = this->lexcalResult->getLexcalResult();
	this->inputString.push_back(0);
	for (int i = inputSymbols.size() - 1; i >= 0; i--) {
		this->inputString.push_back(inputSymbols[i].type);
	}
	Process process = { this->states, this->symbols, this->inputString };
	this->analysisProcess.push_back(process);
}

int GrammerAnalysis::performAnalysis() {
	while (true) {
		//从分析表拿到对应操作
		vector<int> location;
		vector<int> operation;
		location.push_back(this->states[this->states.size() - 1]);
		location.push_back(this->inputString[this->inputString.size() - 1]);
		int result = this->analysisTable->findOperation(location, operation);
		//分情况进行分析
		if (result == GRAMMER_ANALYSIS_WRONG) {//错误
			for (int i = 0; i < this->analysisTreeNodes.size(); i++) {
				delete this->analysisTreeNodes[i];
			}//清理已经生成的树节点
			return result;
		}
		else {
			if (operation[0] == SHIFT) {//移进
				this->states.push_back(operation[1]);
				this->symbols.push_back(this->inputString[this->inputString.size() - 1]);
				AnalysisTreeNode* analysisTreeNode = new AnalysisTreeNode(this->inputString[this->inputString.size() - 1], 0);
				this->analysisTreeNodes.push_back(analysisTreeNode);
				this->inputString.pop_back();
			}
			else if (operation[0] == REDUCE) {//规约
				Generic generic = this->generics->findGeneric(operation[1]);
				AnalysisTreeNode* analysisTreeNode = new AnalysisTreeNode(generic.left, generic.right.size());
				for (int i = 0; i < generic.right.size(); i++) {
					this->states.pop_back();
					this->symbols.pop_back();
					analysisTreeNode->setChildren(i, this->analysisTreeNodes[this->analysisTreeNodes.size() - 1]);
					this->analysisTreeNodes.pop_back();
				}
				this->symbols.push_back(generic.left);
				this->analysisTreeNodes.push_back(analysisTreeNode);
				vector<int> location1;
				vector<int> operation1;
				location1.push_back(this->states[this->states.size() - 1]);
				location1.push_back(this->symbols[this->symbols.size() - 1]);
				int result1 = this->analysisTable->findOperation(location1, operation1);
				if (result1 == GRAMMER_ANALYSIS_WRONG) {
					for (int i = 0; i < this->analysisTreeNodes.size(); i++) {
						delete this->analysisTreeNodes[i];
					}//清理已经生成的树节点
					return result1;
				}
				else {
					this->states.push_back(operation1[1]);
				}
			}
			else if (operation[0] == ACCEPT) {//接受
				this->rootNode = this->analysisTreeNodes[this->analysisTreeNodes.size() - 1];
				return GRAMMER_ANALYSIS_SUCCESS;
			}
			//将当前步骤保存
			Process process = { this->states, this->symbols, this->inputString };
			this->analysisProcess.push_back(process);
		}
	}
}

void GrammerAnalysis::outputProcess() {
	ofstream processOut;
	processOut.open("./output_files/process.txt", ios::out);
	if (!processOut) {
		cout << "can't open file!" << endl;
		exit(-1);
	}
	for (int i = 0; i < this->analysisProcess.size(); i++) {
		processOut << i << "      ";
		for (int j = 0; j < this->analysisProcess[i].states.size(); j++) {
			processOut << this->analysisProcess[i].states[j] << " ";
		}
		processOut << "     ";
		for (int j = 0; j < this->analysisProcess[i].symbols.size(); j++) {
			processOut << this->wordLabel->getWord(this->analysisProcess[i].symbols[j]) << " ";
		}
		processOut << "     ";
		for (int j = this->analysisProcess[i].inputString.size() - 1; j >= 0; j--) {
			processOut << this->wordLabel->getWord(this->analysisProcess[i].inputString[j]) << " ";
		}
		processOut << endl;
	}
}

void GrammerAnalysis::genTree() {
	//生成深度优先遍历的结果
	ofstream treeVisitResultOut;
	treeVisitResultOut.open("./output_files/tree_visit_result.txt");
	if (!treeVisitResultOut) {
		cout << "can't open file!" << endl;
		exit(-1);
	}
	this->rootNode->visitAnalysisTree(treeVisitResultOut, this->wordLabel);
	treeVisitResultOut.close();
	
	//生成可视化的树
	ofstream dotFileOut;
	dotFileOut.open("./output_files/analysis_tree.dot");
	if (!dotFileOut) {
		cout << "can't open file!" << endl;
		exit(-1);
	}
	dotFileOut << "digraph mygraph {" << endl;
	this->rootNode->genDotFile(dotFileOut, this->wordLabel);
	dotFileOut << "}";
	dotFileOut.close();
	system("dot -Tpng ./output_files/analysis_tree.dot -o ./output_files/analysis_tree.png");
}