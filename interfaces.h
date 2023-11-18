#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

//�������
#define LEXCAL_ANALYSIS_WRONG -1
#define GRAMMER_ANALYSIS_WRONG -2

//����������еĲ���
#define SHIFT 0
#define REDUCE 1
#define GOTO 2
#define ACCEPT 3


/*��ʶ��ʽṹ��*/
struct Token {
	int type;
	string value;
};

/*�������ڴ洢�ʷ������Ľ��*/
class LexcalResult {
public:
	void addOneToken(Token);//�����������һ��ʶ�����token
	vector<Token> getLexcalResult();//��ȡ�ʷ������Ľ��

private:
	vector<Token> tokens;
};

/*�������ڴ洢LR1������*/
class AnalysisTable {
public:
	void addOneBlock(vector<int> location, vector<int> operation);//��������е�һ�����һ������
	int findOperation(vector<int> location, vector<int> &operation);//�ҳ��������ж�Ӧ��Ĳ���
	map<vector<int>, vector<int>> getAnalysisTable();//��ȡ������

private:
	map<vector<int>, vector<int>>analysisTable;//�洢�ķ�����
};

/*����ʽ�ṹ��*/
struct Generic {
	int left;
	vector<int> right;
};

/*�������ڴ洢����ʽ*/
class Generics {
public:
	void addOneGeneric(Generic generic);//�����ʽ�����һ��
	Generic findGeneric(int genericId);//���ݲ���ʽ����ҳ���Ӧ�Ĳ���ʽ
	vector<Generic> getGenerics();//��ȡ����ʽ

private:
	vector<Generic> generics;//�洢�Ĳ���ʽ��
};
