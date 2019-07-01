package Project;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

import javax.swing.Timer;
import javax.swing.JPanel;

/**
 * This class is for drawing Panel containing map and balls
 */
public class GameController extends JPanel implements KeyListener, ActionListener {

    private Recorder recorder;

    private boolean start = false;
    private boolean play = false;
    private boolean victory = false;
    private boolean gameOver = false;
    private int score = 0;
    private int bestScore = 0;
    private int stage = 2;  
    private int numBalls = 0;
    private Ball balls[];
    
    private int totalBricks = 21;
    
    private Timer timer;
    private int delay = 8;

    private int playerX = 310;

    private int ballPosX = 100;
    private int ballPosY = 350;
    private int ballXdir = 1;
    private int ballYdir = 2;

    private MapDrawer mapDrawer;

    public GameController() {
        
        loadRecord();
        mapDrawer = new MapDrawer(3, 7);
        addKeyListener(this);
        setFocusable(true);
        setFocusTraversalKeysEnabled(false);
        timer = new Timer(delay, this);
        timer.start();
        initBall();

    }

    public void loadRecord() {

        this.recorder = null;

        try {
            FileInputStream fis = new FileInputStream(".\\record.ser");
            ObjectInputStream ois = new ObjectInputStream(fis);
            this.recorder = (Recorder) ois.readObject();
            System.out.println("best: " + this.recorder.getBestScore());
            ois.close();
            fis.close();
            this.bestScore = this.recorder.getBestScore();
        } catch(IOException e) {
            e.getMessage();
            e.printStackTrace();
        } catch(ClassNotFoundException e) {
            e.printStackTrace();
            System.out.println("Cannot find the Recorder class");
        }
    }

    public void saveRecord() {
        
        try {
            FileOutputStream fos = new FileOutputStream(".\\record.ser");
            ObjectOutputStream oos = new ObjectOutputStream(fos);
            oos.writeObject(this.recorder);
            oos.close();
            fos.close();
        } catch(IOException e) {
            e.getMessage();

        }
    }

    public synchronized void initBall() {
        
        int ballPosX;
        int ballPosY;
        
        this.balls = new Ball[this.stage];

        for (int i = 0; i < this.stage; i++) {
            
            ballPosX = this.ballPosX + (int) (Math.random() * 500);
            ballPosY = this.ballPosY - (int) (Math.random() * 100);
            this.balls[i] = new Ball(ballPosX, ballPosY, this.ballXdir, this.ballYdir);
            this.numBalls++;
            Thread thread = new Thread(balls[i]);
            thread.start();
            System.out.println("ball is created");
        }
        System.out.println("number of balls: " + this.numBalls);
        
    }

