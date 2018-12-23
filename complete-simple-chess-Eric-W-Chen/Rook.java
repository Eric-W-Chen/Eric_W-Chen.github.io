
/**
 *
 * @author Eric
 */
public class Rook extends ChessPiece {

    /**
     * constructor
     *
     * @param x
     * @param y
     */
    public Rook(int x, int y, String black) {
        super(x, y, "ROOK", black);
    }

    /**
     * move the rock
     *
     * @param nx
     * @param ny
     * @param board
     * @return true if the rook could be moved
     */
    @Override
    public boolean move(int nx, int ny, Board board) {

        // the path should be either sideway or forward
        if (x != nx && y != ny) {
            return false;
        }
        // check for obstacle
        return straightCheck(x, y, nx, ny, board);
    }

    /**
     * check vertical and horizontal path
     * @param x current x
     * @param y current y
     * @param nx new x
     * @param ny new y
     * @param board
     * @return true if we could move the piece
     */
    public static boolean straightCheck(int x, int y, int nx, int ny, Board board) {
        ChessPiece[][] cpses = board.getBoard();

        // forward
        if (x == nx) {

            // find start and stop position
            int min = 0;
            if(y > ny) {
                min = ny;
            }
            else {
                min = y;
            }
            int max = 0;

            if(y>ny){
                max = y;
            }
            else {
                max = ny;
            }

            // move along the path
            for (int i = min + 1; i < max; i++) {

                // if meeting an obstacle
                if (cpses[x][i] != null) {
                    return false;
                }
            }
        } // sideway
        else {
            
            // find start and stop position
            int min = 0;
            if (x > nx){
                min = nx;
            }
            else {
                min = x;
            }
            int max = 0;
            if (x > nx){
                max = x;
            }
            else{
                max = nx;
            }

            // move along the path
            for (int i = min + 1; i < max; i++) {
                
                // if meeting an obstacle
                if (cpses[i][y] != null) {
                    return false;
                }
            }
        }
        return true;
    }

}
