

/**
 *
 * @author Eric
 */
public class Bishop extends ChessPiece{

    /**
     * constructor 
     * @param x
     * @param y 
     */
    public Bishop(int x, int y, String black) {
        super(x, y, "BISHOP", black);
    }
    
    /**
     * move the piece
     * @param nx
     * @param ny
     * @param board
     * @return true if it could be moved
     */
    @Override
    public boolean move(int nx, int ny, Board board) {
        
        // the move is legal when it moves along the
        // diagonal line of a square
        // both sides of the square shuold be equal
        if (Math.abs(x - nx) != Math.abs(y - ny)) {
            return false;
        }  
        
        // check for obstacles on the path
        return diagonalCheck(x, y, nx, ny, board);
    }
    
    /**
     * check diagonal line
     * @param x
     * @param y
     * @param nx
     * @param ny
     * @param board
     * @return true if there isn't any obstacle 
     */
    public static boolean diagonalCheck(int x, int y, int nx, int ny, Board board) {
        // choose the right way to increase or decrease the position  of x and y
        int sign_x = 0;
        if(x > nx) {
            sign_x = -1;
        }
        else {
            sign_x = 1;
        }
        int sign_y = 0;
        if(y > ny) {
            sign_y = -1;
        }
        else {
            sign_y = 1;
        }
        
        // started positions 
        int i = x + sign_x;
        int j = y + sign_y;
        
        ChessPiece[][] cpses = board.getBoard();
        
        // try to move along the path
        while (i != nx && j != ny) {            
            
            // stop if there is any obstacle on the path 
            if (cpses[i][j] != null) {
                return false;
            }
            
            i += sign_x;
            j += sign_y;
        }        
        
        return true;
    }
}
