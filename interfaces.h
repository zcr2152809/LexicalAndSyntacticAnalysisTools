#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
using namespace std;

//����ɹ�
#define LEXCAL_ANALYSIS_SUCCESS 1
#define GRAMMER_ANALYSIS_SUCCESS 2

//�������
#define LEXCAL_ANALYSIS_WRONG -1
#define GRAMMER_ANALYSIS_WRONG -2

//����������еĲ���
#define SHIFT 0
#define REDUCE 1
#define GOTO 2
#define ACCEPT 3

/*�������ڴ洢�������Ͷ�Ӧ��*/
class WordTypeCorrespondenceTable {
public:
	WordTypeCorrespondenceTable() {};//Ĭ�Ϲ���
	WordTypeCorrespondenceTable(string wordLabelFileName, bool isAll);//�����ļ����й��죬ֻ�����ս������ȫ����
	void insertOneRelation(string word, int type);//����һ����Ӧ��ϵ
	int getWordType(string word);//���ݵ��ʻ�ȡ����ֵ
	string getWord(int type);//��������ֵ��ȡ����
	int getWordLabelSize();//��ȡ�������������

private:
	map<string, int> wordLabelForward;//����ӳ��
	map<int, string> wordLabelReverse;//����ӳ��
};


/*��ʶ��ʽṹ��*/
struct Token {
	int type = 0;
	string value;
};

/*�������ڴ洢�ʷ������Ľ��*/
class LexcalResult {
public:
	LexcalResult() {};//Ĭ�Ϲ���
	LexcalResult(string lexResultFileName);//�����ļ������й���
	void addOneToken(Token);//�����������һ��ʶ�����token
	vector<Token> getLexcalResult();//��ȡ�ʷ������Ľ��

private:
	vector<Token> tokens;
};

/*�������ڴ洢LR1������*/
class AnalysisTable {
public:
	AnalysisTable() {};//Ĭ�Ϲ���
	AnalysisTable(string analysisTableFileName, WordTypeCorrespondenceTable* wordLabel);//�����ļ��͵������Ͷ�Ӧ����
	void addOneBlock(vector<int> location, vector<int> operation);//��������е�һ�����һ������
	int findOperation(vector<int> location, vector<int> &operation);//�ҳ��������ж�Ӧ��Ĳ���
	map<vector<int>, vector<int>> getAnalysisTable();//��ȡ������

private:
	map<vector<int>, vector<int>>analysisTable;//�洢�ķ�����
};

/*����ʽ�ṹ��*/
struct Generic {
	int left = 0;
	vector<int> right;
};

/*�������ڴ洢����ʽ*/
class Generics {
public:
	Generics() {};//Ĭ�Ϲ���
	Generics(string genericsFileName, WordTypeCorrespondenceTable* wordLabel);//�����ļ����͵��ʶ�Ӧ����
	void addOneGeneric(Generic generic);//�����ʽ�����һ��
	Generic findGeneric(int genericId);//���ݲ���ʽ����ҳ���Ӧ�Ĳ���ʽ
	vector<Generic> getGenerics();//��ȡ����ʽ
	int getGenericsNum();//��ȡ����ʽ������

private:
	vector<Generic> generics;//�洢�Ĳ���ʽ��
};
