package W11.HW_2013311689_week11.Assignment_1;
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.util.Random;

public class Main extends JFrame {

    public Main() {

        KeyPanel panel = new KeyPanel();
        setContentPane(panel);
        setSize(400, 250);
        setVisible(true);
        panel.requestFocus();

    }

    class KeyPanel extends JPanel {

        JLabel label = new JLabel("The <Enter> key changes the background color");
        
        public KeyPanel() {
            
            add(label);
            this.addKeyListener(new KeyAdapter() {
                public void keyPressed(KeyEvent e) {
                    
                    if (e.getKeyChar() == 'q') {
                        System.exit(0);
                    } else if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                        Random random = new Random();
                        int R = random.nextInt(256);
                        int G = random.nextInt(256);
                        int B = random.nextInt(256);
                        Color color = new Color(R, G, B);
                        
                        setBackground(color);
                        label.setText("r=" + R + ", g=" + G + ", b=" + B);
                    }

                }
            });

        }
    }

    public static void main(String[] args) {
        new Main();
    }
}