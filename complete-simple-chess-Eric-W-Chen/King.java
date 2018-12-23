
/**
 *
 * @author Eric
 */
public class King extends ChessPiece{

    public King(int x, int y, String black) {
    	super(x, y, "KING", black);
    }

    @Override
    public boolean move(int nx, int ny, Board board) {  
        
        // the king move to near by slot
        return Math.abs(x - nx) <= 1 && Math.abs(y - ny) <= 1;
    }
    
}
