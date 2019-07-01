package W4.Practice;

public class Cat extends Mammal {

    private String name;
    private double weight;
    private int age;
    private String kind;

    Cat() {
        this.name = new String("NULL");
        this.weight = 0;
        this.age = 0;
        this.kind = new String("Mammal");
    }
    Cat(String name, double weight, int age) {
        this.name = name;
        this.weight = weight;
        this.age = age;
        this.kind = new String("Mammal");
    }

    public void printCatInfo() {

        if (this.name.equals("NULL") && this.age == 0 && this.weight == 0) {
            
            System.out.println("There is no information yet.");

        } else {

            System.out.println(this.name + "'s Info!!");
            System.out.println("Weight(kg): " + this.weight);
            System.out.println("Age: " + this.age);
        
        }

    }

    @Override
    public void printKind() {

        System.out.println("This animal is a " + this.kind);

    }

    @Override
    public void setNumBabies(int num) {

        this.num_babies = num;
    
    }

    @Override
    public int getNumMammals() {

        return this.num_babies;
    }

}