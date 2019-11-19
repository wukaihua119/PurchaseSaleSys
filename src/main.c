#include <stdio.h>

/* define a struct */
struct item{
	size_t ItemNo_Internal_USE; /* Use it to locate the position of the record. */
	size_t Purchase_Date; /* The merchandise purchase date */
	char ItemNo[8]; /* The item number of merchandise defined by user */
	char Brand[20]; /* The detail of merchandise */
	char Scale[6]; 
	char Manufacture[20];
	char Detail_[20];
	char Color[9];
	size_t Cost; /* The purchase cost */
    size_t Sold_Date; /* The date of merchandise sold */
	size_t Revenue; /* The sold price of merchandise */
	size_t Profit; /* The profits */
	double ProfitMargin; 
};


/* Function prototypes */
void RECREATE( void );
int enterChoice( void );
void textFile( FILE *readPtr );
void updateRecord( FILE *fPtr );
void newRecord( FILE *fPtr );
void deleteRecord( FILE *fPtr );

int main( void ) {
    FILE *cfPtr; /* DataBase.dat file pointer */
    int choice; /* user's choice */

    /* fopen opens the file; RECREATE if file cannot be opened */
    if ( ( cfPtr = fopen( "DataBase.dat", "rb+" ) ) == NULL ) {
		RECREATE();
    } else {
        /* enable user to specify action */
        while ( ( choice = enterChoice() ) != 5 ) { 
            switch ( choice ) { 
                /* create text file from record file */
                case 1:
                    textFile( cfPtr );
                    break;
                    /* update record */
                case 2:
                    updateRecord( cfPtr );
                    break;
                    /* create record */
                case 3:
                    newRecord( cfPtr );
                    break;
                    /* delete existing record */
                case 4:
                    deleteRecord( cfPtr );
                    break;
                    /* display message if user does not select valid choice */
                default:
                    printf( "Incorrect choice\n" );
                    break;
            } /* end switch */
        } /* end while */

        fclose( cfPtr ); /* fclose closes the file */
    } /* end else */

    return 0; /* indicates successful termination */
} /* end main */

/* Recreate file */
void RECREATE( void ){
	
	FILE *cPtr;

	struct item detail = { 0, 0, "", "", "", "", "", "", 0, 0, 0, 0, 0.0 };

	if( ( cPtr = fopen( "DataBase.dat", "wb" ) ) == NULL ){
		printf( "File couldn't be opened.\n" );
	}else{
		for( int size_ = 1; size_ <= 1000; size_++ ){
			fwrite( &detail, sizeof( struct item ), 1, cPtr );
		}
	}
	fclose( cPtr );
}

/* create formatted text file for printing */ 
void textFile( FILE *readPtr ) {
    FILE *writePtr; /* DataBase.dat file pointer */

    /* create detailData with default information */
	struct item detail = { 0, 0, "", "", "", "", "", "", 0, 0, 0, 0, 0.0 } ;

    /* fopen opens the file; exits if file cannot be opened */
    if ( ( writePtr = fopen( "DataBase.txt", "w" ) ) == NULL ) {
        printf( "File could not be opened.\n" );
    } else {
        rewind( readPtr ); /* sets pointer to beginning of file */
		fprintf( writePtr, "                               The retail record is shown below.\n" );
		fprintf( writePtr, "%4s%14s%8s%9s%6s%14s%14s%10s%7s    ||%13s%10s%10s%14s\n%s\n", 
        		"No.", "Purchase_Date", "ItemNo", "Brand", "Scale", 
				"Manufacture", "Detail", "Color", "Cost", "Sold_Date", "Revenue", "Profit", "ProfitMargin",
				"-------------------------------------------------------------------------------------------------------------------------------------------" );
        /* copy all records from random-access file into text file */
        while ( !feof( readPtr ) ) { 
            fread( &detail, sizeof( struct item ), 1, readPtr );

            /* write single record to text file */
            if ( detail.ItemNo_Internal_USE != 0 ) {
				fprintf( writePtr, "%4zu%14zu%8s%9s%6s%14s%14s%10s%7zu    ||%13zu%10zu%10zu%14.2f\n",
	        			detail.ItemNo_Internal_USE, detail.Purchase_Date, detail.ItemNo, detail.Brand, detail.Scale, 
						detail.Manufacture, detail.Detail_, detail.Color, detail.Cost,
						detail.Sold_Date, detail.Revenue, detail.Profit, detail.ProfitMargin );
            } /* end if */
        } /* end while */

        fclose( writePtr ); /* fclose closes the file */
    } /* end else */
} /* end function textFile */

