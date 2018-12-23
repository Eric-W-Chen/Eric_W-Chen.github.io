import java.util.Random;

public class Card {

    //Enums used instead of ints
    //enums so it can use both strings and ints. string of card and int of face value
    public static final int ACE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, SIX = 6, SEVEN = 7, EIGHT = 8, NINE = 9, TEN = 10, JACK = 10, QUEEN = 10, KING = 10,
            CLUBS = 11, DIAMONDS = 12, HEARTS = 13, SPADES = 14;

    Face face;
    SUIT suit;

    Card() {

        Face[] possibleValues  = new Face[] {
                Face.ACE, Face.TWO, Face.THREE, Face.FOUR, Face.FIVE, Face.SIX, Face.SEVEN, Face.EIGHT, Face.NINE, Face.TEN, Face.JACK, Face.QUEEN, Face.KING
        };

        SUIT[] possibleFaces = new SUIT[] {
                SUIT.DIAMONDS, SUIT.CLUBS, SUIT.HEARTS, SUIT.SPADES
        };

        Random random = new Random();
        this.suit = possibleFaces[random.nextInt(4)];
        this.face = possibleValues[random.nextInt(12)];

    }

    Card(Face faceValue, SUIT suitValue) {

        this.face = faceValue;
        this.suit = suitValue;

    }

    public int getFace() {
        return this.face.value;
    }

    public String getFaceName() {

        return this.face.name();
    }

    public int getSuit() {
        return this.suit.value;
    }

    public String getSuitName() {
        return this.suit.name();
    }

    public boolean isHigherThan(Card card2) {
        if (this.face.value > card2.face.value)
            return true;
        else
            return false;
    }

    public boolean isHigherThan(Card card2, boolean aceHigh) {
        if (this.face.value > card2.face.value)
            return true;
        else
            return false;
    }

    public String toString() {

        return "The face of this card is " + this.face.value + " and the value is " + this.face.value;

    }

}

enum SUIT {
// stores all suits in an enum
    DIAMONDS(1), SPADES(2), HEARTS(3), CLUBS(4);

    public int value;

    SUIT(int value) {
        this.value = value;
    }

}

enum Face {
//stores all faces in an enum.
    ACE(1), TWO(2), THREE(3), FOUR(4), FIVE(5), SIX(6), SEVEN(7), EIGHT(8), NINE(9), TEN(10), JACK(10), QUEEN(10), KING(10);

    public int value;

    Face(int value) {
        this.value = value;
    }

    public void setValue(int value) {
        this.value = value;
    }
}





