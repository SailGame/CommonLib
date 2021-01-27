#include <sailgame/uno/card.h>

namespace SailGame { namespace Uno {

const std::set<CardColor> CardSet::NonWildColors = 
    { CardColor::RED, CardColor::YELLOW, CardColor::GREEN, CardColor::BLUE };

const std::set<CardText> CardSet::NonWildTexts = 
    { CardText::ZERO, CardText::ONE, CardText::TWO, CardText::THREE, CardText::FOUR, 
      CardText::FIVE, CardText::SIX, CardText::SEVEN, CardText::EIGHT, CardText::NINE, 
      CardText::SKIP, CardText::REVERSE, CardText::DRAW_TWO };

const std::set<CardText> CardSet::DrawTexts = { CardText::DRAW_TWO, CardText::DRAW_FOUR };

void Handcards::Draw(Card card)
{
    mCards.emplace(card);
}

void Handcards::Draw(const std::vector<Card> &cards)
{
    std::for_each(cards.begin(), cards.end(), [this](Card card) {
        Draw(card);
    });
}

void Handcards::FillInitHandcards(const InitHandcardsT &initHandcards)
{
    for (const auto & card : initHandcards) {
        Draw(card);
    }
}

void Handcards::Erase(int index)
{
    mCards.erase(std::next(mCards.begin(), index));
}

void Handcards::Erase(Card card)
{
    Erase(GetIndex(card));
}

int Handcards::GetIndex(Card card) const
{
    auto it = mCards.find(card);
    assert(it != mCards.end());
    return std::distance(mCards.begin(), it);
}

int Handcards::GetIndexOfNewlyDrawn(const Handcards &handcardsBeforeDraw) const
{
    assert(Number() == handcardsBeforeDraw.Number() + 1);
    for (int i = 0; i < handcardsBeforeDraw.Number(); i++) {
        if (At(i) != handcardsBeforeDraw.At(i)) {
            return i;
        }
    }
    return handcardsBeforeDraw.Number();
}

void Deck::Init()
{
    this->Clear();
    mDiscardPile.Clear();
    for (auto color : CardSet::NonWildColors) {
        for (auto text : CardSet::NonWildTexts) {
            PushFront(color, text);
            if (text != CardText::ZERO) {
                // in UNO, there is only one zero for each color
                // and two cards for other text (except wild and wild draw four)
                PushFront(color, text);
            }
        }
    }

    auto wildCardNum = 4;
    for (int i = 0; i < wildCardNum; i++) {
        PushFront(CardColor::BLACK, CardText::WILD);
        PushFront(CardColor::BLACK, CardText::DRAW_FOUR);
    }

    Shuffle();
}

std::vector<InitHandcardsT> Deck::DealInitHandcards(int playerNum)
{
    std::vector<InitHandcardsT> initHandCards(playerNum);
    for (int card = 0; card < 7; card++) {
        for (int player = 0; player < playerNum; player++) {
            initHandCards[player][card] = Draw();
        }
    }
    return initHandCards;
}

Card Deck::Draw()
{
    if (Empty()) {
        Swap(mDiscardPile);
        Shuffle();
    }
    return PopFront();
}

std::vector<Card> Deck::Draw(int number)
{
    std::vector<Card> cards(number);
    std::generate(cards.begin(), cards.end(), [this]() { return Draw(); });
    return cards;
}

}}