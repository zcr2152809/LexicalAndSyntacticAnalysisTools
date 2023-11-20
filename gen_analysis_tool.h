#pragma once
#include "interfaces.h"
#include <set>

/*�������ڴ洢LR1��Ŀ������Ԫ�����ʽ*/ 
class Item {
public:
	Item(int genericId, int pointLocation, int terminal);//���캯��
	int getGenericId();//��ȡ����ʽId
	int getPointLocation();//��ȡԲ��λ��
	int getTerminal();//��ȡ��Ŀչ�����ս��
	bool operator<(const Item& other) const;//����С�������

private:
	int genericId;//����ʽ��Id
	int pointLocation;//Բ���λ��
	int terminal;//��Ŀչ�����ս��
};

/*�������ڴ洢��Ŀ��*/
class ItemSet {
public:
	void addOneItem(Item item);//����Ŀ�������һ����Ŀ
	bool isInSet(Item item);//�ж���Ŀ�Ƿ�����Ŀ����
	bool operator==(const ItemSet& other) const;//����==�����

private:
	set<Item> itemsets;
};

/*�������ڲ����﷨��������Ĺ���*/
class GenAnalysisTool {
public:
	~GenAnalysisTool();//��������
	void readRuleFile(string lexFileName, string grammarFileName);//��ȡ�ʷ����﷨�����ļ��������洢
	void caculateSymbolFIRST();//����ÿ�����ս����FIRST��
	vector<int> caculateSymbolListFIRST(vector<int> symbolList);//����������Ŵ���FIRST��
	void CLOSURE(ItemSet* I);//������Ŀ��I�ıհ���������Ŀ��I��
	ItemSet* GO(ItemSet* I, int X);//GO����������һ���µ���Ŀ��
	void genAnalysisTool();//����LR1��Ҫ�ķ������ߣ���Ŀ����ͷ�����
	WordTypeCorrespondenceTable* getWordLabel();//��ȡ���ʶ�Ӧ��
	Generics* getGenerics();//��ȡ����ʽ����
	AnalysisTable* getAnalysisTable();//��ȡLR1������

private:
	WordTypeCorrespondenceTable* wordLabel;//���ʶ�Ӧ��
	Generics* generics;//����ʽ��
	map<int, set<int>>FIRSTsets;//���ս���ŵ�FIRST��
	vector<ItemSet*> itemSetsFamily;//��Ŀ����
	AnalysisTable* analysisTable;//LR1������
	int dividingLine;//�����ս���ͷ��ս���ķָ���
};