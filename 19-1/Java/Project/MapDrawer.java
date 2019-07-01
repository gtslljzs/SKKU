package Project;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics2D;

/**
 * This class is for drawing the bricks
 */
public class MapDrawer {

    public int map[][];
    public int brickWidth;
    public int brickHeight;

    /**
     * This Constructor receives the row and column to calculate brick's width and height
     * @param row   the row is the vertical position of a brick
     * @param col   the col is the horizontal position of a brick
     */
    public MapDrawer(int row, int col) {
        
        map = new int[row][col];
        for (int i = 0; i < map.length; i++)
            for (int j = 0; j < map[0].length; j++)
                map[i][j] = 1;
            
        brickWidth = 540 / col;
        brickHeight = 150 / row;
    }

    /**
     * This method practically draw the map
     * @param g     the g is {@link java.awt.Graphics2D} which is {@link javax.swing.JPanel} of {@link GameController} class
     */
    public void draw(Graphics2D g) {

        for (int i = 0; i < map.length; i++) {
            for (int j = 0; j < map[0].length; j++) {

                if (map[i][j] > 0) {
                    g.setColor(Color.CYAN);
                    g.fillRect(j * brickWidth + 80, i * brickHeight + 50, brickWidth, brickHeight);
                
                    g.setStroke(new BasicStroke(3));
                    g.setColor(Color.black);
                    g.drawRect(j * brickWidth + 80, i * brickHeight + 50, brickWidth, brickHeight);
                }
            }
        }

    }

    /**
     * This method is used to know whether the brick is broken or not
     * @param value     the value is 1 if brick is alive or 0 if brick was broken
     * @param row       the row is used to know the vertical position of a brick
     * @param col       the col is used to know the horizontal position of a brick
     */
    public void setBrickValue(int value, int row, int col) {
        map[row][col] = value;
    }

}