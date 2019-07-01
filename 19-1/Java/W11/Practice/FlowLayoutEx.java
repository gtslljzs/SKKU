package W11.Practice;
import javax.swing.*;
import java.awt.*;

public class FlowLayoutEx extends JFrame {

    public FlowLayoutEx() {

        setTitle("FlowLayout Example");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Container contentPane = getContentPane();   // 

        // Left align, 30*40 pixel
        contentPane.setLayout(new FlowLayout(FlowLayout.LEFT, 30, 40));
        contentPane.add(new JButton("add"));
        contentPane.add(new JButton("sub"));
        contentPane.add(new JButton("mul"));
        contentPane.add(new JButton("div"));
        contentPane.add(new JButton("Calculate"));

        setSize(300, 200);  // Frame Size
        setVisible(true);   // print Frame

    }

    public static void main(String[] args) {

        new FlowLayoutEx();

    }
}