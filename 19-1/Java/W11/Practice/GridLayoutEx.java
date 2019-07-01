package W11.Practice;
import java.awt.*;
import javax.swing.*;

public class GridLayoutEx extends JFrame {

    public GridLayoutEx() {
        
        setTitle("GridLayout Example");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Container contentPane = getContentPane();

        // 1*10 pixel
        contentPane.setLayout(new GridLayout(1, 10));
        
        for (int i = 0; i < 10; i++) {
            String text = Integer.toString(i);
            JButton button = new JButton(text);
            contentPane.add(button);
        }

        setSize(500, 200);
        setVisible(true);

    }

    public static void main(String[] args) {
        
        new GridLayoutEx();
        
    }
}