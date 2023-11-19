#pragma once
#include "interfaces.h"

/*该结构体表示规约过程中某一步的状态*/
struct Process {
	vector<int> states;
	vector<int> symbols;
	vector<int> inputString;
};

/*该类用于存储分析树结点*/
class AnalysisTreeNode {
public:
	AnalysisTreeNode(int type, int childrenNum);//初始化
	~AnalysisTreeNode();//析构函数
	void setChildren(int i, AnalysisTreeNode* child);//将子节点的指针放到子结点数组中的指定位置
	void visitAnalysisTree(ofstream& treeVisitResultOut, WordTypeCorrespondenceTable* wordLabel);//深度优先的遍历这棵分析树
	void genDotFile(ofstream& dotFileOut, WordTypeCorrespondenceTable* wordLabel);//生成用于产生png的dot文件
	static int nodeNum;

private:
	int id;//节点的编号
	int type;//节点的符号类型
	int childrenNum;//节点的子节点数目
	AnalysisTreeNode** children = NULL;//存储子节点指针的数组
};

/*该类用于根据LR1分析表，词法分析结果进行语法分析，产生语法分析树*/
class GrammerAnalysis {
public:
	GrammerAnalysis(string wordLabelFileName, string lexResultFileName, string analysisTableFileName, string genericsFileName);//根据文件初始化
	GrammerAnalysis(WordTypeCorrespondenceTable* wordLabel, LexcalResult* lexcalResult, AnalysisTable* analysisTable, Generics* generics);//根据传入的指针初始化
	~GrammerAnalysis();
	void initAnalysisMachine();//初始化分析机器
	int performAnalysis();//进行分析
	void outputProcess();//输出分析的过程
	void genTree();
private:
	WordTypeCorrespondenceTable* wordLabel;//单词类型对应表
	LexcalResult* lexcalResult;//词法分析结果
	AnalysisTable* analysisTable;//LR1分析表
	Generics* generics;//产生式
	vector<int> states;//栈内状态
	vector<int> symbols;//栈内符号
	vector<AnalysisTreeNode*> analysisTreeNodes;//于栈内符号同步进出的用于生成语法分析树的结点
	vector<int> inputString;//输入串
	vector<Process> analysisProcess;//分析过程
	AnalysisTreeNode* rootNode = NULL;//语法分析树的根节点
};