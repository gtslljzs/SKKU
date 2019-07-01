package W3.Java_2013311689_week2;

public class Main_2013311689 {

    public static void main(String[] args) {

        Library_2013311689 lib = new Library_2013311689(3);

        lib.addBook(new Book_2013311689("K&R"));
        lib.addBook(new Book_2013311689("2046"));
        lib.addBook(new Book_2013311689("Particles"));
        lib.addBook(new Book_2013311689("Postman"));

        lib.findBook("2046");
        lib.findBook("Window is garbage");
        
        lib.borrowBook("So is mac");
        lib.borrowBook("K&R");

        lib.returnBook("K&R");
        lib.returnBook("2046");

        return;
    }
}