    public void paint(Graphics g) {

        // Draw the background
        g.setColor(Color.black);
        g.fillRect(0, 0, 692, 592);

        // Draw the map
        mapDrawer.draw((Graphics2D)g);

        // Show stage
        g.setColor(Color.WHITE);
        g.setFont(new Font("serif", Font.BOLD, 25));
        g.drawString("Stage: "+ this.stage, 10, 30);

        // Show scores
        g.setColor(Color.WHITE);
        g.setFont(new Font("serif", Font.BOLD, 25));
        g.drawString(""+score, 590, 30);

        // Draw the paddle
        g.setColor(Color.green);
        g.fillRect(playerX, 550, 100, 8);

        // Draw balls
        for (Ball ball: this.balls)
            ball.drawBall((Graphics2D)g);

        // Show guide
        if (!this.start) {
            
            g.setColor(Color.RED);
            g.setFont(new Font("serif", Font.BOLD, 30));
            g.drawString("Break all Bricks and get score", 160, 300);

            g.setFont(new Font("serif", Font.BOLD, 20));
            g.drawString("You can move your paddle with LEFT and RIGHT key", 120, 350);

            g.setFont(new Font("serif", Font.BOLD, 20));
            g.drawString("Best score: " + this.bestScore, 290, 400);
        
            g.setFont(new Font("serif", Font.BOLD, 20));
            g.drawString("Press Enter to Start", 260, 450);

            g.setFont(new Font("serif", Font.BOLD, 20));
            g.drawString("Press ESC to Quit", 270, 500);
        }
        
        // Victory
        if (totalBricks <= 0) {
            play = false;
            for (Ball ball: this.balls) {
                ball.ballXdir = 0;
                ball.ballYdir = 0;
            
            }
            
            g.setColor(Color.RED);
            g.setFont(new Font("serif", Font.BOLD, 30));
            g.drawString("You Win, Score: " + score, 220, 300);

            g.setFont(new Font("serif", Font.BOLD, 20));
            g.drawString("Press Enter to Start Next Stage", 220, 350);

            g.setFont(new Font("serif", Font.BOLD, 20));
            g.drawString("Press ESC to Quit", 270, 400);
            
            this.victory = true;
        }

        // A ball falls down
        for (Ball ball: this.balls) {
            if (ball.ballPosY >= 590) {
                ball.live = false;
                ball.interrupt();
                this.numBalls--;
                ball.ballXdir = 0;
                ball.ballYdir = 0;
                ball.ballPosX = -20;
                ball.ballPosY = -20;
                this.score -= 50;
                if (this.score < 0) this.score = 0;
                System.out.println("ball is dead");
                System.out.println("number of balls: " + this.numBalls);
            }
        }
        
        // Game over
        if (this.numBalls <= 0) {
            play = false;

            g.setColor(Color.RED);
            g.setFont(new Font("serif", Font.BOLD, 30));
            g.drawString("Game Over, Score: " + score, 220, 300);

            g.setFont(new Font("serif", Font.BOLD, 20));
            g.drawString("Press Enter to Restart", 250, 400);
            this.gameOver = true;

            g.setFont(new Font("serif", Font.BOLD, 20));
            g.drawString("Press ESC to Quit", 270, 450);

            if (this.score > this.bestScore) {
                g.setFont(new Font("serif", Font.BOLD, 20));
                g.drawString("Best Score!", 290, 350);

                this.bestScore = this.score;
                this.recorder.setBestScore(this.bestScore);
                saveRecord();
                System.out.println("saved");
                
            }

        }

        g.dispose();

    }

    @Override
    public void actionPerformed(ActionEvent e) {

        timer.start();

        if (play) {

            for (Ball ball: this.balls) {

                if (ball.live) {
                    
                    // ball direction when meet the paddle
                    if (new Rectangle(ball.ballPosX, ball.ballPosY, 20, 20).intersects(new Rectangle(playerX, 550, 20, 8))) {
                        ball.ballYdir = -ball.ballYdir;
                        if (ball.ballXdir == -1) {
                            ball.ballXdir = -2;
                            ball.ballYdir = -1;
                        } else if (ball.ballXdir == 1) {
                            ball.ballXdir = 0;
                            ball.ballYdir = -2;
                        } else if (ball.ballXdir == 0) {
                            ball.ballXdir = -1;
                            ball.ballYdir = -2;
                        } else if (ball.ballXdir == 2) {
                            ball.ballXdir = 1;
                            ball.ballYdir = -2;
                        }
                    } else if (new Rectangle(ball.ballPosX, ball.ballPosY, 20, 20).intersects(new Rectangle(playerX + 20, 550, 60, 8))) {
                        ball.ballYdir = -ball.ballYdir;
                    } else if (new Rectangle(ball.ballPosX, ball.ballPosY, 20, 20).intersects(new Rectangle(playerX +80, 550, 20, 8))) {
                        ball.ballYdir = -ball.ballYdir;
                        if (ball.ballXdir == -2) {
                            ball.ballXdir = -1;
                            ball.ballYdir = -2;
                        } else if (ball.ballXdir == -1) {
                            ball.ballXdir = 0;
                            ball.ballYdir = -2;
                        } else if (ball.ballXdir == 0) {
                            ball.ballXdir = 1;
                            ball.ballYdir = -2;
                        } else if (ball.ballXdir == 1) {
                            ball.ballXdir = 2;
                            ball.ballYdir = -1;
                        }
                    }
                    
                    // check whether a ball breaks a brick
                    A: for (int i = 0; i < mapDrawer.map.length; i++) {
                        for (int j = 0; j < mapDrawer.map[0].length; j++) {
                            
                            if (mapDrawer.map[i][j] > 0) {
                            
                                int brickX = j * mapDrawer.brickWidth + 80;
                                int brickY = i * mapDrawer.brickHeight + 50;
                                int brickWidth = mapDrawer.brickWidth;
                                int brickHeight = mapDrawer.brickHeight;

                                Rectangle rect = new Rectangle(brickX, brickY, brickWidth, brickHeight);
                                Rectangle ballRect = new Rectangle(ball.ballPosX, ball.ballPosY, 20, 20);
                                Rectangle brickRect = rect;

                                if (ballRect.intersects(brickRect)) {
                                    
                                    mapDrawer.setBrickValue(0, i, j);
                                    totalBricks--;
                                    score += 5 * this.stage;

                                    if (ball.ballPosX + 19 <= brickRect.x || ball.ballPosX + 1 >= brickRect.x + brickRect.width)
                                        ball.ballXdir = -ball.ballXdir;
                                    else
                                        ball.ballYdir = -ball.ballYdir;

                                    break A;
                                }

                            }
                        }
                    }
                    
                    ball.ballPosX += ball.ballXdir;
                    ball.ballPosY += ball.ballYdir;

                    if (ball.ballPosX < 0) ball.ballXdir = -ball.ballXdir;
                    if (ball.ballPosY < 0) ball.ballYdir = -ball.ballYdir;
                    if (ball.ballPosX > 670) ball.ballXdir = -ball.ballXdir;
                }
            }
        }
        
        // draw a map again
        repaint();
    }

