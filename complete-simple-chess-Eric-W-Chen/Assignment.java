
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

/**
 *
 * @author Eric
 */
public class Assignment {

    public static void main(String[] args) {
        // PART A. The Chess Board
        Board board = new Board(8);
        Board originalBoard = new Board(8);

        try {
            Scanner scanner = new Scanner(new File("input.txt"));
            // PART B. Place the chess pieces on the board.
            String line;
            boolean black = false;
            while (scanner.hasNext()) {
                
                // read line
                line = scanner.nextLine();
                
                // there is an empty live between 
                // piece declaration and move command
                if (line.isEmpty()) {
                    break;
                }
                
                // split line by space
                String[] sp = line.split("\\s+");
                // add piece to board
                switch (sp[0]) {
                    case "knight":
                        if(!black) {
                            board.putPiece(new Knight(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-n-"));
                            originalBoard.putPiece(new Knight(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-n-"));
                        }
                        else {
                            board.putPiece(new Knight(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-N-"));
                            originalBoard.putPiece(new Knight(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-N-"));
                        }
                        break;
                    case "pawn":
                        if(!black) {
                            board.putPiece(new Pawn(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-p-"));
                            originalBoard.putPiece(new Pawn(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-p-"));
                        }
                        else {
                            board.putPiece(new Pawn(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-P-"));    
                            originalBoard.putPiece(new Pawn(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-P-"));                           
                        }
                        break;
                    case "rook":
                        if(!black) {
                            board.putPiece(new Rook(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-r-"));
                            originalBoard.putPiece(new Rook(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-r-"));
                        }
                        else {
                            board.putPiece(new Rook(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-R-"));
                            originalBoard.putPiece(new Rook(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-R-"));
                        }
                        break;
                    case "bishop":
                        if(!black) {
                             board.putPiece(new Bishop(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-b-"));
                             originalBoard.putPiece(new Bishop(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-b-"));
                        }
                        else {
                            board.putPiece(new Bishop(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-B-"));
                            originalBoard.putPiece(new Bishop(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-B-"));
                        }
                        break;
                    case "queen":
                        if(!black) {
                            board.putPiece(new Queen(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-q-"));
                            originalBoard.putPiece(new Queen(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-q-"));
                        }
                        else {
                            board.putPiece(new Queen(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-Q-"));
                            originalBoard.putPiece(new Queen(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-Q-"));
                        }
                        break;
                    case "king":
                        if(!black) {
                            board.putPiece(new King(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-k-"));
                            originalBoard.putPiece(new King(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-k-"));
                        }
                        else {
                            board.putPiece(new King(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-K-"));
                            originalBoard.putPiece(new King(Integer.parseInt(sp[1]),
                                Integer.parseInt(sp[2]), "-K-"));
                        }
                        break;
                    case "something":
                        black = true;
                    default:
                        break;
                }
            }
            System.out.println(board.toString());
            
            Scanner input = new Scanner(System.in);
            String source, destination;
            boolean done = false;
            boolean player1 = true;
            boolean again = true;
            while (again) {
                while (!done) {
                    if (player1) {
                        System.out.println("Player 1, it's your turn.");
                    }
                    else {
                        System.out.println("Player 2, it's your turn.");
                    }
                    System.out.println("Enter your chess piece to move: ");
                    source = input.nextLine();
                    System.out.println("Move where?: ");
                    destination = input.nextLine();
                    int x = Integer.parseInt(source.split(" ")[0]);
                    int y = Integer.parseInt(source.split(" ")[1]);
                    int nx = Integer.parseInt(destination.split(" ")[0]);
                    int ny = Integer.parseInt(destination.split(" ")[1]);
                    ChessPiece cp = board.getPiece(x, y);
                    ChessPiece dest = board.getPiece(nx, ny);
                    board.removePiece(cp);
                    ChessPiece ncp = null;
                    String type = cp.getType();
                    switch(type){
                        case "KNIGHT":
                            if(player1) {
                                ncp = new Knight(nx, ny, "-n-");
                            }
                            else {
                                ncp = new Knight(nx, ny, "-N-");
                            }
                            break;
                        case "PAWN":
                            if(player1) {
                                ncp = new Pawn(nx, ny, "-p-");
                            }
                            else {
                                ncp = new Pawn(nx, ny, "-P-");
                            }                        
                            break;
                        case "ROOK":
                            if(player1) {
                                ncp = new Rook(nx, ny, "-r-");
                            }
                            else {
                                ncp = new Rook(nx, ny, "-R-");
                            }                        
                            break;
                        case "BISHOP":
                            if(player1) {
                                ncp = new Bishop(nx, ny, "-b-");
                            }
                            else {
                                ncp = new Bishop(nx, ny, "-B-");
                            }                        
                            break;
                        case "QUEEN":
                            if(player1) {
                                ncp = new Queen(nx, ny, "-q-");
                            }
                            else {
                                ncp = new Queen(nx, ny, "-Q-");
                            }                        
                            break;
                        case "KING":
                            if(player1) {
                                ncp = new Knight(nx, ny, "-k-");
                            }
                            else {
                                ncp = new Knight(nx, ny, "-K-");
                            }                        
                            break;
                        default:
                            break;
                    }
                    board.putPiece(ncp);
                    String change;
                    if(player1) {
                        if(ncp.getType() == "PAWN" && ncp.getY() == 7) {
                            System.out.println("Change your pawn to what piece?: ");
                            change = input.nextLine().toUpperCase();
                            ncp.setType(change);
                            switch(change){
                                case "KNIGHT":
                                    ncp.setSign("-n-");
                                    break;
                                case "PAWN":
                                    ncp.setSign("-p-");               
                                    break;
                                case "ROOK":
                                    ncp.setSign("-r-");               
                                    break;
                                case "BISHOP":
                                    ncp.setSign("-b-");               
                                    break;
                                case "QUEEN":
                                    ncp.setSign("-q-");                     
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                    else {
                        if(ncp.getType() == "PAWN" && ncp.getY() == 0) {
                            System.out.println("Change your pawn to what piece?: ");
                            change = input.nextLine().toUpperCase();
                            ncp.setType(change);
                            switch(change){
                                case "KNIGHT":
                                    ncp.setSign("-N-");
                                    break;
                                case "PAWN":
                                    ncp.setSign("-P-");               
                                    break;
                                case "ROOK":
                                    ncp.setSign("-R-");               
                                    break;
                                case "BISHOP":
                                    ncp.setSign("-B-");               
                                    break;
                                case "QUEEN":
                                    ncp.setSign("-Q-");
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                    if(dest != null && dest.getType() == "KING") {
                        done  = true;
                    }
                    System.out.println(board.toString());
                    player1 = !player1;
                }
                if(player1) {
                    System.out.println("Player 2 wins");
                }
                else {
                    System.out.println("Player 1 wins");
                }
                System.out.println("Play again? [y,n]: ");
                String yes = input.nextLine();
                if(yes.equals("y")) {
                    again = true;
                    done = false;
                    player1 = true;
                    board = originalBoard;
                    System.out.println(board.toString());
                }
                else {
                    again = false;
                }
            }

            scanner.close();
        } catch (FileNotFoundException ex) {
            System.err.println(ex.getMessage());
        } catch (OutOfBoardException ex) {
            System.err.println(ex.getMessage());
        }
    }

}
