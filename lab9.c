#include <stdio.h>
#include <stdlib.h> // Include this header file since warning for using malloc can be triggered otherwise.

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	int id;
	char name;
	int order;
	struct HashType *next;
};

// Compute the hash function
int hash(int x)
{
	x = (x * x) % 720;
	return x;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		if (pHashArray[i].id != 0) {
			printf("index %d -> %d %c %d", i, pHashArray[i].id, pHashArray[i].name, pHashArray[i].order);

			// While the index still contains linked records, use a temp pointer to traverse that linked list, print all records.
            struct HashType* tmp = pHashArray[i].next;
            while (tmp != NULL) {
                tmp = tmp->next;
                printf(" -> %d %c %d", tmp[i].id, tmp[i].name, tmp[i].order);
            }

			// Print a newline.
            printf("\n");
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords); // Changed from "input.txt" to input the correct name of the given file.
	printRecords(pRecords, recordSz);
	// Your hash implementation
	
    struct HashType *table; // The hash table, a pointer to a dynamically allocated array of HashType structures.
	table = (struct HashType*) malloc(720 * sizeof(struct HashType));
    // Using the ID component to determine a record's index position in the hash table, size the table for 6! records.
	int tableSz = 720; // 6! = 6 * 5 * 4 * 3 * 2 * 1 = 720.
	
	// Initialize the id components to 0 for every index in the hash table, indicating that index is NOT occupied by a record.
	for (int k = 0; k < tableSz; k++) {
		table[k].id = 0;
        table[k].next = NULL; // Furthermore, initialize the next pointers to NULL, indicating no other records occupy index k.
	}

	// Hash records from DMA pRecords into the hash table.
	for (int j = 0; j < recordSz; j++) {
        // Calculate the index value in the hash table to assign a record to, using the id component of the RecordTypes.
		int hIndex = hash(pRecords[j].id);
        int encounter = 0; // If this int is non-zero, no record was encountered to be all ready occupying table[hIndex].

        // For each element in the array of RecordTypes, pRecords...
		for (int y = 0; y < tableSz; y++) {
            // If the given index in the hash table is all ready occupied...
			if (table[hIndex].id != 0) {
				// Increment the int 'encounter', making its value non-zero to signify the index was all ready occupied...
                encounter += 1;
                // Navigate via the next link pointer to the last linked record occupying this index...
				struct HashType* tmp = table[hIndex].next;
				if (tmp != NULL) {
                    while (tmp != NULL) {
                        tmp = tmp->next;
                    }
                    // Then allocate memory for a new record linked to the prior last, enter the data accordingly.
                    tmp->next = (struct HashType*) malloc(sizeof(struct HashType));
                    tmp->next->id = pRecords[j].id;
                    tmp->next->name = pRecords[j].name;
                    tmp->next->order = pRecords[j].order;
					tmp->next->next = NULL;
                }
			}
		}

        // If the index position was not all ready occupied, enter the data at that position.
        if (encounter == 0) {
            table[hIndex].id = pRecords[j].id;
            table[hIndex].name = pRecords[j].name;
            table[hIndex].order = pRecords[j].order;
        }
	}

	// Print the records stored in the hash table to the display.
	displayRecordsInHash(table, tableSz);
}