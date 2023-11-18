#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

//定义错误
#define LEXCAL_ANALYSIS_WRONG -1
#define GRAMMER_ANALYSIS_WRONG -2

//定义分析表中的操作
#define SHIFT 0
#define REDUCE 1
#define GOTO 2
#define ACCEPT 3


/*可识别词结构体*/
struct Token {
	int type;
	string value;
};

/*该类用于存储词法分析的结果*/
class LexcalResult {
public:
	void addOneToken(Token);//向结果当中添加一个识别出的token
	vector<Token> getLexcalResult();//获取词法分析的结果

private:
	vector<Token> tokens;
};

/*该类用于存储LR1分析表*/
class AnalysisTable {
public:
	void addOneBlock(vector<int> location, vector<int> operation);//向分析表中的一块添加一个内容
	int findOperation(vector<int> location, vector<int> &operation);//找出分析表中对应块的操作
	map<vector<int>, vector<int>> getAnalysisTable();//获取分析表

private:
	map<vector<int>, vector<int>>analysisTable;//存储的分析表
};

/*产生式结构体*/
struct Generic {
	int left;
	vector<int> right;
};

/*该类用于存储产生式*/
class Generics {
public:
	void addOneGeneric(Generic generic);//向产生式中添加一项
	Generic findGeneric(int genericId);//根据产生式编号找出对应的产生式
	vector<Generic> getGenerics();//获取产生式

private:
	vector<Generic> generics;//存储的产生式集
};
