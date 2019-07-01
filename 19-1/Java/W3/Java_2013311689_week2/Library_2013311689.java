package W3.Java_2013311689_week2;

public class Library_2013311689 {

    private Book_2013311689[] books;
    private int maxNumBooks;
    private int curNumBooks;

    Library_2013311689(int numBooks) {

        this.books = new Book_2013311689[numBooks];
        this.maxNumBooks = numBooks;
        this.curNumBooks = 0;

    }

    void addBook(Book_2013311689 newBook) {

        if (this.curNumBooks < this.maxNumBooks) {
    
            for (int i = 0; i < this.curNumBooks; i++) {

                if (newBook.getName().equals(this.books[i].getName())) {

                    System.out.println("The book already exists\n");

                    return;
                }
            }
        
            this.books[this.curNumBooks] = newBook;
            this.curNumBooks++;
            System.out.println("Successfully added the book.\n");

            return;
        }
        
        System.out.println("Library is full.\n");

        return;
    }

    void findBook(String bookName) {

        for (Book_2013311689 book : this.books) {
            
            if (bookName.equals(book.getName())) {
                
                System.out.println("Found book.\n");

                return;
            }
        }

        System.out.println("There is no book with that name.\n");

        return;
    }

    void borrowBook(String bookName) {
        
        for (Book_2013311689 book : this.books) {
            
            if (bookName.equals(book.getName()) && book.getState()) {
                
                book.setState(false);
                System.out.println("Successfully borrowed the book.\n");
                
                return;
            }
        }

        System.out.println("There is no book available.\n");

        return;
    }

    void returnBook(String bookName) {

        for (Book_2013311689 book : this.books) {
        
            if (bookName.equals(book.getName()) && !book.getState()) {

                book.setState(true);
                System.out.println("Successfully returned.\n");
                
                return;
            }
        }

        System.out.println("There is no book borrowed with that name.\n");

        return;
    }

}