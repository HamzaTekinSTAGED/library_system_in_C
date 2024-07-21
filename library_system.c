#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
// ý had to make constant max values to store accounts and books in arrays
#define MAX_CUSTOMERS 100
#define MAX_BOOKS 100
#define MAX_RENTALS 100

struct customer{
    int C_ID ;
    char name[50];
    char surname[50];
    int age;
    float wallet;
};
struct book{
    int B_ID ;
    char bookName[50];
    char author[50];
    int ageLimit;
    float priceperweek;
    int rented; //(1 means rented and 0 not rented )
    };
struct rental{
    int R_ID;
    int C_ID;
    int B_ID;
    char rantedDate [50];
    int week;
};

void AddCustomer(struct customer customers[], int *numcust){
    //create newcustomer struct for new customer
    struct customer newCustomer;


    printf("give name of customer\n");
    scanf("%s", newCustomer.name);


    printf("give surname of customer\n");
    scanf("%s", newCustomer.surname);

    // checking name and surname  because there cannot be 2 account have same name and surname
    for (int i = 0; i < *numcust; i++) {
        //we learned strcmp function in 8 lab
        if (strcmp(customers[i].name, newCustomer.name) == 0 && strcmp(customers[i].surname, newCustomer.surname) == 0) {
            printf("it already exist.\n");
            //I used return because there is no need to keep running the code
            return;
        }
    }

    printf("give age of customer\n");
    scanf("%d", &newCustomer.age);
    printf("give the amount of dollar in the wallet as a number\n");
    scanf("%f", &newCustomer.wallet);

    // choose ID number of customer as one plus to its place in the array to make it easy other calculations
    newCustomer.C_ID = *numcust + 1;
    customers[*numcust] = newCustomer;
    (*numcust)++;

    // I upload the datas to customers.txt
    // I used "w" version because when we update values of one customer I don't wanna write new line for it. it will change in its original line in txt file


   // I explained everything about saving process. So, I will not explain everytime
    FILE *cpoint = fopen("customers.txt", "w");
    for(int i =0; i<*numcust;i++){
    fprintf(cpoint, "%d\n%s\n%s\n%d\n%.2f\n", customers[i].C_ID, customers[i].name, customers[i].surname, customers[i].age, customers[i].wallet);
    }
    fclose(cpoint);

    printf("operation ended succesfully\n");

}

void Deposit(struct customer customers[], int numcust){
    // d equals to ID
    int d;
    float money;
    printf("Enter the ID number of the account you want to send money to\n");
    scanf("%d", &d);


    // checking ID in if statement
    // I started ID from 1 so index of the customer is customer ID minus 1
    if( d>=1 && d<=numcust){
        printf("Enter the amount of money to deposit:\n");
        scanf("%f",&money);
        customers[d-1].wallet += money;
    }else{
        printf("ID does not exist \n");
        return;
    }



    // saving datas to file
    FILE *cpoint = fopen("customers.txt", "w");
    for(int i =0; i<numcust;i++){
    fprintf(cpoint, "%d\n%s\n%s\n%d\n%.2f\n", customers[i].C_ID, customers[i].name, customers[i].surname, customers[i].age, customers[i].wallet);
    }
    fclose(cpoint);

    printf("operation ended succesfully\n");

}

void AddBook(struct book books[], int *numbooks){
    // I generally used same strategy with AddCustomer function
    //create newbook struct for new book
    struct book newBook;
    // for a new book it is impossible to be rented
    newBook.rented=0;

    printf("give name of book \n");
    scanf("%49s", newBook.bookName);
    printf("give name of author \n");
    scanf("%49s", newBook.author);
    // checking name of book and author  because there cannot be 2 book have same name and author
    for (int i = 0; i < *numbooks; i++) {
        if (strcmp(books[i].bookName, newBook.bookName) == 0 && strcmp(books[i].author, newBook.author) == 0) {
            printf("it already exist.\n");
            return;
        }
    }


    printf(" give the age limit for book \n");
    scanf("%d", &newBook.ageLimit);
    printf(" give the price per week to rent \n");
    scanf("%f", &newBook.priceperweek);

    // choose ID number of book as one plus to its place in the array to make it easy other calculations
    newBook.B_ID = *numbooks + 1;
    books[*numbooks] = newBook;
    (*numbooks)++;

    // to save informations of books in books.txt
    // same process with the customers.txt
    FILE *bpoint = fopen("books.txt", "w");
    if (bpoint == NULL) {
        printf("Error opening file.\n");
        return;
    }
    for(int i=0; i<*numbooks;i++){
    fprintf(bpoint, "%d\n%s\n%s\n%d\n%.2f\n%d\n", books[i].B_ID, books[i].bookName, books[i].author, books[i].ageLimit, books[i].priceperweek, newBook.rented);
    }
    fclose(bpoint);


    printf("operation ended succesfully\n");


}

