package W12.Practice;

class RunnableDemo implements Runnable {

    private Thread thread;
    private String thread_name;

    RunnableDemo(String name) {

        thread_name = name;
        System.out.println("Creating" + thread_name);

    }

    @Override
    public void run() {

        System.out.println("Running " + thread_name);
        
        try {

            for (int i = 4; i > 0; i--) {

                System.out.println("Thread: " + thread_name + ", " + i);

                // Let the thread sleep for a while
                Thread.sleep(50);
            }
        } catch (InterruptedException e) {

            System.out.println("Thread " + thread_name + " interrupted");
        }

        System.out.println("Thread " + thread_name + " exiting.");
    }

    public void start() {

        System.out.println("Starting " + thread_name);

        if (thread == null) {

            thread = new Thread(this, thread_name);
            thread.start();
        }
    }
}

public class ThreadTest {

    public static void main(String[] args) {

        RunnableDemo R1 = new RunnableDemo("Thread-1");
        R1.start();
        RunnableDemo R2 = new RunnableDemo("Thread-2");
        R2.start();

    }
}