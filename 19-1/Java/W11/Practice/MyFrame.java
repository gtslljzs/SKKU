package W11.Practice;
import javax.swing.*;
import java.awt.*;

public class MyFrame extends JFrame {
    
    MyFrame() {
        setTitle ("First Frame");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(null);

        JLabel label = new JLabel("Hey!");
        label.setBounds(130, 50, 200, 20);
        add(label);

        for (int i = 1; i < 10; i++) {
            JButton button = new JButton(Integer.toString(i));
            button.setBounds(i * 15, i * 15, 50, 20);
            add(button);
        }
    
        setSize (300, 300); // pixel
        setVisible (true);
        // ImageIcon imageIcon = new ImageIcon("C:\\Users\\venv\\Google Drive\\Pictures\\e2dfeb485d9b16fd8cea0befb188e3157df71a4d310f1b44bcb0fdbd07538f25.png");
        // // JLabel label1 = new JLabel("Image and text", imageIcon);
        // JLabel label2 = new JLabel(imageIcon);
    
    }

    public static void main (String[] args) {
        
        // MyFrame myFrame = new MyFrame();
        new MyFrame();
    }
}