void rentBook(struct customer customers[], int numcust,struct book books[], int *numbooks,struct rental rentals[], int *numrentals ){


    // b is the book ID
    int b;
    printf("say ID of the book\n");
    scanf("%d", &b);
    // index of the book equals to idnumber-1
    //checking the rental status of book. you cannot rent a book already rented
    if(books[b-1].rented==0){
        //c is the customer ID
        int c;
        printf("say ID of the customer\n");
        scanf("%d", &c);
        //checking age of the customer
        if(customers[c-1].age>=books[b-1].ageLimit){
            int week;
            printf("say how much week you want to rent\n");
            scanf("%d", &week);
            // checking money
            if( customers[c-1].wallet>= books[b-1].priceperweek * week ){
                //all conditions are right, rent operations start

                //withdraw the required money from the customer
                customers[c-1].wallet -= books[b-1].priceperweek * week;

                //changing rental status of book
                books[b-1].rented=1;

                //fill in all the data of the rental structure
                rentals[*numrentals].R_ID = *numrentals + 1;
                rentals[*numrentals].C_ID = c;
                rentals[*numrentals].B_ID = b;
                rentals[*numrentals].week = week;

                //current time finding resource https://www.youtube.com/watch?v=M1E3roUNTMY ,https://www.youtube.com/watch?v=inruYTVZdoM
                struct tm *ptr;
                time_t tim = time(0);
                ptr = localtime(&tim);
                strftime(rentals[*numrentals].rantedDate, 50, "%Y-%m-%d", ptr);

                //adding 1 to the rental num for next calculations
                (*numrentals)++;

            }else{
            printf("your money isn't enough\n");
            return;
            }


        }else{
        printf("your age is not enough\n");
        return;
        }
    }else{
        printf("it's already rented \n");
        return;
    }
    // updating book rented status
    FILE *bpoint = fopen("books.txt", "w");
    if (bpoint == NULL) {
        printf("Error opening books.txt\n");
        return;
    }
    for (int i = 0; i < *numbooks; i++) {
        fprintf(bpoint, "%d\n%s\n%s\n%d\n%f\n%d\n",
                books[i].B_ID, books[i].bookName, books[i].author, books[i].ageLimit, books[i].priceperweek, books[i].rented);
    }
    fclose(bpoint);

    // updating wallet values
    FILE *cpoint = fopen("customers.txt", "w");
    if (cpoint == NULL) {
        printf("Error opening customers.txt\n");
        return;
    }
    for (int i = 0; i < numcust; i++) {
        fprintf(cpoint, "%d\n%s\n%s\n%d\n%.2f\n",customers[i].C_ID, customers[i].name, customers[i].surname, customers[i].age, customers[i].wallet);
    }
    fclose(cpoint);

    // to store rented structure in rented txt
    FILE *rpoint = fopen("rented.txt", "w");
    if (rpoint == NULL) {
        printf("Error opening rented.txt\n");
        return;
    }
    for (int i = 0; i < *numrentals; i++) {
        fprintf(rpoint, "%d\n%d\n%d\n%s\n%d\n",
                rentals[i].R_ID, rentals[i].B_ID, rentals[i].C_ID, rentals[i].rantedDate, rentals[i].week);
    }
    fclose(rpoint);

    printf("operation ended succesfully\n");

}

