#pragma once
#include "interfaces.h"
#include <set>

/*该类用于存储LR1项目，以三元组的形式*/ 
class Item {
public:
	Item(int genericId, int pointLocation, int terminal);//构造函数
	int getGenericId();//获取产生式Id
	int getPointLocation();//获取圆点位置
	int getTerminal();//获取项目展望的终结符
	bool operator<(const Item& other) const;//重载小于运算符

private:
	int genericId;//产生式的Id
	int pointLocation;//圆点的位置
	int terminal;//项目展望的终结符
};

/*该类用于存储项目集*/
class ItemSet {
public:
	void addOneItem(Item item);//向项目集中添加一个项目
	bool isInSet(Item item);//判断项目是否在项目集中
	bool operator==(const ItemSet& other) const;//重载==运算符

private:
	set<Item> itemsets;
};

/*该类用于产生语法分析所需的工具*/
class GenAnalysisTool {
public:
	~GenAnalysisTool();//析构函数
	void readRuleFile(string lexFileName, string grammarFileName);//读取词法和语法规则文件，处理后存储
	void caculateSymbolFIRST();//计算每个非终结符的FIRST集
	vector<int> caculateSymbolListFIRST(vector<int> symbolList);//计算输入符号串的FIRST集
	void CLOSURE(ItemSet* I);//计算项目集I的闭包，置于项目集I中
	ItemSet* GO(ItemSet* I, int X);//GO函数，返回一个新的项目集
	void genAnalysisTool();//产生LR1需要的分析工具，项目集族和分析表
	WordTypeCorrespondenceTable* getWordLabel();//获取单词对应表
	Generics* getGenerics();//获取产生式集合
	AnalysisTable* getAnalysisTable();//获取LR1分析表

private:
	WordTypeCorrespondenceTable* wordLabel;//单词对应表
	Generics* generics;//产生式集
	map<int, set<int>>FIRSTsets;//非终结符号的FIRST集
	vector<ItemSet*> itemSetsFamily;//项目集族
	AnalysisTable* analysisTable;//LR1分析表
	int dividingLine;//划分终结符和非终结符的分隔线
};