#include "TextAnalizator.h"
#include <cstdlib>
#include <iostream>

#include <thread>
#include <unordered_set>
#include <unordered_map>
#include <locale>
#include <codecvt>
#include <cwctype>


AnalysisResult analyze_text(const std::string& text) 
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv32;
    std::u32string u32text = conv32.from_bytes(text);

    std::vector<std::string> words;         
    std::vector<std::string> lowered_words;  
    std::vector<char32_t> token_chars;
    token_chars.reserve(64);

    auto flush_token = [&]() 
    {
        if (!token_chars.empty()) 
        {
            std::string original_word = conv32.to_bytes(token_chars.data(), token_chars.data() + token_chars.size());

            // Перетворення в нижній регістр для перевірки унікальності
            std::u32string token_u32(token_chars.begin(), token_chars.end());
            std::u32string lowered_u32;
            lowered_u32.reserve(token_u32.size());

            for (char32_t ch : token_u32) 
            {
                lowered_u32.push_back(std::towlower(static_cast<wint_t>(ch)));
            }
            std::string lowered_word = conv32.to_bytes(lowered_u32.data(), lowered_u32.data() + lowered_u32.size());

            words.push_back(std::move(original_word));
            lowered_words.push_back(std::move(lowered_word));

            token_chars.clear();
        }
    };

    size_t n = u32text.size();
    for (size_t i = 0; i < n; ++i) 
    {
        char32_t cp = u32text[i];
        bool is_letter = std::iswalpha(static_cast<wint_t>(cp));
        bool is_apostrophe = (cp == U'\'' || cp == U'’') &&
            i > 0 && i + 1 < n &&
            std::iswalpha(static_cast<wint_t>(u32text[i - 1])) &&
            std::iswalpha(static_cast<wint_t>(u32text[i + 1]));

        if (is_letter || is_apostrophe) 
        {
            token_chars.push_back(cp);
        }
        else 
        {
            flush_token();
        }
    }
    flush_token();

    AnalysisResult result;
    result.total_words = words.size();

    // Підрахунок унікальних слів без урахування регістру
    std::unordered_set<std::string> uniq(lowered_words.begin(), lowered_words.end());
    result.unique_words = uniq.size();

    // Знаходження найдовшої послідовності унікальних слів
    std::unordered_map<std::string, size_t> last_pos;
    
    size_t start = 0, max_len = 0, max_start = 0;
    for (size_t i = 0; i < lowered_words.size(); ++i) 
    {
        auto it = last_pos.find(lowered_words[i]);
        if (it != last_pos.end() && it->second >= start) 
        {
            start = it->second + 1;
        }
        last_pos[lowered_words[i]] = i;
        size_t curr_len = i - start + 1;
        if (curr_len > max_len) 
        {
            max_len = curr_len;
            max_start = start;
        }
    }
    result.longest_sequence_length = max_len;

    if (max_len > 0) 
    {
        std::string seq;
        for (size_t i = max_start; i < max_start + max_len; ++i) 
        {
            seq += words[i];
            if (i + 1 < max_start + max_len) 
            {
                seq += " "; // слова додаєм через пробіл
            }
        }
        result.longest_sequence = std::move(seq);
    }
    else 
    {
        result.longest_sequence.clear();
    }

    return result;
}
