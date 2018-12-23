
/**
 *
 * @author Eric
 */
public class Board {

    private ChessPiece[][] board;
    private int size;

    /**
     * constructor 
     * @param size 
     */
    public Board(int size) {
        this.board = new ChessPiece[size][size];
        this.size = size;
    }

    public ChessPiece getPiece(int x, int y) {
        if(board[x][y] == null) {
            return null;
        }
        ChessPiece rv = board[x][y];
        return rv;
    }

    public void removePiece(ChessPiece cp) {
        board[cp.getX()][cp.getY()] = null;
    }

    /**
     * put piece on the board
     * @param cp
     * @throws OutOfBoardException 
     */
    public void putPiece(ChessPiece cp) throws OutOfBoardException {
        int x = cp.getX();
        int y = cp.getY();
        
        //bad case 
        if (outOfBoard(x, y)) {
            throw new OutOfBoardException();
        }        
        
        board[x][y] = cp;
    }

    /**
     * move piece on the board 
     * @param x
     * @param y
     * @param nx
     * @param ny
     * @return 
     */
    public boolean movePiece(int x, int y, int nx, int ny) {

        try {
            // checking for legal input
            if (outOfBoard(x, y) || outOfBoard(nx, ny)) {
                throw new OutOfBoardException();
            }
            
            // get the piece 
            ChessPiece piece = board[x][y];
            
            // there isn't any piece 
            if (piece == null) {
                throw new IllegalArgumentException();
            }
            
            // the destination is not empty
            if (board[nx][ny] != null) {
                throw new PathwayException();
            }
            
            // test it
            if (piece.move(nx, ny, this) == false) {
                throw new PathwayException();
            }
            
            // move it 
            board[nx][ny] = board[x][y];
            board[x][y] = null;
            // show message
            System.out.printf("%s: (%d,%d) (%d,%d)\n",
                    piece.getType(),
                    x, y, nx, ny);
            System.out.printf("Moved! (%d,%d) (%d,%d)\n", x, y, nx, ny);
            return true;
        } catch (OutOfBoardException | PathwayException | IllegalArgumentException e) {
            System.out.printf("Invalid Board Position. (%d,%d) (%d,%d)\n",
                    x, y, nx, ny);
        }
        return false;
    }

    public ChessPiece[][] getBoard() {
        return board;
    }

    private boolean outOfBoard(int x, int y) {
        if (x < 0 || y < 0 || x >= size || y >= size) {
            return true;
        }
        return false;
    }

    @Override
    public String toString() {
        String s = "";
        int line = 7;

        for (int i = size - 1; i >= 0; i--) {
            s += "=" + line + "= ";
            for (int j = 0; j < size; j++) {
                ChessPiece piece = board[j][i];
                if (piece == null) {
                    s += "--- ";
                } else {
                    s += piece.getSign() + " ";
                }
            }
            s += System.lineSeparator();
            line--;
        }

        s += "=== ";
        for (int i = 0; i < board.length; i++) {
            s += "=" + i + "= ";
        }
        s += System.lineSeparator();

        return s;
    }

}