void DeliveryBook(struct customer customers[], int numcust,struct book books[], int numbooks,struct rental rentals[], int *numrentals){
    int wek;
    int ID;
    printf("give after how many week, person bring the book\n");
    scanf("%d",&wek);

    //ID is the B_ID
    printf("give ID of the rented book \n");
    scanf("%d",&ID);
    int index= -1;

    // check the ID of book
    if (ID < 1 || ID > numbooks) {
        printf("Invalid book ID\n");
        return;
    }
    //checking book rented or not
    if(books[ID-1].rented == 1){

        // to found rental ID
        for(int i=0;i<*numrentals;i++){
            if( ID == rentals[i].B_ID){
                index = i;
            }
        }
         // Check if rental was found
        if (index == -1) {
            printf("Rental not found\n");
            return;
        }

        //if-else statements calculates the exchange.
        if(wek== rentals[index].week){
            books[ID-1].rented=0;

        }else {
            int dif = rentals[index].week - wek;
            int difference = dif * books[ID-1].priceperweek;
            customers[rentals[index].C_ID-1].wallet += difference;

            books[ID-1].rented=0;
        }

        // deleting from array
        for (int i = index; i < *numrentals - 1; i++) {
            rentals[i] = rentals[i+1];
        }
        (*numrentals)--;


    }else{
        printf("the book is not rented");
        return;
    }


    //updating book rented status
    FILE *bpoint = fopen("books.txt", "w");
    if (bpoint == NULL) {
        printf("Failed to open books.txt\n");
        return;
    }
    for(int i=0; i<numbooks;i++){
        fprintf(bpoint, "%d\n%s\n%s\n%d\n%.2f\n%d\n", books[i].B_ID, books[i].bookName, books[i].author, books[i].ageLimit, books[i].priceperweek, books[i].rented);
    }
    fclose(bpoint);

    //updating wallet values
    FILE *cpoint = fopen("customers.txt", "w");
    if (cpoint == NULL) {
        printf("Failed to open customers.txt\n");
        return;
    }
    for(int i =0; i< numcust;i++){
        fprintf(cpoint, "%d\n%s\n%s\n%d\n%.2f\n", customers[i].C_ID, customers[i].name, customers[i].surname, customers[i].age, customers[i].wallet);
    }
    fclose(cpoint);

    // to store rented structure in rented txt
    FILE *rpoint = fopen("rented.txt", "w");
    if (rpoint == NULL) {
        printf("Failed to open rented.txt\n");
        return;
    }
    for(int i =0; i<*numrentals;i++){
        fprintf(rpoint, "%d\n%d\n%d\n%s\n%d\n", rentals[i].R_ID, rentals[i].B_ID, rentals[i].C_ID, rentals[i].rantedDate, rentals[i].week);
    }
    fclose(rpoint);

    printf("operation ended succesfully \n");

}
void updatecustomer(struct customer customers[], int numcust){

    int ID;
    printf("enter the ID number\n");
    scanf("%d",&ID);
    if(ID > 0 && ID <= numcust){
    //updating name and surname
    printf("enter customer name\n");
    scanf("%49s", customers[ID-1].name);
    printf("enter customer surname\n");
    scanf("%49s", customers[ID-1].surname);

    // checking name and surname  because there cannot be 2 account have same name and surname
       for (int i = 0; i < numcust; i++) {
            if (i != (ID - 1) && strcmp(customers[i].name, customers[ID - 1].name) == 0 && strcmp(customers[i].surname, customers[ID - 1].surname) == 0) {
                printf("You cannot take this name and surname.\n");
                return;
            }
        }


    //updating age and wallet
    printf("enter age\n");
    scanf("%d", &customers[ID-1].age);
    printf("enter wallet\n");
    scanf("%f", &customers[ID-1].wallet);
    }else{
        printf("this ID doesn't exist\n");
        return;
    }
    // Updating customer information in the file
    FILE *cpoint = fopen("customers.txt", "w");
    if (cpoint == NULL) {
        printf("Error opening customers.txt\n");
        return;
    }
    for (int i = 0; i < numcust; i++) {
        fprintf(cpoint, "%d\n%s\n%s\n%d\n%.2f\n", customers[i].C_ID, customers[i].name, customers[i].surname, customers[i].age, customers[i].wallet);
    }
    fclose(cpoint);
    printf("Operation ended successfully\n");


}
void updatebook(struct book books[], int numbooks){
    int ID;
    printf("enter the ID number\n");
    scanf("%d",&ID);
    if(numbooks>=ID){
    //updating name and author
    printf("update book's name\n");
    scanf("%49s", books[ID-1].bookName);
    printf("update the author's name\n");
    scanf("%49s", books[ID-1].author);

    // checking author and bookname  because there cannot be 2 book have same name and author
    for (int i = 0; i <= numbooks; i++) {
        if (i != (ID - 1) && strcmp(books[i].bookName, books[ID-1].bookName) == 0 && strcmp(books[i].author, books[ID].author) == 0) {
            printf("this book already exist.\n");
            return;
        }
    }

    //updating age limitation and wallet
    printf("update age limit\n");
    scanf("%d", &books[ID-1].ageLimit);
    printf("update price per week\n");
    scanf("%f", &books[ID-1].priceperweek);



    //saving updated datas
    FILE *bpoint = fopen("books.txt", "w");
    for(int i=0; i<numbooks;i++){
    fprintf(bpoint, "%d\n%s\n%s\n%d\n%.2f\n%d\n", books[i].B_ID, books[i].bookName, books[i].author, books[i].ageLimit, books[i].priceperweek, books[i].rented);
    }
    fclose(bpoint);
    printf("operation ended succesfully\n");
    }else{
        printf("this ID doesn't exist\n");
    }

}



