package W11.Practice;
import javax.swing.*;
import java.awt.*;

public class BorderLayoutEx extends JFrame {

    public BorderLayoutEx() {

        setTitle("BorderLayout Example");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Container contentPane = getContentPane();

        contentPane.setLayout(new BorderLayout(30, 20));

        contentPane.add(new JButton("Calculate"), BorderLayout.CENTER);
        contentPane.add(new JButton("add"), BorderLayout.NORTH);
        contentPane.add(new JButton("sub"), BorderLayout.EAST);
        contentPane.add(new JButton("mul"), BorderLayout.SOUTH);
        contentPane.add(new JButton("div"), BorderLayout.WEST);

        setSize(300, 200);
        setVisible(true);

    }

    public static void main(String[] args) {

        new BorderLayoutEx();

    }
}