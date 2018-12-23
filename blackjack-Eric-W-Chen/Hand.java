import java.util.ArrayList;

public class Hand {

    public ArrayList<Card> cards = new ArrayList<>();
    private int handCount = 0;

    public int getHandCount() {

        int handCountTotal = 0;

        for (Card card : cards) {
            handCountTotal += card.getFace(); 
        }

        return handCountTotal;
    }

    public void clearHand() {
        cards.clear();
    }

    public String toString() {
        StringBuilder sb = new StringBuilder();//creates mutable so cards in hand change
        for (Card card : cards) {
            sb.append(card.toString());
            sb.append("\n");
        }

        return "The cards in the hand are the following : \n" + sb.toString();
    }
}