/*void BurnBook(struct book books[], int *numbooks) {
    int ID;
    printf("give ID number of the book\n");
    scanf("%d", &ID);

    // check ID
    if (ID < 1 || ID > *numbooks) {
        printf("Invalid book ID\n");
        return;
    }

    // check rental status
    if (books[ID - 1].rented == 0) {
        // remove the book from the array and shift the values to one left  which stands its right
        for (int i = ID - 1; i < *numbooks - 1; i++) {
            books[i] = books[i + 1];
             // I calculate every index in other methods with respect to ID number such as "scanf("%s", books[ID-1].bookName);".
            books[i].B_ID = i+1; // if I don't fix it, other methods will face problems.
        }
        (*numbooks)--;

        // saving books' informations
        FILE *bpoint = fopen("books.txt", "w");
        if (bpoint == NULL) {
            printf("Error opening file.\n");
            return;
        }
        for (int i = 0; i < *numbooks; i++) {
            fprintf(bpoint, " %d\n%s\n%s\n%d\n%.2f\n%d\n", books[i].B_ID, books[i].bookName, books[i].author, books[i].ageLimit, books[i].priceperweek, books[i].rented);
        }
        fclose(bpoint);

        printf("operation ended succesfully");
    } else {
        printf("The book is currently rented and cannot be removed.\n");
    }
*/

void listofcustomersR(struct customer customers[], int numcust,struct book books[], int numbooks,struct rental rentals[], int numrentals){

    //for loop to use every element of rentals array
    for(int i=0; i<numrentals ;i++){

        // ý make together name and surname
        char customername[100];
        strcpy(customername,customers[rentals[i].C_ID-1].name);
        strcat(customername," ");
        strcat(customername,customers[rentals[i].C_ID-1].surname);

        printf("%s  ", customername);
        printf("%d   %.2f \n",customers[i].age, customers[i].wallet );


    }
}
void listofcustomers(struct customer customers[], int numcust){

    for(int i=0; i<numcust ;i++){

        // to make together name and surname
        char customername[100];
        strcpy(customername,customers[i].name);
        strcat(customername," ");
        strcat(customername,customers[i].surname);
        printf("%d. %s ", i+1 ,customername);
        printf("%d   %.2f \n",customers[i].age, customers[i].wallet );
    }
}
void listofbooks(struct book books[], int numbooks){
    for(int i=0; i<numbooks ; i++){
        printf("%d. %s written by %s  ", i+1, books[i].bookName, books[i].author);
        printf("%d   %.2f   %d\n",books[i].ageLimit, books[i].priceperweek, books[i].rented );

    }
}




