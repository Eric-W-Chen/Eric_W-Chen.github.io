
/**
 *
 * @author Rric
 */
public class Queen extends ChessPiece {

    public Queen(int x, int y, String black) {
        super(x, y, "QUEEN", black);
    }

    @Override
    public boolean move(int nx, int ny, Board board) {
        
        // the queen is both bishop and rook
        if (Math.abs(x - nx) == Math.abs(y - ny)) {
            return Bishop.diagonalCheck(x, y, nx, ny, board);
        }
        if (x == nx || y == ny) {
            return Rook.straightCheck(x, y, nx, ny, board);
        }
        return false;
    }

}
