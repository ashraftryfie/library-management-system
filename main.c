/*******
** ITE Library - By: Ashraf Tryfie
********/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define RETURNTIME 1

FILE *flib, *fstd,*fb, *ft,*ftemp,*ftemp2;

char findbook;

struct Date {
    int day;
    int month;
    int year;
};

typedef struct {
    char Name[30];
} Author;

typedef struct {
    char category[50];
    int ISBN;
    char Title[20];
    char Section [15];
    int NumCopies;
    int NumAuthors;
    int nummost;
    Author a[10];
    int status; // 1: Book is Available - 0: not Available
} Library;

typedef struct {
    char Name[20];
    int ID;
    int C_SB;
} Students;

typedef struct {
    Students sb;
    Library L;
    struct Date borrow_date;
    struct Date borrow_return;
    struct Date date_punished;
    int status;
} Borrow;

int file_is_ok(FILE *filename);
void Library_info();
int Check_ID(int t,char choice);
int Check_Name(char name[],char choice);
void Add_Books();
void Display_Books();
void Display_Borrow();
void Return_Book(void);
void File_students();
void Display_Students();
void File_borowing();
int DisplayByAuthor(char au[]);
int Book_WithStudent(char s[]);
void Delete_Books();
void Search_books();
void notreturn(void);
void Sort_BBook(Borrow b);
void mainmenu();
void Delete_BBook(char String[]);
void Clear_Screen();
void Common_BBooks();
void issmost(void);

COORD coord = {0,0};

