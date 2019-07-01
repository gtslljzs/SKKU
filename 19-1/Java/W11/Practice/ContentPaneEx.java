package W11.Practice;
import javax.swing.*;
import java.awt.*;

public class ContentPaneEx extends JFrame {

    public ContentPaneEx() {

        setTitle("ContentPane and JFrame Example");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        Container contentPane = getContentPane();   // ContentPane
        contentPane.setBackground(Color.ORANGE);    // Background
        contentPane.setLayout(new FlowLayout());    // Layout Manager

        contentPane.add(new JButton("OK")); // OK Button
        contentPane.add(new JButton("Cancel")); // Cancel Button
        contentPane.add(new JButton("Ignore")); // Ignore Button

        setSize(300, 150);  // Frame set size
        setVisible(true);   // Print Frame

        // ImageIcon imageIcon = new ImageIcon("C:/Users/venv/Google Drive/Pictures/e2dfeb485d9b16fd8cea0befb188e3157df71a4d310f1b44bcb0fdbd07538f25.png");
        // JLabel label1 = new JLabel("Image and text", imageIcon);
        // JLabel label2 = new JLabel(imageIcon);
    
    }

    public static void main(String[] args) {

        new ContentPaneEx();

    }
}