package W11.HW_2013311689_week11.Assignment_2;
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;

public class Main extends JFrame {

    private JLabel label = new JLabel("Hello");

    public Main() {

        setTitle("Week 11 Assignment 2");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        Container contentPane = getContentPane();
        MouseEventListener mouseEventListener = new MouseEventListener();
        contentPane.addMouseListener(mouseEventListener);
        contentPane.addMouseMotionListener(mouseEventListener);
        
        contentPane.setLayout(null);
        label.setSize(80, 20);
        label.setLocation(100, 80);
        contentPane.add(label);

        setSize(300, 200);
        setVisible(true);

    }

    class MouseEventListener implements MouseListener, MouseMotionListener {
        
        public void mousePressed(MouseEvent e) {
            label.setLocation(e.getX(), e.getY());
        }

        public void mouseReleased(MouseEvent e) {}
        public void mouseClicked(MouseEvent e) {}
        public void mouseEntered(MouseEvent e) {}
        public void mouseExited(MouseEvent e) {}
        public void mouseDragged(MouseEvent e) {}
        public void mouseMoved(MouseEvent e) {}
    }

    public static void main(String[] args) {
        
        new Main();
    
    }
}
