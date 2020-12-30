#pragma once

#include <deque>
#include <set>
#include <algorithm>
#include <random>

#include <uno/uno.pb.h>

namespace SailGame { namespace Game {

using ::Uno::CardColor;
using ::Uno::CardText;

struct Card;

using InitHandcardsT = std::array<Card, 7>;

struct Card {
    CardColor mColor;
    CardText mText;

    Card() = default;
    Card(CardColor color, CardText text) : mColor(color), mText(text) {}
    Card(const ::Uno::Card &card) : mColor(card.color()), mText(card.text()) {}

    Card(const Card &) = default;
    Card &operator=(const Card &) = default;

    ::Uno::Card ConvertToGrpcCard() const {
        ::Uno::Card card;
        card.set_color(mColor);
        card.set_text(mText);
        return card;
    }

    bool operator<(const Card &rhs) const {
        return (mColor < rhs.mColor) || 
            (mColor == rhs.mColor && mText < rhs.mText);
    }
    
    bool operator==(const Card &rhs) const {
        return (mColor == rhs.mColor) && (mText == rhs.mText);
    }

    bool operator!=(const Card &card) const {
        return !(*this == card);
    }
};

struct CardSet {
    const static std::set<CardColor> NonWildColors;
    const static std::set<CardText> NonWildTexts;
    const static std::set<CardText> DrawTexts;
};

class Handcards {
public:
    Handcards() = default;

    void Draw(Card card);

    void Draw(const std::vector<Card> &cards);

    void FillInitHandcards(const InitHandcardsT &initHandcards);

    void Erase(int index);

    void Erase(Card card);

    int GetIndex(Card card) const;

    int GetIndexOfNewlyDrawn(const Handcards &handcardsBeforeDraw) const;

    bool Empty() const { return mCards.empty(); }

    Card At(int index) const { return *std::next(mCards.begin(), index); }

    int Number() const { return mCards.size(); }

private:
    std::multiset<Card> mCards;
};

/**
 * \c CardPile: a plie of cards, can be derived as \c Deck and \c DiscardPile
 * providing some methods about push/pop, which can be used in different scenarios:
 *   PushFront: Init deck in the game start. The card goes into discard pile.
 *   PopFront:  Draw from deck.
 *   PushBack:  When the flipped card is a wild card, put it back to under the deck.
 *   PopBack:   not used yet
 */
class CardPile {
protected:
    template <typename... Types>
    void PushFront(Types... args) {
        mPile.emplace_front(args...);
    }

    Card PopFront() {
        Card card = mPile.front();
        mPile.pop_front();
        return card;
    }

    template <typename... Types>
    void PushBack(Types... args) {
        mPile.emplace_back(args...);
    }

    Card PopBack() {
        Card card = mPile.back();
        mPile.pop_back();
        return card;
    }

    void Shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(mPile.begin(), mPile.end(), g);
    }

    void Swap(CardPile &pile) { std::swap(mPile, pile.mPile); }

    void Clear() { mPile.clear(); }

    bool Empty() const { return mPile.empty(); }

protected:
    std::deque<Card> mPile;
};

class DiscardPile : public CardPile {
public:
    void Add(Card card) { PushFront(card); }

    void Clear() { CardPile::Clear(); }

    int Number() const { return mPile.size(); }
};

class Deck : public CardPile {
public:
    Deck(DiscardPile &discardPile) : mDiscardPile(discardPile) {}

    void Init();

    std::vector<InitHandcardsT> DealInitHandcards(int playerNum);

    Card Draw();

    std::vector<Card> Draw(int number);

    void PutToBottom(Card card) { PushBack(card); }

    int Number() const { return mPile.size(); }

private:
    // link a discard pile to deck. when the deck is exhausted, swap them
    DiscardPile &mDiscardPile;
};
}}