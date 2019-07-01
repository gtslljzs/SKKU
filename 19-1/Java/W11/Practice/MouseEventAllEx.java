package W11.Practice;
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;

public class MouseEventAllEx extends JFrame {

    private JLabel label = new JLabel("Move me");

    public MouseEventAllEx() {
        setTitle("MouseListener and MOuseMotionListener Example");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Container contentPane = getContentPane();
        MyMouseListener mouseListener = new MyMouseListener();
        contentPane.addMouseListener(mouseListener);
        contentPane.addMouseMotionListener(mouseListener);

        contentPane.setLayout(null);
        label.setSize(80, 20);
        label.setLocation(100, 80);
        contentPane.add(label);

        setSize(300, 200);
        setVisible(true);
    }

    class MyMouseListener implements MouseListener, MouseMotionListener {

        public void mousePressed(MouseEvent e) {
            label.setLocation(e.getX(), e.getY());
            setTitle("mousePressed("+ e.getX() + ", " + e.getY() + ")");
        }

        public void mouseReleased(MouseEvent e) {
            label.setLocation(e.getX(), e.getY());
            setTitle("mouseReleased("+ e.getX() + ", " + e.getY() + ")");    
        }

        public void mouseClicked(MouseEvent e) {}
        public void mouseEntered(MouseEvent e) {
            Component component = (Component)e.getSource();
            component.setBackground(Color.CYAN);
        }
        
        public void mouseExited(MouseEvent e) {
            Component component = (Component)e.getSource();
            component.setBackground(Color.YELLOW);
            setTitle("mouseExited(" + e.getX() + ", " + e.getY() + ")");
        }

        public void mouseDragged(MouseEvent e) {
            label.setLocation(e.getX(), e.getY());
            setTitle("mouseDragged("+ e.getX() + ", " + e.getY() + ")");   
        }
        
        public void mouseMoved(MouseEvent e) {
            label.setLocation(e.getX(), e.getY());
            setTitle("mouseMoved("+ e.getX() + ", " + e.getY() + ")");   
        }

    }

    public static void main(String[] args) {

        new MouseEventAllEx();
    }
}