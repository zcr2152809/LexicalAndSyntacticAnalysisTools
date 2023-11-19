#pragma once
#include "interfaces.h"

/*�ýṹ���ʾ��Լ������ĳһ����״̬*/
struct Process {
	vector<int> states;
	vector<int> symbols;
	vector<int> inputString;
};

/*�������ڴ洢���������*/
class AnalysisTreeNode {
public:
	AnalysisTreeNode(int type, int childrenNum);//��ʼ��
	~AnalysisTreeNode();//��������
	void setChildren(int i, AnalysisTreeNode* child);//���ӽڵ��ָ��ŵ��ӽ�������е�ָ��λ��
	void visitAnalysisTree(ofstream& treeVisitResultOut, WordTypeCorrespondenceTable* wordLabel);//������ȵı�����÷�����
	void genDotFile(ofstream& dotFileOut, WordTypeCorrespondenceTable* wordLabel);//�������ڲ���png��dot�ļ�
	static int nodeNum;

private:
	int id;//�ڵ�ı��
	int type;//�ڵ�ķ�������
	int childrenNum;//�ڵ���ӽڵ���Ŀ
	AnalysisTreeNode** children = NULL;//�洢�ӽڵ�ָ�������
};

/*�������ڸ���LR1�������ʷ�������������﷨�����������﷨������*/
class GrammerAnalysis {
public:
	GrammerAnalysis(string wordLabelFileName, string lexResultFileName, string analysisTableFileName, string genericsFileName);//�����ļ���ʼ��
	GrammerAnalysis(WordTypeCorrespondenceTable* wordLabel, LexcalResult* lexcalResult, AnalysisTable* analysisTable, Generics* generics);//���ݴ����ָ���ʼ��
	~GrammerAnalysis();
	void initAnalysisMachine();//��ʼ����������
	int performAnalysis();//���з���
	void outputProcess();//��������Ĺ���
	void genTree();
private:
	WordTypeCorrespondenceTable* wordLabel;//�������Ͷ�Ӧ��
	LexcalResult* lexcalResult;//�ʷ��������
	AnalysisTable* analysisTable;//LR1������
	Generics* generics;//����ʽ
	vector<int> states;//ջ��״̬
	vector<int> symbols;//ջ�ڷ���
	vector<AnalysisTreeNode*> analysisTreeNodes;//��ջ�ڷ���ͬ�����������������﷨�������Ľ��
	vector<int> inputString;//���봮
	vector<Process> analysisProcess;//��������
	AnalysisTreeNode* rootNode = NULL;//�﷨�������ĸ��ڵ�
};