void load_customers(struct customer customers[], int *numcust) {
    FILE *cpoint = fopen("customers.txt", "r");
    if (cpoint == NULL) {

        return;
    }
    *numcust = 0;
    while (fscanf(cpoint, "%d", &customers[*numcust].C_ID) == 1) {
        fgetc(cpoint);  // to solve new line problem

        fgets(customers[*numcust].name, sizeof(customers[*numcust].name), cpoint);
        // to remove newline character
        customers[*numcust].name[strcspn(customers[*numcust].name, "\n")] = 0;

        //same thing with the name
        fgets(customers[*numcust].surname, sizeof(customers[*numcust].surname), cpoint);
        customers[*numcust].surname[strcspn(customers[*numcust].surname, "\n")] = 0;

        if (fscanf(cpoint, "%d", &customers[*numcust].age) != 1) break;
        if (fscanf(cpoint, "%f", &customers[*numcust].wallet) != 1) break;

        (*numcust)++;
    }
    fclose(cpoint);
}
void load_books(struct book books[], int *numbooks) {
    FILE *bpoint = fopen("books.txt", "r");
    if (bpoint == NULL) {

        return;
    }
    *numbooks = 0;
    while (fscanf(bpoint, "%d", &books[*numbooks].B_ID) == 1) {
        fgetc(bpoint);  // to solve new line problem

        fgets(books[*numbooks].bookName, sizeof(books[*numbooks].bookName), bpoint);
        // to remove newline character
        books[*numbooks].bookName[strcspn(books[*numbooks].bookName, "\n")] = 0;

        //same thing with the name
        fgets(books[*numbooks].author, sizeof(books[*numbooks].author), bpoint);
        books[*numbooks].author[strcspn(books[*numbooks].author, "\n")] = 0;

        if (fscanf(bpoint, "%d", &books[*numbooks].ageLimit) != 1) break;
        if (fscanf(bpoint, "%f", &books[*numbooks].priceperweek) != 1) break;
        if (fscanf(bpoint, "%d", &books[*numbooks].rented) != 1) break;

        (*numbooks)++;
    }
    fclose(bpoint);
}
void load_rentals(struct rental rentals[], int *numrentals) {
    FILE *cpoint = fopen("rented.txt", "r");
    if (cpoint == NULL) {
        printf("Error opening rented.txt\n");
        return;
    }

    *numrentals = 0;
    while (fscanf(cpoint, "%d\n%d\n%d\n",
                  &rentals[*numrentals].R_ID,
                  &rentals[*numrentals].B_ID,
                  &rentals[*numrentals].C_ID) == 3) {

        fgets(rentals[*numrentals].rantedDate, sizeof(rentals[*numrentals].rantedDate), cpoint);
        rentals[*numrentals].rantedDate[strcspn(rentals[*numrentals].rantedDate, "\n")] = 0;

        if (fscanf(cpoint, "%d\n", &rentals[*numrentals].week) != 1) break;

        (*numrentals)++;
    }

    fclose(cpoint);
}




int main() {




    //to use in operation I created array for every structure
    struct customer customers[MAX_CUSTOMERS];
    struct book books[MAX_BOOKS];
    struct rental rentals[MAX_RENTALS];


    //in the beggining there is no customer or book because of that I started value of them from 0. after operations they will change
    int numcust = 0, numbooks = 0, numrentals = 0;

    load_customers(customers, &numcust);
    load_books(books,&numbooks);
    load_rentals(rentals, &numrentals);


    // I made while loop to make operation as many times as user want
    while(true){
    //to give menu and to choose process
    printf("write the number which you want to be process \n1. Create New Customer \n2. Deposit Money to the Customer \n3. Add New Book \n4. Rent A Book \n5. Delivery A Book \n6. Update Customer Information \n7. Update Book Information \n8. List of Customers Who Rent a Book \n9. List of Customers \n10. List of Books \n11. exit \n");
    int a;
    scanf("%d",&a);

    // if else statements to do right operation (ý don't prefer use switch-case statements)
    if (a==1){
        AddCustomer(customers, &numcust);
    }else if(a==2){
        Deposit(customers, numcust);
    }else if(a==3){
        AddBook(books, &numbooks);
    }else if(a==4){
        rentBook(customers,numcust, books,&numbooks, rentals,&numrentals);
    }else if(a==5){
        DeliveryBook(customers,numcust, books,numbooks, rentals,&numrentals);
    }else if(a==6){
        updatecustomer(customers, numcust);
    }else if(a==7){
        updatebook(books, numbooks);
    }else if(a==8){
        listofcustomersR(customers,numcust, books,numbooks, rentals,numrentals);
    }else if(a==9){
        listofcustomers(customers,numcust);

    }else if(a==10){
        listofbooks(books, numbooks);

    }else if(a==11){
        break;
    }else{
        printf("invalid value");
    }
    }


    return 0;
}
