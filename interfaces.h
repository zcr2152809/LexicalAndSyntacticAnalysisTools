#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
using namespace std;

//定义成功
#define LEXCAL_ANALYSIS_SUCCESS 1
#define GRAMMER_ANALYSIS_SUCCESS 2

//定义错误
#define LEXCAL_ANALYSIS_WRONG -1
#define GRAMMER_ANALYSIS_WRONG -2

//定义分析表中的操作
#define SHIFT 0
#define REDUCE 1
#define GOTO 2
#define ACCEPT 3

/*该类用于存储单词类型对应表*/
class WordTypeCorrespondenceTable {
public:
	WordTypeCorrespondenceTable() {};//默认构造
	WordTypeCorrespondenceTable(string wordLabelFileName, bool isAll);//根据文件进行构造，只构造终结符或者全构造
	void insertOneRelation(string word, int type);//插入一个对应关系
	int getWordType(string word);//根据单词获取类型值
	string getWord(int type);//根据类型值获取单词
	int getWordLabelSize();//获取单词种类的数量

private:
	map<string, int> wordLabelForward;//正向映射
	map<int, string> wordLabelReverse;//逆向映射
};


/*可识别词结构体*/
struct Token {
	int type = 0;
	string value;
};

/*该类用于存储词法分析的结果*/
class LexcalResult {
public:
	LexcalResult() {};//默认构造
	LexcalResult(string lexResultFileName);//根据文件名进行构造
	void addOneToken(Token);//向结果当中添加一个识别出的token
	vector<Token> getLexcalResult();//获取词法分析的结果

private:
	vector<Token> tokens;
};

/*该类用于存储LR1分析表*/
class AnalysisTable {
public:
	AnalysisTable() {};//默认构造
	AnalysisTable(string analysisTableFileName, WordTypeCorrespondenceTable* wordLabel);//根据文件和单词类型对应表构建
	void addOneBlock(vector<int> location, vector<int> operation);//向分析表中的一块添加一个内容
	int findOperation(vector<int> location, vector<int> &operation);//找出分析表中对应块的操作
	map<vector<int>, vector<int>> getAnalysisTable();//获取分析表

private:
	map<vector<int>, vector<int>>analysisTable;//存储的分析表
};

/*产生式结构体*/
struct Generic {
	int left = 0;
	vector<int> right;
};

/*该类用于存储产生式*/
class Generics {
public:
	Generics() {};//默认构造
	Generics(string genericsFileName, WordTypeCorrespondenceTable* wordLabel);//根据文件名和单词对应表构造
	void addOneGeneric(Generic generic);//向产生式中添加一项
	Generic findGeneric(int genericId);//根据产生式编号找出对应的产生式
	vector<Generic> getGenerics();//获取产生式
	int getGenericsNum();//获取产生式的数量

private:
	vector<Generic> generics;//存储的产生式集
};
