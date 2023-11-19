#include "grammer_analysis.h"

/*AnalysisTreeNode����ʵ��*/
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

/*GrammerAnalysis����ʵ��*/
GrammerAnalysis::GrammerAnalysis(string wordLabelFileName, string lexResultFileName, string analysisTableFileName, string genericsFileName) {
	//��ʼ��wordLabel
	this->wordLabel = new WordTypeCorrespondenceTable(wordLabelFileName);

	//��ʼ��lexcalResult
	this->lexcalResult = new LexcalResult(lexResultFileName);

	//��ʼ��analysisTable
	this->analysisTable = new AnalysisTable(analysisTableFileName, this->wordLabel);

	//��ʼ��generics
	this->generics = new Generics(genericsFileName, this->wordLabel);
}

GrammerAnalysis::GrammerAnalysis(WordTypeCorrespondenceTable* wordLabel, LexcalResult* lexcalResult, AnalysisTable* analysisTable, Generics* generics) {
	//��ʼ���ʷ����������������ʽ
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
	//��ʼ��������״̬ջ������ջ���ڵ�ջ�����봮����ʼ����
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
		//�ӷ������õ���Ӧ����
		vector<int> location;
		vector<int> operation;
		location.push_back(this->states[this->states.size() - 1]);
		location.push_back(this->inputString[this->inputString.size() - 1]);
		int result = this->analysisTable->findOperation(location, operation);
		//��������з���
		if (result == GRAMMER_ANALYSIS_WRONG) {//����
			for (int i = 0; i < this->analysisTreeNodes.size(); i++) {
				delete this->analysisTreeNodes[i];
			}//�����Ѿ����ɵ����ڵ�
			return result;
		}
		else {
			if (operation[0] == SHIFT) {//�ƽ�
				this->states.push_back(operation[1]);
				this->symbols.push_back(this->inputString[this->inputString.size() - 1]);
				AnalysisTreeNode* analysisTreeNode = new AnalysisTreeNode(this->inputString[this->inputString.size() - 1], 0);
				this->analysisTreeNodes.push_back(analysisTreeNode);
				this->inputString.pop_back();
			}
			else if (operation[0] == REDUCE) {//��Լ
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
					}//�����Ѿ����ɵ����ڵ�
					return result1;
				}
				else {
					this->states.push_back(operation1[1]);
				}
			}
			else if (operation[0] == ACCEPT) {//����
				this->rootNode = this->analysisTreeNodes[this->analysisTreeNodes.size() - 1];
				return GRAMMER_ANALYSIS_SUCCESS;
			}
			//����ǰ���豣��
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
	//����������ȱ����Ľ��
	ofstream treeVisitResultOut;
	treeVisitResultOut.open("./output_files/tree_visit_result.txt");
	if (!treeVisitResultOut) {
		cout << "can't open file!" << endl;
		exit(-1);
	}
	this->rootNode->visitAnalysisTree(treeVisitResultOut, this->wordLabel);
	treeVisitResultOut.close();
	
	//���ɿ��ӻ�����
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