    @Override
    public void keyPressed(KeyEvent e) {

        if (e.getKeyCode() == KeyEvent.VK_RIGHT) {

            if (playerX >= 600) playerX = 600;
            else playerX += 20;
        }
        if (e.getKeyCode() == KeyEvent.VK_LEFT) {

            if (playerX < 10) playerX = 10;
            else playerX -= 20;
        }

        if (e.getKeyCode() == KeyEvent.VK_ENTER) {
            if (!play) {
                this.start = true;
                play = true;

                if (this.gameOver) {

                    System.out.println("game over");
                    this.stage = 1;
                    this.numBalls = 0;
                    playerX = 310;
                    totalBricks = 21;
                    score = 0;
                    mapDrawer = new MapDrawer(3, 7);
                    initBall();
                    this.gameOver = false;

                } else if (this.victory) {

                    System.out.println("victory");
                    this.stage++;
                    this.numBalls = 0;
                    playerX = 310;
                    totalBricks = 21;
                    mapDrawer = new MapDrawer(3, 7);
                    System.out.println("stage: " + this.stage);
                    initBall();
                    this.victory = false;
                }

                // keep drawing the map
                repaint();
            }
        }

        if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
            System.exit(0);
        }

    }

    @Override
    public void keyTyped(KeyEvent e) {}

    @Override
    public void keyReleased(KeyEvent e) {}

    class Ball extends Thread {
    
        boolean live;
        int ballPosX;
        int ballPosY;
        int ballXdir;
        int ballYdir;
    
        Ball(int _ballPosX, int _ballPosY, int _ballXdir, int _ballYdir) {
            this.ballPosX = _ballPosX;
            this.ballPosY = _ballPosY;
            this.ballXdir = _ballXdir;
            this.ballYdir = _ballYdir;
            this.live = true;
    
        }
    
        public void drawBall(Graphics2D _g) {
    
            // draw a ball
            _g.setColor(Color.white);
            _g.fillOval(this.ballPosX, this.ballPosY, 20, 20);
    
        }
    
        @Override
        public void run() {

            // keep drawing the ball
            try {
                repaint();
                Thread.sleep(1);
            } catch (InterruptedException e) {

            }
            
        }
        
    }
}

class Recorder implements Serializable {

    private int bestScore;
    
    public void setBestScore(int _bestScore) {
        this.bestScore = _bestScore;
    }

    public int getBestScore() {
        return this.bestScore;
    }

}