int gotoxy(int x,int y)
{
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

int main ()
{
    system("color 9");
    mainmenu();
    return 0;
}

//-------------------------------------------- ADD Book to Library --------------------------------------------
void Add_Books()
{
    int line2,i,k,conlin;
    Library l;

    flib=fopen("Books.bin","ab+");

    if(file_is_ok(flib)) {

        line2:

        fseek(flib,0,SEEK_END); //keep the file pointer at the end of file
        char an='y';
        while(an=='y' || an=='Y')
        {
            gotoxy(50,3);
            printf("Enter ISBN Book: ");
            scanf("%d",&l.ISBN);
            if(Check_ID(l.ISBN,'1')==1)
            {
                //system("cls");
                gotoxy(50,5);
                printf("Sorry!! The book is existed in the database\n");
                gotoxy(50,7);
                printf("1 To try again");
                gotoxy(50,9);
                printf("0 to back to the main menu");
                gotoxy(50,11);
                printf("2 to add ");
                fflush(stdin);
                if(getch()=='1') {
                    printf("\n");
                    goto line2;
                }
                else if(getch()=='2') {
                    system("cls");
                    goto conlin;
                }
                else {
                    fclose(flib);
                    mainmenu();
                }
            }

            conlin:

            fseek(flib, 0, SEEK_END);
            gotoxy(50,5);
            fflush(stdin);
            printf("Enter Title book: ");
            gets(l.Title);
            gotoxy(50,7);
            printf("Enter the Section: ");
            scanf("%s", &l.Section);
            do {
                gotoxy(50,9);
                printf("Enter the Number of Copies: ");
                scanf("%d", &l.NumCopies);
            } while(l.NumCopies <= 0);
            gotoxy(50,11);
            printf("Enter the No.Authors: ");
            scanf("%d", &l.NumAuthors);
            int j = 13;
            for(i = 0; i < l.NumAuthors; i++)
            {
                gotoxy(50,j);
                printf("Enter the Author Name: ");
                scanf("%s", &l.a[i].Name);
                j = j + 2;
            }
            l.nummost = 0;
            l.status = 1;
            if(l.NumCopies == 0)
                l.status=0;
            fwrite(&l, sizeof(Library), 1, flib);
            gotoxy(50,j);
            printf("ADD ANOTHER RECORD (y/n)");
            fflush(stdin);//clearing the input buffer
            an = getche();
            system("cls");
        }//end of while
        fclose(flib);
    }
    fclose(flib);
}

//-------------------------------------------- File Borrowing --------------------------------------------
void File_borowing() {
    Borrow b;
    Library L;
    Students s;
    int ID_line,j;
    char student[20];
    SYSTEMTIME time;
    GetSystemTime(&time);

    system("cls");
    gotoxy(0,3);
    printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Borrow Section \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
    gotoxy(0,5);
    printf("\xDB\xDB\xDb\xB2 1. Borrow a Books");
    gotoxy(0,7);
    printf("\xDB\xDB\xDb\xB2 2. View borrowed Books");
    gotoxy(0,9);
    printf("\xDB\xDB\xDb\xB2 3. Book with Student(not return)");
    gotoxy(0,11);
    printf("\xDB\xDB\xDb\xB2 4. Display Borrow_Books Sort by Month/Year");
    gotoxy(0,13);
    printf("\xDB\xDB\xDb\xB2 5. Display The Students that are borrowing");
    gotoxy(0,15);
    printf("\xDB\xDB\xDb\xB2 7. Display borrowing not return");
    gotoxy(0,15);
    printf("\xDB\xDB\xDb\xB2 6. Back to the Main Menu");
    gotoxy(0,17);
    printf("Enter your choice: ");
    fflush(stdin);

    switch(getch()) {
    case '1':
    {
        int sss = 0,ss = 0,line2, lin3, line4, l, k, C_borrowing_student; //l for give choice to user ...line3 for don't repeat the Statement : ****The file Borrowing***
        char ah, find = 't';
        Borrow b;
        Students s;
        fb = fopen("Borrow.bin", "ab+");
        flib = fopen("Books.bin", "rb");
        fstd = fopen("Students.bin", "rb");
        if(file_is_ok(fb))
            if(flib != NULL) {
                gotoxy(50,1);
                printf("********************* The File Borrowing ********************\n");
                gotoxy(50,2);
                printf("--------------------------------------------------------------\n");
                ah = 'y';
                while(ah == 'y')
                {
                    ID_line:

                    gotoxy(50,3);
                    printf("Enter Student ID: ");
                    scanf("%d", &b.sb.ID);
                    if(Check_ID(b.sb.ID,'2') == 0)
                    {
                        gotoxy(50,3);
                        printf("The student is not existed in file students..\n");
                        gotoxy(50,5);
                        printf("to Add press 1 .. to Try another ID press 2 :  ");
                        fflush(stdin);
                        switch(getch()) {
                            case '1':
                                fclose(fb);
                                gotoxy(50,7);
                                printf("******************** Add a Student ********************\n");
                                File_students();
                                gotoxy(50,15);
                                printf("Add Student is done...\nNow Add the Borrow Information....\n\n");
                                system("cls");
                                File_borowing();
                                break;

                            case '2':
                                system("cls");
                                gotoxy(50,1);
                                printf("******************** The File Borrowing ********************\n");
                                gotoxy(50,2);
                                printf("---------------Try------------------------Again-------------\n");
                                goto ID_line;
                                break;

                            default:
                                gotoxy(50,7);
                                printf("Check your choice And try again..!!");
                                Clear_Screen();
                                goto ID_line;
                                break;
                        }
                    }

                    gotoxy(50,5);
                    printf("Enter Student Name: ");
                    scanf("%s", &b.sb.Name);
                    C_borrowing_student = Check_Name(b.sb.Name,'3');
                    gotoxy(50,9);
                    printf("This Student borrow %d once\n", C_borrowing_student);
                    if(Check_ID(b.sb.ID,'2') == 1) {
                        if(Check_Name(b.sb.Name,'2') == 1)
                            goto line2;
                        else {
                            gotoxy(50,7);
                            printf("Name and ID isn't matched..!");
                            gotoxy(50,9);
                            printf("1.Try again");
                            gotoxy(50,11);
                            printf("2. to show the list of student");
                            gotoxy(50,13);
                            printf("0. to back to the main menu");
                            gotoxy(50,15);
                            printf("Enter your Choice: ");
                            if(getch()=='1')
                                File_borowing();
                            else if(getch()=='2') {
                                Display_Students();
                                Clear_Screen();
                            }
                            else {
                                fclose(flib);
                                mainmenu();
                            }
                        }
                    }

                    line2:

                    gotoxy(50,11);
                    Library l;
                    printf("Enter Title of book that is borrowed: ");
                    scanf("%s", &b.L.Title);
                    gotoxy(50,13);
                    printf("Enter ISBN of book that is borrowed: ");
                    scanf("%d", &sss);
                    b.L.ISBN = sss;
                    if(Check_Name(b.L.Title,'1')==0) {

                        line4:

                        gotoxy(52,15);
                        printf("Sorry!! The book isn't existed in the database\n");
                        gotoxy(52,17);
                        printf("To try again press 1..0 to back to the main menu: ");
                        fflush(stdin);
                        if(getch() == '1') {
                            printf("\n");
                            fclose(fb);
                            File_borowing();
                        }
                        else {
                            system("cls");
                            fclose(fb);
                            mainmenu();
                        }

                    }
                    else {
                        while(fread(&L, sizeof(Library), 1, flib)) {
                            gotoxy(50,13);
                            if(L.status == 1) {
                                printf("This Book is Available..");
                                gotoxy(50,15);
                                printf("There are %d unBorrowed books in library ",L.NumCopies);
                            }
                            else
                                goto line4;
                        }
                        gotoxy(50,15);
                        printf("The date of borrowing: ");
                        printf("%d/%d/%d", time.wDay, time.wMonth, time.wYear);
                        b.borrow_date.day = time.wDay;
                        b.borrow_date.month = time.wMonth;
                        b.borrow_date.year = time.wYear;
                        gotoxy(50,17);
                        printf("The Date of return: ");
                        b.borrow_return.day = b.borrow_date.day;   //for return date
                        b.borrow_return.month = b.borrow_date.month+RETURNTIME;
                        b.borrow_return.year = b.borrow_date.year;
                        if(b.borrow_return.month > 12)
                        {
                            b.borrow_return.year += 1;
                            b.borrow_return.month -= 12;
                        }
                        printf("%d/%d/%d", b.borrow_return.day, b.borrow_return.month, b.borrow_return.year);
                        //for day start punish
                        b.date_punished.day = b.borrow_return.day + 1;
                        b.date_punished.month = b.borrow_return.month;
                        b.date_punished.year = b.borrow_return.year;
                        //start period of borrowing
                        b.status = 1;
                        rewind(flib);
                        ftemp2 = fopen("Temp2.bin", "wb");
                        while((fread(&L, sizeof(L), 1, flib))) {
                            if(sss == L.ISBN) {
                                ss = L.nummost+1;
                                L.nummost = ss;
                                ss = L.NumCopies - 1;
                                L.NumCopies = ss;
                                fwrite(&L, sizeof(L), 1, ftemp2);
                            }
                            else
                                fwrite(&L, sizeof(L), 1, ftemp2);
                        }
                        fwrite(&b, sizeof(Borrow), 1, fb);
                        fclose(flib);
                        fclose(ftemp2);
                        remove("Books.bin");
                        rename("Temp2.bin", "Books.bin");
                        fflush(stdin);
                        gotoxy(50,19);
                        printf("Press (y) to continue:");
                        ah = getchar();
                        system("cls");
                        //minise th copies
                    }

                }
                fclose(fb);
                fclose(flib);
                fclose(fstd);
                system("cls");
            }
            else
                Display_Books();
        fclose(fb);
        fclose(flib);
        fclose(fstd);
        mainmenu();
    }
    break;

    case '2' :
        Display_Borrow();
        break;

    case '3' :
        gotoxy(0,19);
        printf("Enter the name of Student to Check: ");
        scanf("%s", &student);
        j = Book_WithStudent(student);
        if (j == 0)
        {
            system("cls");
            gotoxy(50,3);
            printf("This Student is not existed..\n\n");
        }
        break;

    case '4' :
        Sort_BBook(b);
        break;

    case '5' :
        Display_Students();
        break;

    case '6' :
        mainmenu();
        break;

    case '7' :
        notreturn();
        break;

    default:
        system("cls");
        gotoxy(10,18);
        printf("\aWrong Entry!!");
        Clear_Screen();
        File_borowing();
        break;
    }
}

void Display_Borrow() {
    SYSTEMTIME time;
    GetSystemTime(&time);
    Borrow b;
    int j = 4;
    fb = fopen("Borrow.bin", "rb");
    if(file_is_ok(fb)) {
        system("cls");
        printf("\t\t\t\t\t\t------------------------------------------------------------------------");
        printf("\t\t\t\t\t\t\t ID |  STUDENT NAME |  BOOK NAME |  BORROW DATE |  RETURN DATE | STATUS\n");
        printf("\t\t\t\t\t\t------------------------------------------------------------------------\n\t\t\t\t\t");
        while(fread(&b, sizeof(Borrow), 1, fb)) {
            gotoxy(49, j);
            printf("%d", b.sb.ID);
            gotoxy(55, j);
            printf("%s", b.sb.Name);
            gotoxy(73, j);
            printf("%s", b.L.Title);
            gotoxy(85, j);
            printf("%d/%d/%d", b.borrow_date.day, b.borrow_date.month, b.borrow_date.year);
            gotoxy(100, j);
            printf("%d/%d/%d", b.borrow_return.day, b.borrow_return.month, b.borrow_return.year);
            if(time.wMonth == b.borrow_return.month)
                if(time.wDay > b.borrow_return.day) {
                    b.status = 0;
                    //for free from punish
                    b.date_punished.day = b.borrow_return.day + 1;
                    b.date_punished.month = b.borrow_return.month;
                    b.date_punished.year = b.borrow_return.year;
                }
            if(time.wMonth > b.borrow_return.month)
                if(time.wDay > b.borrow_return.day)
                {
                    b.status = 0;
                    b.date_punished.day = b.borrow_return.day + 1;
                    b.date_punished.month = b.borrow_return.month;
                    b.date_punished.year = b.borrow_return.year;
                }
            if(time.wMonth < b.borrow_return.month)
            {
                b.status = 1;
            }

            //conditions for free from punish
            if(time.wMonth > b.date_punished.month)
                if(time.wDay == b.date_punished.day)
                    b.status = 1;
                else if(time.wMonth == b.date_punished.month) {
                    if(time.wDay - b.date_punished.day == 30)
                        b.status = 1;
                }
                else
                    b.status=0;
            gotoxy(115, j);
            printf("%d", b.status);
            printf("\n\t\t\t\t\t\t-----------------------------------------------------------------------\n");
            j = j + 3;
        }
    }
    fclose(fb);
    Clear_Screen();
}

//-------------------------------------------- Return Borrow Book --------------------------------------------
void Return_Book(void) {
    int id,isbn;
    Borrow b;
    Library l;
    char find = 'a';
    fb = fopen("Borrow.bin", "rb");
    flib = fopen("Books.bin", "rb");
    ftemp = fopen("Temp1.bin", "wb");
    gotoxy(50,5);
    printf("Enter ISBN Book:");
    scanf("%d", &isbn);
    while (fread(&l, sizeof(Library), 1, flib)) {
        if(isbn == l.ISBN) {
            gotoxy(50, 7);
            printf("Enter ID: ");
            scanf("%d", &id);
            while (fread(&b, sizeof(Borrow), 1, fb)) {
                if(id == b.sb.ID)
                    find = 't';
            }
        }
    }
    if (find == 't') {
        rewind(fb);
        while (fread(&b, sizeof(Borrow), 1, fb))
            if((isbn != b.L.ISBN) && (id != b.sb.ID)) {
                fwrite(&b, sizeof(Borrow), 1, ftemp);
            }
        rewind(flib);
        int s;
        ftemp2 = fopen("Temp2.bin", "wb");
        while(fread(&l, sizeof(Library), 1, flib)) {
            if(isbn == l.ISBN) {
                s = l.NumCopies + 1;
                l.NumCopies = s;
                fwrite(&l, sizeof(Library), 1, ftemp2);
            }
            else
                fwrite(&l, sizeof(Library), 1, ftemp2);
        }
        fclose(flib);
        fclose(ftemp2);
        remove("Books.bin");
        rename("Temp2.bin", "Books.bin");
    }
    else {
        gotoxy(50,7);
        printf("The  book mo exist");
    }
    fclose(flib);
    fclose(ftemp);
    fclose(ftemp2);
    fclose(fb);

    remove("Borrow.bin");
    rename("Temp1.bin", "Borrow.bin");
}

//-------------------------------------------- Search for Book --------------------------------------------
void Search_books() {
    int i,d;
    Library l;
    gotoxy(50,1);
    printf("*********************** Search Books ***********************");
    gotoxy(50,3);
    printf(" 1. Search By ID");
    gotoxy(50,4);
    printf(" 2. Search By Name");
    gotoxy(51,6);
    printf("Enter Your Choice: ");
    flib = fopen("Books.bin", "rb");
    rewind(flib);
    switch(getch()) {
    case '1':
    {
        int k = 6;
        int j = 6;
        gotoxy(50,7);
        printf("****Search Books By Id****");
        gotoxy(50,9);
        printf("Enter the book id:");
        scanf("%d", &d);
        gotoxy(50,10);
        printf("Searching........\n\n");
        while(fread(&l, sizeof(Library), 1, flib)) {
            if(l.ISBN == d) {
                system("cls");
                printf("\t\t\t\t\tThe Book is available");
                printf("\n\t\t\t\t\t*********************************Library Books*****************************\n");
                printf("\n\t\t\t\t\t ID    BOOK NAME      SECTION       NO.COPIES     AUTHOR NAME     STATUS\n");
                printf("\t\t\t\t\t------------------------------------------------------------------------\n\n\t\t");
                gotoxy(41,j);
                printf("%d", l.ISBN);
                gotoxy(48,j);
                printf("%s", l.Title);
                gotoxy(63,j);
                printf("%s", l.Section);
                gotoxy(79,j);
                printf("%d", l.NumCopies);

                for(i = 0; i < l.NumAuthors; i++) {
                    gotoxy(91, k);
                    printf("%s", l.a[i].Name);
                    k = k+2;
                }
                l.status = 1;
                if(l.NumCopies == 0)
                    l.status = 0;
                gotoxy(108, j);
                printf("%d", l.status);
                i = i + l.NumCopies;
                j = k;
                gotoxy(41, k-1);
                printf("------------------------------------------------------------------------");
                findbook = 't';
            }

        }
        if(findbook != 't') { //checks whether condition enters inside loop or not
            printf("\n\t\t\t\t\aNo Record Found");
        }
        printf("\n\t\t\t\tTry another search?(Y/N)");
        if(getch() == 'y') {
            system("cls");
            Search_books();
        }
        else {
            fclose(flib);
            mainmenu();
        }
        fclose(flib);
        break;
    }
    case '2':
    {
        int k = 6;
        int j = 6;
        char s[15];
        gotoxy(50,7);
        printf("****Search Books By Name****");
        gotoxy(50,9);
        printf("Enter Book Name:");
        scanf("%s", s);
        int d = 0;
        gotoxy(50, 10);
        printf("Searching........\n\n");
        rewind(flib);
        while(fread(&l, sizeof(Library), 1, flib)) {
            if(strcmp(l.Title,s) == 0) { //checks whether a.name is equal to s or not
                system("cls");
                printf("\n\t\t\t\t\t*********************************Library Books*****************************\n");
                printf("\n\t\t\t\t\t ID    BOOK NAME      SECTION       NO.COPIES     AUTHOR NAME     STATUS\n");
                printf("\t\t\t\t\t------------------------------------------------------------------------\n\n\t\t");
                gotoxy(41,j);
                printf("%d", l.ISBN);
                gotoxy(48,j);
                printf("%s", l.Title);
                gotoxy(63,j);
                printf("%s", l.Section);
                gotoxy(79,j);
                printf("%d", l.NumCopies);

                for(i = 0; i < l.NumAuthors; i++) {
                    gotoxy(91,k);
                    printf("%s", l.a[i].Name);
                    k = k + 2;
                }
                l.status = 1;
                if(l.NumCopies == 0)
                    l.status = 0;
                gotoxy(108, j);
                printf("%d", l.status);
                i = i + l.NumCopies;
                j = k;
                gotoxy(41, k-1);
                printf("------------------------------------------------------------------------");
                d++;
            }
        }
        if(d == 0) {
            printf("\n\t\t\t\t\aNo Record Found");
        }
        printf("\n\t\t\t\tTry another search?(Y/N)");
        if(getch() == 'y')
            Search_books();
        else {
            fclose(flib);
            mainmenu();
        }
        break;
    }
    default:
        Search_books();
    }
    fclose(flib);
}

//-------------------------------------------- Display Books In Library --------------------------------------------
void Display_Books()
{
    rewind(flib); //keeps the file pointer at the beginning of file
    int total=0,i;
    int k=6;
    int j=6;
    Library l;
    flib=fopen("Books.bin","rb");
    if(flib!=NULL)
    {
        system("cls");
        printf("\n\t\t\t\t\t*********************************Library Books*****************************\n");
        printf("\t\t\t\t\t ---------------------------------------------------------------------------\n");
        printf("\t\t\t\t\t| ISBN |  BOOK NAME   |  SECTION   |   NO.COPIES  |  AUTHOR NAME  |  STATUS |\n");
        printf("\t\t\t\t\t ---------------------------------------------------------------------------\n\n\t\t");
        while(fread(&l,sizeof(Library),1,flib)) //reads the data from the file
        {
            gotoxy(43,j);
            printf("%d",l.ISBN);
            gotoxy(51,j);
            printf("%s",l.Title);
            gotoxy(66,j);
            printf("%s",l.Section);
            gotoxy(83,j);
            printf("%d",l.NumCopies);

            for(i=0; i<l.NumAuthors; i++)
            {
                gotoxy(95,k);
                printf("%s",l.a[i].Name);
                k=k+2;
            }
            l.status=1;
            if(l.NumCopies==0)
                l.status=0;
            gotoxy(111,j);
            printf("%d",l.status);
            total=total+l.NumCopies;
            j=k;
            gotoxy(41,k-1);
            printf("---------------------------------------------------------------------------");
        }
    }
    else
    {
        gotoxy(42,16);
        printf("The Library is Empty Now..");
    }
    gotoxy(42,18);
    printf("Total Books in Library =%d",total);
    fclose(flib);
}

//--------------------------------------------Display Books By Author--------------------------------------------
int DisplayByAuthor(char au[])
{
    int i,j=5,m,find=0;
    Library l;
    int k=5;
    flib=fopen("Books.bin","rb");
    if(file_is_ok(flib))
    {
        system("cls");
        printf("\n*********************************Library Books*****************************\n");
        printf("\n ID    BOOK NAME      SECTION       NO.COPIES     AUTHOR NAME     STATUS\n");
        printf("------------------------------------------------------------------------\n");
        while(fread(&l,sizeof(Library),1,flib)) //reads the data from the file
        {
            for(m=0; m<l.NumAuthors; m++)
                if(strcmp(au,l.a[m].Name)==0)
                {
                    find=1;
                    printf("%2d",l.ISBN);
                    printf("%9s",l.Title);
                    printf("%16s\t\t",l.Section);
                    printf("%d",l.NumCopies);

                    for(i=0; i<l.NumAuthors; i++)
                    {
                        gotoxy(51,k);
                        puts(l.a[i].Name);
                        k=k+2;
                    }
                    l.status=1;
                    if(l.NumCopies==0)
                        l.status=0;
                    gotoxy(68,j);
                    printf("%d",l.status);
                    i=i+l.NumCopies;
                    j=k+2;
                    gotoxy(0,k-1);
                    printf("------------------------------------------------------------------------\n");
                }
        }
    }
    fclose(flib);
    return find;
}

//--------------------------------------------Sort Borrow Book--------------------------------------------
void Sort_BBook(Borrow b)
{
    printf("\t\t*****************************Sort Books*********************************");
    int m,y; //month , year
    int f=0;    //if he found the month or year take value (1)
    int c=0; //count
    int s; //for choice
    char mo[20]; //store the name of month
    system("cls");
    fb=fopen("Borrow.bin","rb");
    if(file_is_ok(fb))
    {
        rewind(fb);
        printf("\nIf you want to sort by year press 1 :: by month press 0 :   ");
        scanf("%d",&s);

        if(s==0)
        {
            printf("\nEnter the month in which book was borrowed:  ");
            scanf("%d",&m);

            printf("\nThe books are: ");
            while(fread(&b,sizeof(Borrow),1,fb))
            {
                if(m==b.borrow_date.month)
                {
                    f=1;
                    printf("\n\t* ID_Student: %d",b.sb.ID);
                    printf("\n\t* Name_Student: %s",b.sb.Name);
                    printf("\n\t* Book: %s\n",b.L.Title);
                    c++;
                }
            }

            switch(m)
            {
            case 1:
                strcpy(mo,"January");
                break;
            case 2:
                strcpy(mo,"February");
                break;
            case 3:
                strcpy(mo,"March");
                break;
            case 5:
                strcpy(mo,"May");
                break;
            case 6:
                strcpy(mo,"June");
                break;
            case 7:
                strcpy(mo,"July");
                break;
            case 8:
                strcpy(mo,"August");
                break;
            case 9:
                strcpy(mo,"September");
                break;
            case 10:
                strcpy(mo,"October");
                break;
            case 11:
                strcpy(mo,"November");
                break;
            case 12:
                strcpy(mo,"December");
                break;
            case 4:
                strcpy(mo,"April");
                break;
            default:
                printf("\nEnter the valid month");
            }

            if(f==0)
                printf("\n\n\t=> No books borrowed in the month %s are %d\n",mo,c);
            else
                printf("\n\n\t=> Total number of books borrowed in the month %s is %d\n",mo,c);
            fclose(fb);
            Clear_Screen();
        }

        else
        {
            printf("\nEnter the year in which book was Borrowed: ");
            scanf("%d",&y);

            printf("\nThe books are: ");
            while((fread(&b,sizeof(Borrow),1,fb)))
            {
                if(y==b.borrow_date.year)
                {
                    f=1;
                    printf("\n\t* ID_Student: %d",b.sb.ID);
                    printf("\n\t* Name_Student: %s",b.sb.Name);
                    printf("\n\t* Book: %s\n",b.L.Title);
                    c++;
                }
            }

            if(f==0)
                printf("\n\t=>No books issued in the year of %d",y);
            else
                printf("\n\t=>Total number of books issued in the year of %d is %d",y,c);
            fclose(fb);
            Clear_Screen();
        }
    }
    fclose(fb);
}

//--------------------------------------Display The Book That Still with Student----------------------------------------
int Book_WithStudent(char s[])
{
    int i=0;
    int k=6;
    int j=4;
    int find=0;
    Library l;
    Borrow b;
    SYSTEMTIME time;
    GetSystemTime(&time);
    fb=fopen("Borrow.bin","rb");
    if(file_is_ok(fb))
    {
        rewind(fb);
        while(fread(&b,sizeof(Borrow),1,fb))
        {
            if(strcmp(s,b.sb.Name)==0)
            {
                find=1;
                if(time.wMonth==b.borrow_return.month)
                    if(time.wDay>b.borrow_return.day)
                        b.status=0;
                if(time.wMonth>b.borrow_return.month)
                    if(time.wDay!=b.borrow_return.day)
                        b.status=0;
                if(time.wMonth>b.borrow_return.month)
                    b.status=1;
                if(b.status==1)
                {
                    gotoxy(50,18);
                    printf("this student haven't any problem!!");
                    Clear_Screen();
                }
                else
                {
                    gotoxy(50,0);
                    printf("**************The File Book with Student that are late**************\n");
                    gotoxy(49,2);
                    printf(" ID    STUDENT NAME    BOOK NAME    BORROW DATE    RETURN DATE   STATUS\n");
                    gotoxy(49,3);
                    printf("-----------------------------------------------------------------------\n\t\t\t\t\t");
                    gotoxy(50,j);
                    printf("%d",b.sb.ID);
                    gotoxy(55,j);
                    printf("%s",b.sb.Name);
                    gotoxy(73,j);
                    printf("%s",b.L.Title);
                    gotoxy(85,j);
                    printf("%d/%d/%d",b.borrow_date.day,b.borrow_date.month,b.borrow_date.year);
                    gotoxy(100,j);
                    printf("%d/%d/%d",b.borrow_return.day,b.borrow_return.month,b.borrow_return.year);
                    gotoxy(117,j);
                    printf("%d",b.status);
                    j=j+2;
                }
            }
        }
    }
    fclose(flib);
    system("cls");
    return find;
}

//--------------------------------------------File Student--------------------------------------------
void File_students()
{
    int line3,line5;
    char c;
    Students std;
    fstd=fopen("Students.bin","ab+");
    if(file_is_ok(fstd))
    {
        fseek(fstd,0,SEEK_END);
//------------Input the Student ID---------------
        gotoxy(50,9);
        printf("Enter the ID-student: ");
        scanf("%d",&std.ID);

        if(Check_ID(std.ID,'2')==1)
        {
            //system("cls");
            gotoxy(50,11);
            printf("Sorry!! The student is existed in the database\n");
            gotoxy(50,13);
            printf("1.to show Student list");
            gotoxy(50,15);
            printf("2.Try or Back to the main menu");
            gotoxy(50,17);
            printf("Enter your choice: ");
            fflush(stdin);
            switch(c=getchar())
            {
            case '1':
                Display_Students();
                goto line3;
                break;

            case '2':
line3:
                gotoxy(50,5);
                printf("To try again press 1..0 to back to the main menu: ");
                fflush(stdin);
                if(getch()=='1')
                {
                    printf("\n");
                    fclose(fstd);
                    File_students();
                }
                else
                {
                    fclose(fstd);
                    mainmenu();
                }
                break;
            }
        }

//------------Input the Student Name---------------

        gotoxy(50,11);
        printf("Enter the Name-student: ");
        fflush(stdin);
        gets(std.Name);
        if(Check_Name(std.Name,'2')==1)
        {
            gotoxy(50,11);
            printf("Sorry!! The student is existed in the database\n");
            gotoxy(50,13);
            printf("1.to show Student list");
            gotoxy(50,15);
            printf("2.Try or Back to the main menu");
            gotoxy(50,17);
            printf("Enter your choice: ");
            fflush(stdin);
            switch(c=getchar())
            {
            case '1':
                Display_Students();
                goto line5;
                break;

            case '2':
line5:
                gotoxy(50,5);
                printf("To try again press 1..0 to back to the main menu: ");
                fflush(stdin);
                if(getch()=='1')
                {
                    printf("\n");
                    File_students();
                }
                else
                {
                    fclose(fstd);
                    mainmenu();
                }
                break;
            }
        }
        fb=fopen("Borrow.bin","rb");
        if (file_is_ok(fb))
            std.C_SB=Check_Name(std.Name,'3');
        fwrite(&std,sizeof(Students),1,fstd);
        fclose(fb);
    }
    fclose(fstd);
    Clear_Screen();
}

void mainmenu()
{
    system("cls");
    char curr_date[20];
    strcpy(curr_date,__DATE__);
    printf("\nThe Date: %14s\n",&curr_date);

    int fun;
    char title[20],String[20];
    int choice;
    while(1)
    {
        gotoxy(0,3);
        printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 MAIN MENU \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
        gotoxy(0,5);
        printf("\xDB\xDB\xDB\xDB\xB2 1. Library Book Section");
        gotoxy(0,7);
        printf("\xDB\xDB\xDB\xDB\xB2 2. Borrow Section");
        gotoxy(0,9);
        printf("\xDB\xDB\xDB\xDB\xB2 3. Search for Book");
        gotoxy(0,11);
        printf("\xDB\xDB\xDB\xDB\xB2 4. Return Book");
        gotoxy(0,13);
        printf("\xDB\xDB\xDB\xDB\xB2 5. The Most Common Books");
        gotoxy(0,15);
        printf("\xDB\xDB\xDB\xDB\xB2 0. Close Application");
        gotoxy(0,17);
        printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
        gotoxy(0,19);
        printf("Enter your choice: ");
        fflush(stdin);
        switch(getch())
        {
        case '1' :
            Library_info();
            break;

        case '2' :
            File_borowing();
            break;

        case '3' :
            Search_books();
            break;

        case '4' :
            Return_Book();
            break;

        case '5' :
            issmost();
            Clear_Screen();
            break;

        case '0' :
            system("cls");
            gotoxy(16,3);
            printf("Programmer....\n");
            gotoxy(16,5);
            printf("****** Ashraf Trifi ******\n");
            gotoxy(16,7);
            printf("Exiting in seconds...........\n\n\n");
            exit(EXIT_SUCCESS);
            break;

        default:
        {
            gotoxy(10,23);
            printf("\aWrong Entry!!Please re-entered correct option");
            if(getch())
                mainmenu();
        }
        }
    }
}

int file_is_ok (FILE *filename)
{
    if(filename==NULL)
    {
        perror("Error while opening the file...!");
        exit(EXIT_FAILURE);
    }
}

void Library_info()
{
    int j;
    char author[20],st[20];
    system("cls");
    gotoxy(0,3);
    printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Books Section \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
    gotoxy(0,5);
    printf("\xDB\xDB\xDB\xDB\xB2 1. Add Book");
    gotoxy(0,7);
    printf("\xDB\xDB\xDB\xDB\xB2 2. Display Books in Library");
    gotoxy(0,9);
    printf("\xDB\xDB\xDB\xDB\xB2 3. Display by a specific Author");
    gotoxy(0,11);
    printf("\xDB\xDB\xDB\xDB\xB2 4. Delete a Book");
    gotoxy(0,13);
    printf("\xDB\xDB\xDB\xDB\xB2 5. Back To the Main Menu");
    gotoxy(0,15);
    printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
    gotoxy(0,17);
    printf("Enter your choice: ");
    fflush(stdin);
    switch(getch())
    {
    case '1' :
        Add_Books();
        break;

    case '2' :
        Display_Books();
        Clear_Screen();
        break;

    case '3' :
        gotoxy(50,1);
        printf("Enter the name of author:");
        scanf("%s",&author);
        j=DisplayByAuthor(author);
        if(j==0)
        {
            system("cls");
            gotoxy(40,5);
            printf("This Author Not Found..!!");
        }
        Clear_Screen();
        break;

    case '4' :
        Delete_Books();
        break;

    case '5' :
        mainmenu();
        break;

    default:
        Library_info();
    }
}

//--------------------------------------------Check By ID--------------------------------------------
int Check_ID(int t,char choice)
{
    Library l;
    Students s;
    switch(choice)
    {
    case '1':
        rewind(flib);
        while(fread(&l,sizeof(l),1,flib))
            if(l.ISBN==t)
                return 1;  //returns 0 if book exits
        return 0; //return 1 if it not
        break;

    case '2':
        rewind(fstd);
        while(fread(&s,sizeof(Students),1,fstd))
            if(s.ID==t)
                return 1;  //returns 0 if id-student exits
        return 0; //return 1 if it not
        break;
    }
}

//--------------------------------------------Check By Name--------------------------------------------
int Check_Name(char name[],char choice)
{
    int i=0;
    Library l;
    Students s;
    Borrow b;
    switch(choice)
    {
    case '1':
        rewind(flib);
        while(fread(&l,sizeof(Library),1,flib))
            if(strcmp(l.Title, name)==0)
                i++;//returns 1 if book exits
        return i; //return 0 if it not
        break;

    case '2':
        rewind(fstd);
        while(fread(&s,sizeof(Students),1,fstd))
            if(strcmp(s.Name,name)==0)
                i++;  //returns 1 if student exits
        return i; //return 0 if it not
        break;

    case '3' :
        rewind(fb);
        while(fread(&b,sizeof(Borrow),1,fb))
            if(strcmp(b.sb.Name,name)==0)
                i++; //returns 1 if student exits
        if(i>=3)
        {
            gotoxy(50,7);
            printf("This student borrow 3 Books..he must return 1 to borrow another");
            gotoxy(52,9);
            printf("=>Press 1 to back to the main menu");
            if(getch()=='1')
                mainmenu();
        }
        return i;
        break;
    }
}

void Display_Students()
{
    Students std;
    int i=0,j=0;
    fstd=fopen("Students.bin","rb");
    if(fstd!=NULL)
    {
        rewind(fstd);
        system("cls");
        gotoxy(50,1);
        printf("****************** The File Student ******************\n");
        gotoxy(65,2);
        printf("ID  |  Student Name  \n");
        gotoxy(65,3);
        printf("---------------------\n");
        while(fread(&std,sizeof(Students),1,fstd))
        {
            printf("\t\t\t\t\t\t\t\t%2d\t",std.ID);
            printf("%s\t\n",std.Name);
            i++;
        }
        j=i;
        gotoxy(40,j+5);
        printf("Total Number of Students that borrowing from library is %d",i);
        fclose(fstd);
        Clear_Screen();
    }
    else
    {
        j=i;
        gotoxy(50,j+5);
        printf("There are %d Students Borrow from library",i);
        fclose(fstd);
        Clear_Screen();
    }
}

//--------------------------------------------for clean the screen--------------------------------------------
void Clear_Screen()
{
    gotoxy(50,20);
    printf("Press Enter to hide this list: ");
    fflush(stdin);
    getch();
    system("cls");
}

//--------------------------------------------Most Issue in Library--------------------------------------------
void issmost(void)
{
    Library l;
    int n,j=6;
    gotoxy(50,1);
    printf("Enter the number for sort by: ");
    scanf("%d",&n);
    flib=fopen("Books.bin","rb");
    system("cls");
    printf("\t\t\t\t\t\t-----------------------------------");
    printf("\n\t\t\t\t\t\t ISBN  |  BOOK NAME   |  rate  \n");
    printf("\t\t\t\t\t\t-------------------------------------\n\n\t\t");
    while(fread(&l,sizeof(Library),1,flib))
    {
        if(l.nummost>=n)
        {
            gotoxy(50,j);
            printf("%d",l.ISBN);
            gotoxy(58,j);
            printf("%s",l.Title);
            gotoxy(73,j);
            printf("%d\n",l.nummost);
            j++;
        }
    }
    fclose(flib);
}

//--------------------------------------------Delete Book From Library--------------------------------------------
void Delete_Books()
{
    Library l;
    system("cls");
    int d;
    char another='y';
    while(another=='y')
    {
        system("cls");
        printf("\n\n\t\t\t\tEnter the Book ID to  delete:");
        scanf("%d",&d);
        flib=fopen("Books.bin","rb+");
        rewind(flib);
        while(fread(&l,sizeof(Library),1,flib))
        {
            if(l.ISBN==d)
            {
                printf("\n\t\t\t\t\tThe book record is available");
                printf("\n\t\t\t\t\tBook name is %s",l.Title);
                printf("\n\t\t\t\t\tName Section is %s",l.Section);
                findbook='t';
            }
        }
        if(findbook!='t')
        {
            printf("\n\t\t\t\tNo record is found modify the search");
            if(getch())
            {
                fclose(flib);
                mainmenu();
            }
        }
        if(findbook=='t' )
        {
            printf("\n\t\t\t\tDo you want to delete it?(Y/N):");
            if(getch()=='y')
            {
                ft=fopen("test.dat","wb+");  //temporary file for delete
                rewind(flib);
                while(fread(&l,sizeof(Library),1,flib)==1)
                {
                    if(l.ISBN!=d)
                    {
                        fseek(ft,0,SEEK_CUR);
                        fwrite(&l,sizeof(Library),1,ft); //write all in temporary file except that
                    }                              //we want to delete
                }
                fclose(ft);
                fclose(flib);
                remove("Books.bin");
                rename("test.dat","Books.bin"); //copy all item from temporary file to flip except that
                flib=fopen("Books.bin","rb+");    //we want to delete
                if(findbook=='t')
                {
                    printf("\n\t\t\t\tThe record is successfully deleted");
                    printf("\n\t\t\t\tDelete another record?(Y/N)");
                }
            }
            else
            {
                fclose(flib);
                mainmenu();
            }
            fflush(stdin);
            another=getch();
        }
    }
    printf("\n\t\t\t\t");
    fclose(flib);
    mainmenu();
}


void notreturn(void)
{
    system("cls");
    Borrow b;
    SYSTEMTIME time;
    GetSystemTime(&time);
    fb=fopen("Borrow.bin","rb");

    a:

    while(fread(&b,sizeof(Borrow),1,fb)==1)
    {
        if(b.borrow_date.year<time.wYear)
        {
            printf("aaa\n");
            goto a;
        }
        if(b.borrow_date.month<time.wMonth)
        {
            printf("bnb\n");
            goto a;
        }
    }
    Clear_Screen();
}
