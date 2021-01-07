#pragma once

#include <sailgame_pb/codenames/codenames.pb.h>
#include <filesystem>

namespace SailGame { namespace Game {

using ::Codenames::Party;

class Word
{
public:
    Word(const std::string &word) : mStr(word) {}
    ::Codenames::Word ConvertToGrpcWord() const;
    bool operator==(const Word &other) const
    {
        return mStr == other.get_str();
    }
    bool operator<(const Word &other) const
    {
        return mStr < other.get_str();
    }
    std::string get_str() const {return mStr;}
private:
    std::string mStr;
};

class Card
{
public:
    Card(const std::string &word, const Party &party) : mWord(word), mParty(party) {}
    Card(const Word &word, const Party &party) : mWord(word), mParty(party) {}
    ::Codenames::Card ConvertToGrpcCard() const;
private:
    Word mWord;
    Party mParty;
};

template<typename T>
class Set
{
public:
    virtual std::vector<T> Draw() = 0;
};

class WordSet : public Set<Word>
{
public:
    WordSet(std::filesystem::path filepath);
    std::vector<Word> Draw();
private:
    std::vector<Word> mWords;
};

class PartySet : public Set<Party>
{
public:
    PartySet(std::filesystem::path filepath);
    std::vector<Party> Draw();
private:
    std::vector<Party> mParties;
};

}}