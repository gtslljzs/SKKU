package W6.Java_2013311689_week6;
import java.util.Scanner;

public class Main_2013311689 {

    public static void main(String[] args) {

        Library lib = new Library(1);

        try {

            lib.addBook(new Book("K&R"));

        } catch (LibraryFullException e) {
            
            System.out.println("Library is full");

        }

        try {

            lib.addBook(new Book("2046"));

        } catch (LibraryFullException e) {

            System.out.println("Library is full");

        }

        try {

            lib.findBook("Window is garbage");

        } catch (NoBookException e) {

            System.out.println("No book with that name");
        
        }

        try {

            lib.borrowBook("Window is garbage");

        } catch (NoBookException e) {

            System.out.println("No book with that name");

        } catch (BorrowBookException e ) {

            System.out.println("Book is already borrowed");

        }

        try {

            lib.borrowBook("K&R");

        } catch (NoBookException e) {

            System.out.println("No book with that name");

        } catch (BorrowBookException e ) {

            System.out.println("Book is already borrowed");

        }

        try {

            lib.returnBook("2045");

        } catch (NoBookException e) {

            System.out.println("No book with that name");

        } catch (BorrowBookException e ) {

            System.out.println("Book is not borrowed");

        }

    }
}

class Book {

    private String book_name;
    private boolean state;

    Book() {}
    Book(String name) {
        
        this.book_name = name;
        this.state = true;

    }

    String getName() { return this.book_name; }

    void setState(boolean type) {

        this.state = type;
        return;

    }

    boolean getState() {

        if (this.state == true) return true;
        else return false;

    }
}

class Library {

    private int max_num_books;
    private int cur_num_books;
    private Book[] books;

    Library() {}
    Library(int num_books) { 

        this.books = new Book[num_books];
        this.max_num_books = num_books;
        this.cur_num_books = 0;

    }

    void addBook(Book new_book) throws LibraryFullException {

        if (this.cur_num_books >= this.max_num_books) throw new LibraryFullException();
        else {

            this.books[this.cur_num_books] = new_book;
            this.cur_num_books++;
            System.out.println("Successfully added the book");

        }
    }

    void findBook(String book_name) throws NoBookException {

        for(Book book : this.books) {

            if (book.getName().equals(book_name)) {

                System.out.println("Successfully found the book");
            
                return;
            }
        }

        throw new NoBookException();

    }

    void borrowBook(String book_name) throws NoBookException, BorrowBookException {

        for (Book book : this.books) {
            
            if (book_name.equals(book.getName())) {

                if(book.getState()) {

                    book.setState(false);
                    System.out.println("Successfully borrowed the book");
                    
                    return;                    
                } else throw new BorrowBookException();

            }
        }

        throw new NoBookException();

    }

    void returnBook(String book_name) throws NoBookException, BorrowBookException {

        for (Book book : this.books) {
        
            if (book_name.equals(book.getName()) && !book.getState()) {

                if (!book.getState()) {

                    book.setState(true);
                    System.out.println("Successfully returned the book");

                    return;
                } else throw new BorrowBookException();

            }
        }

        throw new NoBookException();

    }
}

class NoBookException extends Exception {

    NoBookException() { super(); }

}

class BorrowBookException extends Exception {

    BorrowBookException() { super(); }

}

class LibraryFullException extends Exception {

    LibraryFullException() { super(); }
    
}