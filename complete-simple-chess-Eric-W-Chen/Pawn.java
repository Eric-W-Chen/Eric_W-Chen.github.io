
/**
 *
 * @author Eric
 */
public class Pawn extends ChessPiece {

    public Pawn(int x, int y, String black) {
    	super(x, y, "PAWN", black);
    }

    @Override
    public boolean move(int nx, int ny, Board board) {
        
        // the pwn can move forwad only 
        return ny - y == 2
                && nx == x;
    }

}
