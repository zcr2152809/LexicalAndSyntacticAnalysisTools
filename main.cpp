#include "grammer_analysis.h"

int main() {
    GrammerAnalysis* grammerAnalysis = new GrammerAnalysis("word_label.txt", "lex_result.txt", "analysis_table.txt", "generics.txt");
    grammerAnalysis->initAnalysisMachine();
    grammerAnalysis->performAnalysis();
    grammerAnalysis->outputProcess();
    grammerAnalysis->genTree();

    return 0;
}
