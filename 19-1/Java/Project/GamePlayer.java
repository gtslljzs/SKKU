package Project;

import Project.GameController;
import java.awt.Container;
import java.awt.event.MouseListener;

import javax.swing.JFrame;

public class GamePlayer extends JFrame {

    private GameController gameController = new GameController();

    GamePlayer() {

        setBounds(10, 10, 700, 600);
        setTitle("Brick Breaker");
        setResizable(false);
        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Container contentPane = getContentPane();
        contentPane.add(gameController);
    }

    public static void main(String[] args) {

        GamePlayer gamePlayer = new GamePlayer();     

    }
}