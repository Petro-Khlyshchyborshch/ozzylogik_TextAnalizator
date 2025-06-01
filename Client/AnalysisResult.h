#ifndef ANALYSISRESULT_H
#define ANALYSISRESULT_H

#include <string>

struct AnalysisResult
{
    size_t total_words;
    size_t unique_words;
    size_t longest_sequence_length;
    std::string longest_sequence;
};

#endif // ANALYSISRESULT_H
