import java.util.ArrayList;
import java.util.Collections;
import java.util.List; //used list over arraylist to utilize collections api
import java.util.Scanner;
import java.util.stream.Collectors;

public class BlackJack {

    private static DeckOfCards deckOfCards = new DeckOfCards();
    private static List<Player> players = new ArrayList<>();

    private static Card hit() {
        Scanner scanner = new Scanner(System.in);
        Boolean done = false;

        Card cardDealt;//creates individual card object
        cardDealt = deckOfCards.deal();
        if (cardDealt.face.name().equals("ACE")) {

            while (!done) {//while not done
                System.out.println("Do you want the ACE to have the value of a 1 or 11?");
                int answer = scanner.nextInt();//bc ace has value of 1 or 11 you want to continuously ask the user to choose a vlue of 1 or 11 based on what he wants. while loop makes that happen so once user chooses 1 or 11 it breaks out. 
                if (answer == 1) {
                    cardDealt.face.setValue(1);
                    done = true;
                } else if (answer == 11) {
                    cardDealt.face.setValue(11);
                    done = true;
                } else {
                    System.out.println("That is not a valid answer");
                }
            }
        }

        return cardDealt;

    }

    private static void determineWinners() {

        Player winner = null;

        int highest = 0;

        List<Player> playersNotBusted = players.stream()//iterates through list and converts to another type of data structure so that it can be filtered
                .filter(player ->  player.hand.getHandCount() < 22) //each player that has less than 22 is not busted
                .collect(Collectors.toList()); //converts back to a list
                //Utilizing the collections library to produces cleaner and more readable code
                //Stream is a bit overkill but necessary to filter. 
                //players by getting only players that have hand count less than 22 
                //than converting the stream back to a list 
                //could have made loop but chose this instead as i wanted to try it out due to better performance

        for (Player player : playersNotBusted) {//for each player not busted, sets highest variable to highest handcount
            if (player.hand.getHandCount() > highest) {//determines highest handcount between 0 and 22
                highest = player.hand.getHandCount();//sets that as the highest
            }
        }

        int numberOfWinners = 0;
        for (Player player : playersNotBusted) {//for each player not busted checks who has the highest handcount
            if (player.hand.getHandCount() == highest) {//determines which player wins
                numberOfWinners++;
                winner = player;
            } else {
                player.losses++;
            }
        }

        if (numberOfWinners > 1) {
            System.out.println("There is a tie");
        } else {
            winner.wins++;
            System.out.println("The winner is player " + winner.getName());
        }

    }

    public static void main (String[] args) {

        boolean stillPlaying = true;

        deckOfCards.shuffle();

        Card cardDealt;//creates individual card object so each card dealt is diff

        Scanner scanner = new Scanner(System.in);

        int numberOfPlayers = 0;
        int activePlayers;

        while (numberOfPlayers < 2) {
            System.out.println("How many players are playing this game?");
            numberOfPlayers = scanner.nextInt();
            if (numberOfPlayers < 2)
                System.out.println("Number of players have to be greater than 1");
        }

        for (int i = 0; i < numberOfPlayers; i++ ) {
            players.add(new Player(i + 1));
        }

        while (stillPlaying) {

            activePlayers = numberOfPlayers; //players who are making a move

            //Deal the first two cards to each player
            for (Player player : players) {//does this for each player
                player.stillInGame = true;
                player.hand.clearHand();
                cardDealt = hit();
                player.hand.cards.add(cardDealt);
                cardDealt = hit();
                player.hand.cards.add(cardDealt);
                System.out.println("The hand count for Player " + player.getName() + " is " + player.hand.getHandCount());
                //Add player to game
            }

            while (activePlayers > 0) {

                for (Player player : players) {

                    if (player.isStillInGame()) {

                        System.out.println("Player " + player.getName() + " has " + player.wins + " wins, " + player.losses + " losses and their count is " + player.hand.getHandCount() + " , Do you want to Hit(1) or Stand(2)?");
                        int choice = scanner.nextInt();

                        //Handles Hit and Stand
                        switch (choice) {

                            case 1:
                                //Deal card (hit)
                                player.hand.cards.add(hit());//adding inside cards list whatever the hit method returns
                                System.out.println("The hand count for player " + player.getName() + " is now " + player.hand.getHandCount());//getting hand count
                                if (player.hand.getHandCount() > 21) {
                                    System.out.println("Player " + player.getName() + " has busted");
                                    player.stillInGame = false;
                                    activePlayers--;
                                    player.losses++;
                                }
                                break;

                            case 2:
                                //Player chooses to stand
                                player.stillInGame = false; //inactive for that turn
                                activePlayers--;//real-world. they are an active player they just don't make a move. 
                                break; // an active player is someone who is not hitting. so if two people stand, the game is over

                            default:
                                System.out.println("That was an invalid choice");
                                break;
                        }
                    }
                }
            }

            determineWinners();
            System.out.println("Do you want to play another game? Yes(1) No(Any other key)");
            if (!scanner.next().equals("1"))//if user input does not equal 1, then make stillPlaying = false which breaks out of the loop
                stillPlaying = false;
        }

    }

    static class Player {

        boolean stillInGame = true;
        private int name;
        int wins = 0, losses = 0;
        Hand hand;//creates individual hand object so each player has different cards

        Player(int name) {
            this.hand = new Hand();
            this.name = name;
        }

        public int getName() {
            return name;
        }

        public void setName(int name) {
            this.name = name;
        }

        public boolean isStillInGame() {
            return stillInGame;
        }

        public void setStillInGame(boolean stillInGame) {
            this.stillInGame = stillInGame;
        }

    }

}


