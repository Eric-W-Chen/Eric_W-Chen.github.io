import java.util.Collections;//API of methods
import java.util.Stack;
//could have created two array lists but chose this method
public class DeckOfCards {
//first in last out data structure
    Stack<Card> deck = new Stack();

    public DeckOfCards() {

        deck.push(new Card(Face.ACE, SUIT.DIAMONDS));
        deck.push(new Card(Face.TWO, SUIT.DIAMONDS));
        deck.push(new Card(Face.THREE, SUIT.DIAMONDS));
        deck.push(new Card(Face.FOUR, SUIT.DIAMONDS));
        deck.push(new Card(Face.FIVE, SUIT.DIAMONDS));
        deck.push(new Card(Face.SIX, SUIT.DIAMONDS));
        deck.push(new Card(Face.SEVEN, SUIT.DIAMONDS));
        deck.push(new Card(Face.EIGHT, SUIT.DIAMONDS));
        deck.push(new Card(Face.NINE, SUIT.DIAMONDS));
        deck.push(new Card(Face.TEN, SUIT.DIAMONDS));
        deck.push(new Card(Face.JACK, SUIT.DIAMONDS));
        deck.push(new Card(Face.QUEEN, SUIT.DIAMONDS));
        deck.push(new Card(Face.KING, SUIT.DIAMONDS));

        deck.push(new Card(Face.ACE, SUIT.SPADES));
        deck.push(new Card(Face.TWO, SUIT.SPADES));
        deck.push(new Card(Face.THREE, SUIT.SPADES));
        deck.push(new Card(Face.FOUR, SUIT.SPADES));
        deck.push(new Card(Face.FIVE, SUIT.SPADES));
        deck.push(new Card(Face.SIX, SUIT.SPADES));
        deck.push(new Card(Face.SEVEN, SUIT.SPADES));
        deck.push(new Card(Face.EIGHT, SUIT.SPADES));
        deck.push(new Card(Face.NINE, SUIT.SPADES));
        deck.push(new Card(Face.TEN, SUIT.SPADES));
        deck.push(new Card(Face.JACK, SUIT.SPADES));
        deck.push(new Card(Face.QUEEN, SUIT.SPADES));
        deck.push(new Card(Face.KING, SUIT.SPADES));

        deck.push(new Card(Face.ACE, SUIT.HEARTS));
        deck.push(new Card(Face.TWO, SUIT.HEARTS));
        deck.push(new Card(Face.THREE, SUIT.HEARTS));
        deck.push(new Card(Face.FOUR, SUIT.HEARTS));
        deck.push(new Card(Face.FIVE, SUIT.HEARTS));
        deck.push(new Card(Face.SIX, SUIT.HEARTS));
        deck.push(new Card(Face.SEVEN, SUIT.HEARTS));
        deck.push(new Card(Face.EIGHT, SUIT.HEARTS));
        deck.push(new Card(Face.NINE, SUIT.HEARTS));
        deck.push(new Card(Face.TEN, SUIT.HEARTS));
        deck.push(new Card(Face.JACK, SUIT.HEARTS));
        deck.push(new Card(Face.QUEEN, SUIT.HEARTS));
        deck.push(new Card(Face.KING, SUIT.HEARTS));

        deck.push(new Card(Face.ACE, SUIT.CLUBS));
        deck.push(new Card(Face.TWO, SUIT.CLUBS));
        deck.push(new Card(Face.THREE, SUIT.CLUBS));
        deck.push(new Card(Face.FOUR, SUIT.CLUBS));
        deck.push(new Card(Face.FIVE, SUIT.CLUBS));
        deck.push(new Card(Face.SIX, SUIT.CLUBS));
        deck.push(new Card(Face.SEVEN, SUIT.CLUBS));
        deck.push(new Card(Face.EIGHT, SUIT.CLUBS));
        deck.push(new Card(Face.NINE, SUIT.CLUBS));
        deck.push(new Card(Face.TEN, SUIT.CLUBS));
        deck.push(new Card(Face.JACK, SUIT.CLUBS));
        deck.push(new Card(Face.QUEEN, SUIT.CLUBS));
        deck.push(new Card(Face.KING, SUIT.CLUBS));

    }

    public Card deal() {
        Card cardDealt = deck.pop();
        System.out.println("The card that was dealt is " + cardDealt.getFaceName() + " of " + cardDealt.getSuitName());
        return cardDealt;
    }

    public int getNumCardsInDeck() {
        return deck.size(); //returns how many cards in deck
    }

    public boolean hasMoreCards() {
        if (deck.isEmpty()) //checks to see if there's any cards in the deck
            return false;
        else
            return true;
    }

    public void shuffle() {
        Collections.shuffle(deck);//utilizing shuffle methods in the collections class and shuffles the list
    }

}
