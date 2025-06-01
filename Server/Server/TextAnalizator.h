#ifndef TEXTANALIZATOR_H
#define TEXTANALIZATOR_H

#include <string>
#include <vector>
struct AnalysisResult 
{
    size_t total_words;
    size_t unique_words;
    size_t longest_sequence_length;
    std::string longest_sequence;
};


AnalysisResult analyze_text(const std::string& text);


#endif // !TEXTANALIZATOR_H