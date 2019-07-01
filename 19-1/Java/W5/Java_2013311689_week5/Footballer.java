package W5.Java_2013311689_week5;
import java.util.PriorityQueue;

public class Footballer implements Comparable<Footballer> {

    // fields
    String name;
    double salary;

    // constructor
    public Footballer(String name, double salary) {

        super();
        this.name = name;
        this.salary = salary;

    }

/**
 * this method makes 10 Footballer objects, inserts them into priority queue
 * and returns priority queue
 * @param
 * @return      return priority queue
 */
     private static PriorityQueue<Footballer> getPriorityQueue() {

        // initializing priority queue
        PriorityQueue<Footballer> priorityQueue = new PriorityQueue<Footballer>();
        // constructing objects
        Footballer footballer1 = new Footballer("Cristiano Ronaldo", 1187);
        Footballer footballer2 = new Footballer("Lionel Messi", 1220);
        Footballer footballer3 = new Footballer("Neymar Jr", 989);
        Footballer footballer4 = new Footballer("Alexis Sanchez", 350);
        Footballer footballer5 = new Footballer("Gareth Bale", 380);
        Footballer footballer6 = new Footballer("Mesut Ozil", 288);
        Footballer footballer7 = new Footballer("Oscar", 301);
        Footballer footballer8 = new Footballer("Paul Pogba", 324);
        Footballer footballer9 = new Footballer("Wayne Rooney", 297);
        Footballer footballer10 = new Footballer("Thiago Silva", 246);

        // insert objects into priority queue
        priorityQueue.offer(footballer1);
        priorityQueue.offer(footballer2);
        priorityQueue.offer(footballer3);
        priorityQueue.offer(footballer4);
        priorityQueue.offer(footballer5);
        priorityQueue.offer(footballer6);
        priorityQueue.offer(footballer7);
        priorityQueue.offer(footballer8);
        priorityQueue.offer(footballer9);
        priorityQueue.offer(footballer10);

        return priorityQueue;
    }

/**
 * This method compares the footballer obejcts by their salary
 * @param target    the target is the object of Footballer class
 * @return          return 1 or -1
 */
    @Override
    public int compareTo(Footballer target) {
        
        // compares the objects by their salary
        if (this.salary < target.salary) return 1;
        else if (this.salary > target.salary) return -1;
         
        return 0;
         
    }

    public static void main(String[] args) {

        PriorityQueue<Footballer> priorityQueue = getPriorityQueue();
        
        System.out.println("Here introduces " + priorityQueue.size() + " football players who receive largest Footballer in the world");
        System.out.println("Who receives the largest football player is " + priorityQueue.peek().name);
        System.out.println(priorityQueue.peek().name + " receives " + priorityQueue.peek().salary / 10 + " billion-won");
        System.out.println("The order is as follows ");
        while (!priorityQueue.isEmpty()) {

            Footballer footballer = priorityQueue.poll();
            String name = footballer.name;
            double salary = footballer.salary;
            System.out.println(name + ", " + salary / 10 + " billion-won");

        }

        return;
    }

}