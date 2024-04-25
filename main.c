#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_BOOKS 200

struct book {
    char title[100];
    char author[100];
    char isbn[20];
    char year_of_publication[20];
    char publisher[100];
    int is_borrowed;
};

int read_books(struct book *books, char *filename);
void display_books(struct book *books, int num_books);
void remove_book(struct book *books, int *num_books);
void add_book(struct book *books, int *num_books);
void search_books_by_title(struct book *books, int num_books);
void search_books_by_author(struct book *books, int num_books);
void search_books_by_year(struct book *books, int num_books);
void search_books_by_publisher(struct book *books, int num_books);
void countUniquePublishers(struct book *books, int num_books);
void countUniqueAuthors(struct book *books, int num_books);
void borrow_book(struct book *books, int *num_books);
void return_book(struct book *books, int *num_books);

int compare_by_year(const void *a, const void *b) {
    struct book *bookA = (struct book *)a;
    struct book *bookB = (struct book *)b;
    return strcmp(bookA->year_of_publication, bookB->year_of_publication);
}



int main() {
    struct book books[MAX_BOOKS];
    int num_books = read_books(books, "books.csv");
    int choice;

    do {
        printf("\nLibrary Management System\n");
        printf("1. Display all books\n");
        printf("2. Remove a book\n");
        printf("3. Add a book\n");
        printf("4. Search for a book by title\n");
        printf("5. Search for a book by author\n");
        printf("6. Search for a book by year\n");
        printf("7. Search for a book by publisher\n");
        printf("8. Sort books by year (ascending)\n");
        printf("9. Sort books by year (descending)\n");
        printf("10. Display the number of publishers\n");
        printf("11. Display the number of authors\n");
        printf("12. Borrow a book\n");
        printf("13. Return a book\n");
        printf("0. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_books(books, num_books);
                break;
            case 2:
                remove_book(books, &num_books);
                break;
            case 3:
                add_book(books, &num_books);
                break;
            case 4:
                search_books_by_title(books, num_books);
                break;
            case 5:
                search_books_by_author(books, num_books);
                break;
            case 6:
                search_books_by_year(books, num_books);
                break;
            case 7:
                search_books_by_publisher(books, num_books);
                break;
            case 8:
                qsort(books, num_books, sizeof(struct book), compare_by_year);
                display_books(books, num_books);
                break;
            case 9:
                qsort(books, num_books, sizeof(struct book), compare_by_year);
                for(int i = 0; i < num_books / 2; i++){
                    struct book temp = books[i];
                    books[i] = books[num_books - i - 1];
                    books[num_books - i - 1] = temp;
                }
                display_books(books, num_books);
                break;
            case 10:
                countUniquePublishers( books,num_books);
                break;
            case 11:
                countUniqueAuthors(books,num_books);
                break;
            case 12:
                borrow_book(books, &num_books);
                break;
            case 13:
                return_book(books, &num_books);
                break;

            case 0:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 0);

    return 0;
}

int read_books(struct book *books, char *filename) {
    FILE *fp;
    char line[4096];
    int num_books = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return -1;
    }
    int linenumber = 0;
    fgets(line, sizeof(line), fp);
    while (linenumber<MAX_BOOKS && fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%[^;];%[^;];%[^;];%[^;];%[^;]", books[num_books].isbn,
                   books[num_books].title, books[num_books].author,
                   books[num_books].year_of_publication, books[num_books].publisher) == 5) {
            num_books++;
        }
        linenumber++;
    }

    fclose(fp);
    return num_books;
}

void display_books(struct book *books, int num_books) {
    printf("\nBooks in the library:\n");
    for (int i = 0; i < num_books; i++) {
        printf("ISBN: %s, Title: %s, Author: %s, Year: %s, Publisher: %s\n",
               books[i].isbn, books[i].title, books[i].author,
               books[i].year_of_publication, books[i].publisher);
    }
    printf("Total number of books: %d\n", num_books);
}

void remove_book(struct book *books, int *num_books) {
    char isbn[20];
    printf("Enter ISBN of the book to remove: ");
    scanf("%s", isbn);

    for (int i = 0; i < *num_books; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) {
            for (int j = i; j < *num_books - 1; j++) {
                books[j] = books[j + 1];
            }
            (*num_books)--;
            printf("Book removed successfully.\n");
            return;
        }
    }
    printf("Book with ISBN %s not found.\n", isbn);
}

void add_book(struct book *books, int *num_books) {
    if (*num_books >= MAX_BOOKS) {
        printf("Library is full! Cannot add more books.\n");
        return;
    }

    printf("Enter book details:\n");
    printf("ISBN: ");
    scanf("%s", books[*num_books].isbn);
    printf("Title: ");
    scanf(" %[^\n]", books[*num_books].title);
    printf("Author: ");
    scanf(" %[^\n]", books[*num_books].author);
    printf("Year of Publication: ");
    scanf("%s", books[*num_books].year_of_publication);
    printf("Publisher: ");
    scanf(" %[^\n]", books[*num_books].publisher);

    (*num_books)++;
    printf("Book added successfully.\n");
}

