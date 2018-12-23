
/**
 *
 * @author Eric
 */
public abstract class ChessPiece {

    // position
    protected int x;
    protected int y;
    protected String type;
    protected String sign;

    /**
     * constructor 
     * @param x
     * @param y
     * @param type
     * @param sign 
     */
    public ChessPiece(int x, int y, String type, String sign) {
        this.x = x;
        this.y = y;
        this.type = type;
        this.sign = sign;
    }

    public String getSign() {
        return sign;
    }

    public void setSign(String sign) {
        this.sign = sign;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public abstract boolean move(int nx, int ny, Board board);
}
