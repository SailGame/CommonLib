#include <vector>
#include <fstream>
#include <sailgame_pb/codenames/codenames.pb.h>
#include "sailgame/codenames/word.h"

namespace SailGame {namespace Game {

using ::Codenames::Party;
static const size_t MATRIX_SIZE = 25;

::Codenames::Word Word::ConvertToGrpcWord() const
{
    ::Codenames::Word word;
    word.set_word(mStr);
    return word;
}

::Codenames::Card Card::ConvertToGrpcCard() const
{
    ::Codenames::Card card;
    ::Codenames::Word word = mWord.ConvertToGrpcWord();
    card.set_allocated_word(&word);
    card.set_party(mParty);
    return card;
}

WordSet::WordSet(std::filesystem::path filepath)
{
    if(!fs::exists(filepath) || !fs::is_regular_file(filepath)) {
        throw std::runtime_error("File not exists");
    }
    std::string word;
    std::ifstream in(filepath);
    if(in.is_open())
    {
        while(getline(in, word))
        {
            mWords.emplace_back(word);
        }
        in.close();
    }
    else {
        throw std::runtime_error("Unable to open file");
    }
    std::srand(std::time(nullptr));
}

std::vector<Word> WordSet::Draw()
{
    auto begin = std::rand() % mWords.size();
    auto end = (begin + MATRIX_SIZE) % mWords.size();
    if(begin < end)
    {
        return {mWords.begin()+begin, mWords.begin()+end};
    }
    else {
        std::vector<Word> first{mWords.begin()+begin, mWords.end()};
        decltype(first) second = {mWords.begin(), mWords.begin()+end};
        first.insert(first.end(), second.begin(), second.end());
        return first;
    }
}

PartySet::PartySet(std::filesystem::path filepath)
{
    if(!fs::exists(filepath) || !fs::is_regular_file(filepath)) {
        throw std::runtime_error("File not exists");
    }
    std::string party = "null";
    std::ifstream in(filepath);
    if(in.is_open())
    {
        while(getline(in, party))
        {
            if(party=="BYSTANDER") {
                mParties.emplace_back(Party::BYSTANDER);
            } else if (party=="BLUE") {
                mParties.emplace_back(Party::BLUE);
            } else if (party=="RED") {
                mParties.emplace_back(Party::RED);
            } else if(party=="ASSASSIN") {
                mParties.emplace_back(Party::ASSASSIN);
            } else {
                throw std::runtime_error("Unsupported Party type");
            }
        }
        in.close();
    }
    else {
        throw std::runtime_error("Unable to open file");
    }
    std::srand(std::time(nullptr));
}

std::vector<Party> PartySet::Draw()
{
    auto begin = std::rand() % (mParties.size() / MATRIX_SIZE);
    return {mParties.begin()+(begin*MATRIX_SIZE), mParties.begin()+(begin*MATRIX_SIZE+MATRIX_SIZE)};
}
}}