void search_books_by_title(struct book *books, int num_books) {
    char title[100];
    int flag = 0;
    printf("Enter book's name: ");
    scanf(" %[^\n]", title);

    for (int i = 0; i < num_books; i++) {
        if (strstr(books[i].title, title)) {
            printf("ISBN: %s, Title: %s, Author: %s, Year: %s, Publisher: %s\n",
                   books[i].isbn, books[i].title, books[i].author,
                   books[i].year_of_publication, books[i].publisher);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf("We don't have a book with this title.\n");
    }
}

void search_books_by_author(struct book *books, int num_books) {
    char author[100];
    int flag = 0;
    printf("Enter author's name: ");
    scanf(" %[^\n]", author);

    for (int i = 0; i < num_books; i++) {
        if (strstr(books[i].author, author)) {
            printf("ISBN: %s, Title: %s, Author: %s, Year: %s, Publisher: %s\n",
                   books[i].isbn, books[i].title, books[i].author,
                   books[i].year_of_publication, books[i].publisher);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf("We don't have a book by this author.\n");
    }
}

void search_books_by_year(struct book *books, int num_books) {
    char* year;
    int flag = 0;
    printf("Enter publication year: ");
    scanf("%s", year);

    for (int i = 0; i < num_books; i++) {
        if (strcmp(books[i].year_of_publication,year)== 0) {
            printf("ISBN: %s, Title: %s, Author: %s, Year: %s, Publisher: %s\n",
                   books[i].isbn, books[i].title, books[i].author,
                   books[i].year_of_publication, books[i].publisher);
            flag = 1;
        }


    }
    if (flag == 0) {
        printf ("We don't have any book for that year.\n");
    }
}


void search_books_by_publisher(struct book *books, int num_books) {
    char publisher[100];
    int flag = 0;
    printf("Enter publisher's name: ");
    scanf(" %[^\n]", publisher);

    for (int i = 0; i < num_books; i++) {
        if (strstr(books[i].publisher, publisher)) {
            printf("ISBN: %s, Title: %s, Author: %s, Year: %s, Publisher: %s\n",
                   books[i].isbn, books[i].title, books[i].author,
                   books[i].year_of_publication, books[i].publisher);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf ("We don't have any books from this publisher.\n");
    }
}
void countUniquePublishers(struct book *books, int num_books) {
    printf("All Publishers and Their Counts:\n");
    char uniquePublishers[100][100];
    int publisherCount[100] = {0};
    int count = 0;

    for (int i = 0; i < num_books; i++) {
        int isUnique = 1;
        for (int j = 0; j < count; j++) {
            if (strcmp(uniquePublishers[j], books[i].publisher) == 0) {
                isUnique = 0;
                publisherCount[j]++;
                break;
            }
        }
        if (isUnique) {
            strcpy(uniquePublishers[count], books[i].publisher);
            publisherCount[count]++;
            count++;
        }
    }

    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", uniquePublishers[i], publisherCount[i]);
    }
    printf("Total number of publishers: %d\n", count);
}
void countUniqueAuthors(struct book *books, int num_books) {
    printf("All Publishers and Their Counts:\n");
    char uniqueAuthors[100][100];
    int authorCount[100] = {0};
    int count = 0;

    for (int i = 0; i < num_books; i++) {
        int isUnique = 1;
        for (int j = 0; j < count; j++) {
            if (strcmp(uniqueAuthors[j], books[i].author) == 0) {
                isUnique = 0;
                authorCount[j]++;
                break;
            }
        }
        if (isUnique) {
            strcpy(uniqueAuthors[count], books[i].author);
            authorCount[count]++;
            count++;
        }
    }

    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", uniqueAuthors[i], authorCount[i]);
    }
    printf("Total number of authors: %d\n", count);
}
void borrow_book(struct book *books, int *num_books) {
    char input[100];
    printf("Enter ISBN or title of the book to borrow: ");
    scanf(" %[^\n]", input);

    for (int i = 0; i < *num_books; i++) {
        if (strcmp(books[i].isbn, input) == 0 || strcmp(books[i].title, input) == 0) {
            if (books[i].is_borrowed == 1) {
                printf("This book is already borrowed.\n");
                return;
            }
            books[i].is_borrowed = 1;
            printf("Book borrowed successfully.\n");
            return;
        }
    }
    printf("Book with ISBN or title %s not found.\n", input);
}

void return_book(struct book *books, int *num_books) {
    char input[100];
    printf("Enter ISBN or title of the book to return: ");
    scanf(" %[^\n]", input);

    for (int i = 0; i < *num_books; i++) {
        if (strcmp(books[i].isbn, input) == 0 || strcmp(books[i].title, input) == 0) {
            if (books[i].is_borrowed == 0) {
                printf("This book is not borrowed.\n");
                return;
            }
            books[i].is_borrowed = 0;
            printf("Book returned successfully.\n");
            return;
        }
    }
    printf("Book with ISBN or title %s not found.\n", input);
}
