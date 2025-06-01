#include "gtest/gtest.h"
#include "../BoostStart/TextAnalizator.h"
#include <fstream>

//curl -X POST --data-binary @test.txt http://localhost:8080/analyze


TEST(AnalyzeTextTest, Basic) 
{
    std::string text = "'I do,' Alice hastily replied; 'at least--at least I mean what I say--that's the same thing, you know.'";
    AnalysisResult res = analyze_text(text);
    EXPECT_EQ(res.total_words, 20);
    EXPECT_EQ(res.unique_words, 16);
    EXPECT_EQ(res.longest_sequence_length, 10);

    std::string expected = "mean what I say that's the same thing you know";
    EXPECT_EQ(res.longest_sequence, expected);
}

TEST(AnalyzeTextTest, RepeatedWords) 
{
    std::string text = "one two two three three three four";
    AnalysisResult res = analyze_text(text);
    EXPECT_EQ(res.total_words, 7);
    EXPECT_EQ(res.unique_words, 4);
    EXPECT_EQ(res.longest_sequence_length, 2);

    std::string expected = "one two";
    EXPECT_EQ(res.longest_sequence, expected);
}

TEST(AnalyzeTextTest, Apostrophes) 
{
    std::string text = "I'm happy. Isn't it great? Yes, I'm sure.";
    AnalysisResult res = analyze_text(text);
    EXPECT_EQ(res.total_words, 8);
    EXPECT_EQ(res.unique_words, 7);
    EXPECT_EQ(res.longest_sequence_length, 7);

    std::string expected = "happy Isn't it great Yes I'm sure";
    EXPECT_EQ(res.longest_sequence, expected);
}

TEST(AnalyzeTextTest, CyrillicSimple)
{
    std::string text = u8"Привіт, як справи? Привіт!";
    AnalysisResult res = analyze_text(text);
    EXPECT_EQ(res.total_words, 4);
    EXPECT_EQ(res.unique_words, 3);
    EXPECT_EQ(res.longest_sequence_length, 3);

    std::string expected = u8"Привіт як справи";
    EXPECT_EQ(res.longest_sequence, expected);
}


// Словом вважається будь-яка послідовність літер розділена символом не-літерою. 
// Але слова типу Alice's, isn't, I'm,… вважати одним словом. Великі і маленькі літери не розрізняти.


TEST(AnalyzeTextTest, HardTest1)
{
    std::string text = "1122Tor123Tor123";
    AnalysisResult res = analyze_text(text);
    EXPECT_EQ(res.total_words, 2);
    EXPECT_EQ(res.unique_words, 1);
    EXPECT_EQ(res.longest_sequence_length, 1);

    std::string expected = "Tor";
    EXPECT_EQ(res.longest_sequence, expected);
}

TEST(AnalyzeTextTest, HardTest2)
{
    std::string text = "1122Tor123TOR123";
    AnalysisResult res = analyze_text(text);
    EXPECT_EQ(res.total_words, 2);
    EXPECT_EQ(res.unique_words, 1);
    EXPECT_EQ(res.longest_sequence_length, 1);

    std::string expected = "Tor"; // при порівнні унікальності на регістр не дивимось
    EXPECT_EQ(res.longest_sequence, expected);
}

TEST(AnalyzeTextTest, HardTest3)
{
    std::string text = "````tttt````t`ttt`";
    AnalysisResult res = analyze_text(text);
    EXPECT_EQ(res.total_words, 3);
    EXPECT_EQ(res.unique_words, 3);
    EXPECT_EQ(res.longest_sequence_length, 3);

    std::string expected = "tttt t ttt";
    EXPECT_EQ(res.longest_sequence, expected);
}


TEST(AnalyzeTextTest, FromFile)
{

    std::ifstream inFile;
    std::string text;
    inFile.open("../../TextAnalizatorTesting/TestData/alice.txt");


    if (inFile.is_open())
    {
        std::ostringstream ss;
        ss << inFile.rdbuf();  // зчитуємо весь файл у потоковий рядок
        text = ss.str();
    }

    //text = "````tttt````t`ttt`";
    AnalysisResult res = analyze_text(text);
    EXPECT_EQ(res.total_words, 26800);
    EXPECT_EQ(res.unique_words, 2656);
    EXPECT_EQ(res.longest_sequence_length, 43);

    std::string expected = "same words as before It's all his fancy that he hasn't got no sorrow you know Come on So they went up to the Mock Turtle who looked at them with large eyes full of tears but said nothing This here young lady";
    EXPECT_EQ(res.longest_sequence, expected);
}
