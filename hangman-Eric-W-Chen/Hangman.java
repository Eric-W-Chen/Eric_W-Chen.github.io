
/**
 *
 * @author Eric Chen
 * @version 1.0
 * @since February 9, 2018 Hangman.java
 *
 * This program allows a user to play Hangman with the computer. --EXPLAIN THE
 * RULES--
 *
 */
import java.util.*;

public class Hangman {
    //This is an array of Strings
    // You should have 15 movie names to randomly choose from.

    static String movieList[] = {
        "Star Wars: The Last Jedi", "The Matrix", "Avatar",
        "The Lure", "City of Ghosts", "Super Dark Times",
        "Atomic Blonde", "Dawson City: Frozen Time", "The Blackcoat's Daughter",
        "I Called Him Morgan", "The Lost City of Z", "Phantom Thread",
        "I Don't Feel at Home in This World Anymore", "Marjorie Prime", "Lady Macbeth"};
    // add more to this list by adding more String Literals separated by commas

    /**
     * Entry point of the program
     *
     * @param args input arguments
     *
     */
    public static void main(String[] args) {
        // This code snippet just shows you how to use Stringbuilder. 
        // Change the code as you see fit.

        // 5pts - Randomly pick a movie from 15 preset movies.
        String movie = movieList[(int) (Math.random() * movieList.length)];
        String movieLowcase = movie.toLowerCase();

        // 20pts - Hide the selected movie using asterisks.
        StringBuilder currentGuess = guess(movie);
        
        // number of user's mistake
        int mistakes = 0;

        // previously guessed letter
        String guessed = "";

        // get user's input from keyboarad
        Scanner keyboard = new Scanner(System.in);
        char input;

        // 10pts - Allow 7 user mistakes
        while (mistakes < 7
                
                // the game is over when content of currentGuess matched 
                // with the content of movie
                && currentGuess.toString().equals(movie) == false) {            
            
            // 10pts - Provide a user friendly interface 
            // (Give user clear instructions and status)
            System.out.println("Current guess: " + currentGuess.toString());
            System.out.println("You have " + (7 - mistakes) + " mistake(s) left");
            System.out.println("Previously guessed letter: " + guessed);
            System.out.print("Enter a character: ");
            
            // get a string from input stream then get the fis character from 
            // the string
            input = keyboard.next().charAt(0);
            
            // turn the charater to its lower case 
            // to reduce the number of comparison below            
            input = Character.toLowerCase(input);

            // 10pts - Notify user of a previously guessed letter. 
            // (This does not count towards the 7 count mistake.)
            if (guessed.contains("" + input)) {
                System.out.println(input + " has been used");
            } else {
                // it should be a letter 
                if (Character.isLetter(input)      
                        // and it has to be in the movie name 
                        && movieLowcase.contains(input + "") == true) {
                    
                    //25pts - Correctly reveal the correct letter gueses.
                    for (int i = 0; i < movieLowcase.length(); i++) {
                        
                        // move to the right position, 
                        if (movieLowcase.charAt(i) == input) {
                            // remvoe the asterisk
                            currentGuess.deleteCharAt(i);
                            // insert the right character 
                            currentGuess.insert(i, movie.charAt(i));
                        }
                    }

                } else { // otherwise
                    mistakes++;
                }
                
                // add guess letter to the string 
                guessed += input;
            }
            System.out.println("------------\n");
        }
        System.out.println("--------------------------------");
        System.out.println("Current guess: " + currentGuess.toString());
        
        
        if (mistakes < 7) {
            System.out.println("You win");
        } else {
            System.out.println("You lose");
        }

    }

    /**
     * create a string builder 
     * @param movie
     * @return a string builder
     */
    private static StringBuilder guess(String movie) {
        StringBuilder currentGuess = new StringBuilder(movie.length());
        char c;
        for (int i = 0; i < movie.length(); i++) {
            c = movie.charAt(i);
            // 10pts - Allow movies to have titles with non-letter characters
            if (Character.isLetter(c) == true) {
                // turn letter to asterisk
                currentGuess.insert(i, "*");
            } else {
                // ingore punctuation 
                currentGuess.insert(i, c);
            }
        }
        return currentGuess;
    }
}
