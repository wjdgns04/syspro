#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "book.h"

int main(int argc, char *argv[])
{
   int fd, id;
   char c;
   struct book record;
   if (argc < 2) {
      fprintf(stderr,"How to use : %s file\n", argv[0]);
      exit(1);
   }
   if ((fd = open(argv[1], O_RDWR)) == -1) {
      perror(argv[1]);
      exit(2);
   }

   do {
      printf("Enter 0 or 1 to be modified: ");
      if (scanf("%d", &id) == 1) {
         lseek(fd, (long) (id-START_ID)*sizeof(record), SEEK_SET);
         if ((read(fd, (char *) &record, sizeof(record)) > 0) && (record.id != 0)) {
            printf("id:%d\t bookname:%s\t author:%s\t year:%d\t numofborrow:%d\t borrow:%s\n",
                    record.id, record.bookname, record.author, record.year, record.numofborrow, record.borrow);
            printf("Enter 0 to borrow book id\nEnter 1 to borrow False\n");
            scanf("%s", &record.borrow);
            lseek(fd, (long) -sizeof(record), SEEK_CUR);
            write(fd, (char *) &record, sizeof(record));
         } else printf("Record %s Null\n", borrow);
      } else printf("Insert Error\n");
      printf("Continue?(Y/N)");
      scanf(" %c",&c);
   } while (c == 'Y');
   close(fd);
   exit(0);
}


