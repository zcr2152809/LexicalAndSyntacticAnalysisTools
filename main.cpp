#include "gen_analysis_tool.h"
#include "grammer_analysis.h"

int main() {
    /*GrammerAnalysis* grammerAnalysis = new GrammerAnalysis("word_label.txt", "lex_result.txt", "analysis_table.txt", "generics.txt");
    grammerAnalysis->initAnalysisMachine();
    grammerAnalysis->performAnalysis();
    grammerAnalysis->outputProcess();
    grammerAnalysis->genTree();*/

    /*ItemSet* itemSet1 = new ItemSet;
    ItemSet* itemSet2 = new ItemSet;
    Item item1{ 1, 1, 1 }, item2{ 1, 1, 2 }, item3{ 2, 1, 1 };
    itemSet1->addOneItem(item2);
    itemSet1->addOneItem(item1);
    itemSet2->addOneItem(item1);
    itemSet2->addOneItem(item2);
    cout << (*itemSet1 == *itemSet2);*/

    GenAnalysisTool* genAnalysisTool = new GenAnalysisTool;
    genAnalysisTool->readRuleFile("terminal_label1.txt", "grammer1.txt");
    genAnalysisTool->caculateSymbolFIRST();

    return 0;
}