/* update balance in record */
void updateRecord( FILE *fPtr ) {
    int account; /* account number */
	int sold_Date;
    int transaction; /* transaction amount */

    /* create detailData with no information */
	struct item detail = { 0, 0, "", "", "", "", "", "", 0, 0, 0, 0, 0.0 };

    /* obtain number of account to update */
    printf( "Enter internal-use item number to update ( 1 - 10000 ): " );
    scanf( "%d", &account );

    /* move file pointer to correct record in file */
    fseek( fPtr, ( account - 1 ) * sizeof( struct item ), 
            SEEK_SET );

    /* read record from file */
    fread( &detail, sizeof( struct item), 1, fPtr );

    /* display error if account does not exist */
    if ( detail.ItemNo_Internal_USE == 0 ) {
        printf( "Acount #%d has no information.\n", account );
    } else { /* update record */
		printf( "%9zu%8s%9s%8s%14s%14s%10s%7zu\n",
    		detail.Purchase_Date, detail.ItemNo, detail.Brand, detail.Scale, 
			detail.Manufacture, detail.Detail_, detail.Color, detail.Cost );

        /* request transaction amount from user */ 
        printf( "Enter the sold_Date, revenue: " );
        scanf( "%d %d", &sold_Date, &transaction );
		detail.Sold_Date = sold_Date;
        detail.Revenue = transaction; /* update record balance */
		detail.Profit = detail.Revenue - detail.Cost;
		detail.ProfitMargin =  ( (double)detail.Profit / (double)detail.Cost); 
		printf( "%9zu%8s%9s%8s%14s%14s%10s%7zu%9zu%7zu%7zu%7.2lf\n",
    		detail.Purchase_Date, detail.ItemNo, detail.Brand, detail.Scale, 
			detail.Manufacture, detail.Detail_, detail.Color, detail.Cost,
			detail.Sold_Date, detail.Revenue, detail.Profit, detail.ProfitMargin );

        /* move file pointer to correct record in file */
        fseek( fPtr, ( account - 1 ) * sizeof( struct item ), 
                SEEK_SET );

        /* write updated record over old record in file */
        fwrite( &detail, sizeof( struct item ), 1, fPtr );
    } /* end else */
} /* end function updateRecord */

/* delete an existing record */
void deleteRecord( FILE *fPtr ) {
#if 1

    struct item read; /* stores record read from file */
	struct item detail = { 0, 0, "", "", "", "", "", "", 0, 0, 0, 0, 0.0 } ;

    int account; /* account number */

    /* obtain number of account to delete */
    printf( "Enter account number to delete ( 1 - 100 ): " );
    scanf( "%d", &account );

    /* move file pointer to correct record in file */
    fseek( fPtr, ( account - 1 ) * sizeof( struct item ), 
            SEEK_SET );

    /* read record from file */
    fread( &read, sizeof( struct item ), 1, fPtr );

    /* display error if record does not exist */
    if ( read.ItemNo_Internal_USE == 0 ) {
        printf( "Account %d does not exist.\n", account );
    } else { /* delete record */
        /* move file pointer to correct record in file */
        fseek( fPtr, ( account - 1 ) * sizeof( struct item ), 
                SEEK_SET );

        /* replace existing record with blank record */
        fwrite( &detail, 
                sizeof( struct item ), 1, fPtr );
    } /* end else */
#endif
} /* end function deleteRecord */

/* create and insert record */
void newRecord( FILE *fPtr ) {
    /* create detailData with default information */
	struct item detail = { 0, 0, "", "", "", "", "", "", 0, 0, 0, 0, 0.0 } ;

    int accountNum; /* account number */

    /* obtain number of account to create */
    printf( "Enter new account number ( 1 - 1000 ): " );
    scanf( "%d", &accountNum );

    /* move file pointer to correct record in file */
    fseek( fPtr, ( accountNum - 1 ) * sizeof( struct item ), 
            SEEK_SET );

    /* read record from file */
    fread( &detail, sizeof( struct item ), 1, fPtr );

    /* display error if account already exists */
    if ( detail.ItemNo_Internal_USE != 0 ) {
        printf( "Account #%d already contains information.\n",
                detail.ItemNo_Internal_USE );
    } else { /* create record */
        printf( "Enter the Purchase_Date, ItemNo., Brand, Scale, Manufacture, Detail_, Color, Costs.\n" );
		
        
		scanf( "%zu%s%s%s%s%s%s%zu", 
	        	&(detail.Purchase_Date), detail.ItemNo, detail.Brand, detail.Scale, 
					detail.Manufacture, detail.Detail_, detail.Color, &(detail.Cost) );

        detail.ItemNo_Internal_USE = accountNum;

        /* move file pointer to correct record in file */
        fseek( fPtr, ( detail.ItemNo_Internal_USE - 1 ) * 
                sizeof( struct item ), SEEK_SET );

        /* insert record in file */
        fwrite( &detail, 
                sizeof( struct item ), 1, fPtr );
    } /* end else */
} /* end function newRecord */

/* enable user to input menu choice */
int enterChoice( void ) {
    int menuChoice; /* variable to store user's choice */

    /* display available options */
    printf( "\nEnter your choice\n"
            "1 - store a formatted text file of records called\n"
            "    \"DataBase.dat\" for printing\n"
            "2 - update an record\n"
            "3 - add a new record\n"
            "4 - delete an record\n"
            "5 - end program\n "
			"p.s. Account # - 1  = ItemNo \n? " );


    scanf( "%d", &menuChoice ); /* receive choice from user */
    return menuChoice;
} /* end function enterChoice */



