package W3.Java_2013311689_week2;

public class Book_2013311689 {

    private String bookName;
    private boolean state; // whether the book can be borrowed or not

    Book_2013311689(String bookName) {
        this.bookName = bookName;
        this.state = true; // false means "you cannot borrow the book"
    }

    String getName() { return this.bookName; }

    void setState(boolean type) {

        this.state = type;
        return;

    }

    boolean getState() {

        if (this.state == true) return true;
        else return false;

    }


}