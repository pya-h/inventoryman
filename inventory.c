#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_ENTANCE "entrance.dat"

struct time
{
    int year, month, day, hour , minute;
};

struct block
{
    struct time timestamp;
    int price, weight, trxFee;
    char type[20];
};

void sortBlocks(struct block *blocks, int n);
void swapBlocks(struct block *blocks , int b1, int b2);
void lowStringCopy(char *s, char *result);

int main()
{
    int numberOfBlocks = 0, i = 0;
    struct block *blocks;
    struct time today;
    int incommingHour = 0;
    FILE *fDatabase;

    printf("Please enter the number of blocks: ");
    scanf("%d", &numberOfBlocks);
    blocks = (struct block *) malloc(numberOfBlocks * sizeof(struct block));
    // unpleasant use of malloc :(
    printf("First off, please enter today's date in this format: (yyyy mm dd):\n");
    scanf("%d %d %d", &today.year, &today.month, &today.day);

    // today starts at 00:00
    today.hour = today.minute = 0;
    printf("Please enter parameters below, to collect all blocks data:\n");

    for(i = 0; i < numberOfBlocks; i++)
    {
        printf("\n-----------------------------------------------------\n");
        printf("Block #%d {\n", i+1);
        printf("\tType: ");
        scanf("%s",blocks[i].type);
        printf("\tPrice: ");
        scanf("%d", &blocks[i].price);
        printf("\tWeight: ");
        scanf("%d", &blocks[i].weight);
        printf("\tTrxFee: ");
        scanf("%d", &blocks[i].trxFee);

        // set the date same as today ( according to the question)
        blocks[i].timestamp.year = today.year;
        blocks[i].timestamp.month = today.month;
        blocks[i].timestamp.day = today.day;

        printf("\tblock's incomming time: (hh mm)\n\t");
        scanf("%d %d", &blocks[i].timestamp.hour, &blocks[i].timestamp.minute);
        printf("}");
    }
    printf("\nProgram collected blocks data successfully!\n");

    sortBlocks(blocks, numberOfBlocks);

    printf("\n-----------------------------------------------------\n");
    printf("Now, please enter the hour which you want to see its incomming blocks: ");
    scanf("%d", &incommingHour);

    for(i = 0;i < numberOfBlocks; i++)
    {
        // all data is for today, so we just need to check hours (no need to check year/month/day)
        if(blocks[i].timestamp.hour == incommingHour)
            printf("%s\t%dT\t%dKg\t%d\n", blocks[i].type, blocks[i].price, blocks[i].weight, blocks[i].trxFee);

    }

    printf("\nUpdating the database...\n");
    fDatabase = fopen(FILE_ENTANCE, "a");

    if(!fDatabase)
    {
        printf("Something went wrong while oppening the database!");
        printf("Done!\nPress any key to close...");
        getchar();
        exit(-1);
    }

    for(i = 0; i < numberOfBlocks; i++)
    {
        // write timestamp
        // if we wanted to read from the database : we should throw out extra chars like '/' or ':' or '-'
        // ' between values that must be dumped with %c into a temp character (int fscanf)
        fprintf(fDatabase, "%d / %d / %d - %d : %d", blocks[i].timestamp.year, blocks[i].timestamp.month, blocks[i].timestamp.day,
                                                                blocks[i].timestamp.hour, blocks[i].timestamp.minute);
        // write block's data
        fprintf(fDatabase, "\t%s\t%d\t%d\t%d\n", blocks[i].type, blocks[i].weight, blocks[i].price, blocks[i].trxFee);

    }
    printf("today's entrances have been successfully added to the database...\n");

    printf("Done!\nPress any key to close...");
    getchar();

    fclose(fDatabase);
    free(blocks);
    return 0;
}

void swapBlocks(struct block *blocks, int b1, int b2)
{
    struct block temp;
    strcpy(temp.type, blocks[b2].type);
    temp.price = blocks[b2].price;
    temp.weight = blocks[b2].weight;
    temp.trxFee = blocks[b2].trxFee;
    temp.timestamp.year = blocks[b2].timestamp.year;
    temp.timestamp.month = blocks[b2].timestamp.month;
    temp.timestamp.day = blocks[b2].timestamp.day;
    temp.timestamp.hour = blocks[b2].timestamp.hour;
    temp.timestamp.minute = blocks[b2].timestamp.minute;

    strcpy(blocks[b2].type, blocks[b1].type);
    blocks[b2].price = blocks[b1].price;
    blocks[b2].weight = blocks[b1].weight;
    blocks[b2].trxFee = blocks[b1].trxFee;
    blocks[b2].timestamp.year = blocks[b1].timestamp.year;
    blocks[b2].timestamp.month = blocks[b1].timestamp.month;
    blocks[b2].timestamp.day = blocks[b1].timestamp.day;
    blocks[b2].timestamp.hour = blocks[b1].timestamp.hour;
    blocks[b2].timestamp.minute = blocks[b1].timestamp.minute;

    strcpy(blocks[b1].type, temp.type);
    blocks[b1].price = temp.price;
    blocks[b1].weight = temp.weight;
    blocks[b1].trxFee = temp.trxFee;
    blocks[b1].timestamp.year = temp.timestamp.year;
    blocks[b1].timestamp.month = temp.timestamp.month;
    blocks[b1].timestamp.day = temp.timestamp.day;
    blocks[b1].timestamp.hour = temp.timestamp.hour;
    blocks[b1].timestamp.minute = temp.timestamp.minute;

}

void sortBlocks(struct block *blocks, int n)
{
    int i = 0;
    // i wrote this sorting function in a way that it sorts the blocks no matter how much time difference they have
    // the blocks could all be for the same day, or for entirely different dates
    for(i = 0; i < n;i++)
    {
        int  j = 0;

        for(j = i + 1; j < n; j++)
        {
            if(blocks[i].timestamp.year > blocks[j].timestamp.year)
                swapBlocks(blocks, i, j);
            else if(blocks[i].timestamp.year == blocks[j].timestamp.year)
            {
                if(blocks[i].timestamp.month > blocks[j].timestamp.month)
                    swapBlocks(blocks, i, j);
                else if(blocks[i].timestamp.month == blocks[j].timestamp.month)
                {
                    if(blocks[i].timestamp.day > blocks[j].timestamp.day)
                        swapBlocks(blocks, i, j);
                    else if(blocks[i].timestamp.day == blocks[j].timestamp.day)
                    {
                        if(blocks[i].timestamp.hour > blocks[j].timestamp.month)
                            swapBlocks(blocks, i, j);
                        else if(blocks[i].timestamp.hour == blocks[j].timestamp.hour)
                        {
                            if(blocks[i].timestamp.minute > blocks[j].timestamp.minute)
                                swapBlocks(blocks, i, j);
                            else
                            {
                                char type_i[20], type_j[20];
                                // in alphabet sort, low or high character must be ignored
                                lowStringCopy(blocks[i].type, type_i);
                                lowStringCopy(blocks[j].type, type_j);
                                if(strcmp(type_i, type_j) > 0)
                                    swapBlocks(blocks, i, j);
                            }
                        }
                    }

                }

            }

        }
    }
}

void lowStringCopy(char *s, char *result)
{
    // convert *s to low string and copy it to result
    int i = 0;

    for(i = 0; s[i]; i++)
    {
        result[i] = tolower(s[i]);
    }

    result[i] = 0;

}
