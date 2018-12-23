
/**
 *
 * @author Eric
 */
public class Knight extends ChessPiece {

    public Knight(int x, int y, String black) {
        super(x, y, "KNIGHT", black);
    }

    @Override
    public boolean move(int nx, int ny, Board board) {
        // the knight move on the rectangle 
        // which one side is 1 and one side is 2
        return (Math.abs(x - nx) == 1 && Math.abs(y - ny) == 2)
                || (Math.abs(x - nx) == 2 && Math.abs(y - ny) == 1);

    }

}
