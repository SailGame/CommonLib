#include <sailgame/uno/card.h>

namespace SailGame { namespace Uno {

const std::set<CardColor> CardSet::NonWildColors = 
    { CardColor::RED, CardColor::YELLOW, CardColor::GREEN, CardColor::BLUE };

const std::set<CardText> CardSet::NonWildTexts = 
    { CardText::ZERO, CardText::ONE, CardText::TWO, CardText::THREE, CardText::FOUR, 
      CardText::FIVE, CardText::SIX, CardText::SEVEN, CardText::EIGHT, CardText::NINE, 
      CardText::SKIP, CardText::REVERSE, CardText::DRAW_TWO };

const std::set<CardText> CardSet::DrawTexts = { CardText::DRAW_TWO, CardText::DRAW_FOUR };

Card::Card(const char *str)
{
    switch (*str) {
        case 'R': mColor = CardColor::RED;    str++; break;
        case 'Y': mColor = CardColor::YELLOW; str++; break;
        case 'G': mColor = CardColor::GREEN;  str++; break;
        case 'B': mColor = CardColor::BLUE;   str++; break;
        default:  mColor = CardColor::BLACK;
    }

    switch (*str) {
        case '0': mText = CardText::ZERO;    break;
        case '1': mText = CardText::ONE;     break;
        case '2': mText = CardText::TWO;     break;
        case '3': mText = CardText::THREE;   break;
        case '4': mText = CardText::FOUR;    break;
        case '5': mText = CardText::FIVE;    break;
        case '6': mText = CardText::SIX;     break;
        case '7': mText = CardText::SEVEN;   break;
        case '8': mText = CardText::EIGHT;   break;
        case '9': mText = CardText::NINE;    break;
        case 'S': mText = CardText::SKIP;    break;
        case 'R': mText = CardText::REVERSE; break;
        case 'W': mText = CardText::WILD;    break;
        case '+': mText = (*(str + 1) == '2') ? 
            CardText::DRAW_TWO : CardText::DRAW_FOUR; 
            break;
        case '\0': mText = CardText::EMPTY;  break;
        default: throw std::runtime_error("Unsupported text");
    }
}

std::string Card::ToString() const
{
    std::string color;
    std::string text;

    switch (mColor) {
        case CardColor::RED:    color = "R"; break;
        case CardColor::YELLOW: color = "Y"; break;
        case CardColor::GREEN:  color = "G"; break;
        case CardColor::BLUE:   color = "B"; break;
        case CardColor::BLACK:  color = "";  break;
        default: throw std::runtime_error("Unsupported color");
    }

    switch (mText) {
        case CardText::ZERO:        text = "0";  break;
        case CardText::ONE:         text = "1";  break;
        case CardText::TWO:         text = "2";  break;
        case CardText::THREE:       text = "3";  break;
        case CardText::FOUR:        text = "4";  break;
        case CardText::FIVE:        text = "5";  break;
        case CardText::SIX:         text = "6";  break;
        case CardText::SEVEN:       text = "7";  break;
        case CardText::EIGHT:       text = "8";  break;
        case CardText::NINE:        text = "9";  break;
        case CardText::SKIP:        text = "S";  break;
        case CardText::REVERSE:     text = "R";  break;
        case CardText::DRAW_TWO:    text = "+2"; break;
        case CardText::WILD:        text = "W";  break;
        case CardText::DRAW_FOUR:   text = "+4"; break;
        case CardText::EMPTY:       text = "";   break;
        default: throw std::runtime_error("Unsupported text");
    }

    return color.append(text);
}

bool Card::CanBePlayedAfter(Card lastPlayedCard, bool isUno)
{
    std::set<CardText> specialTexts{CardText::SKIP, CardText::REVERSE, 
        CardText::DRAW_TWO, CardText::WILD, CardText::DRAW_FOUR};

    // special cards can not be played as the last one
    if (isUno && specialTexts.count(mText)) {
        return false;
    }

    // if the last played card is skip, you can only play a skip
    if (lastPlayedCard.mText == CardText::SKIP) {
        return mText == CardText::SKIP;
    }

    // if the last played card is draw two, you can only play a draw two or draw four
    if (lastPlayedCard.mText == CardText::DRAW_TWO) {
        return (mText == CardText::DRAW_TWO || mText == CardText::DRAW_FOUR);
    }

    // if the last played card is draw four, you can only play a draw four
    if (lastPlayedCard.mText == CardText::DRAW_FOUR) {
        return mText == CardText::DRAW_FOUR;
    }

    // wild card can always be played except above conditions
    if (mColor == CardColor::BLACK) {
        return true;
    }

    // if not wild card, only cards with the same num or color can be played
    return (mColor == lastPlayedCard.mColor || mText == lastPlayedCard.mText);
}

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