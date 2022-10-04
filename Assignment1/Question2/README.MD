# Assignment2 Question2 (Linear Hashtable)

## Design
There are totally 46882 records, these records are saved to a database file, _**database_file.txt**_, in blocks of 1024;

The unsigned short hold value from 0 to 65535, should be more than enough to hold all these records.
46877 is the prime smaller than 46882, so we can hash field one to a short, and mode with 46877 as its hash value;

These 46882 pointers (4 bytes each): # of block (short) and # of record in the block (short) are saved as value in the linear hashtable;

For the linear hash table, the block size is set to be 512 bytes, so that this will be enough for saving 38 pointers (2 bytes num of record + (4 bytes value + 9 bytes key)*N = 512 => N = 39);

The hash function, h(K), is simply using the STD to hash a 9 bytes string to an `unsigned long long` with `std::hash<std::string> hasher`

The first Block(512 kb) of the hash table file is the file header, the following things need to be saved and updated when changed:
* i: currently how many bits from right we are using (**unsigned short**), from 1 to 64 ;
* n: currently how many buckets are used (**unsigned short**) (at most 38 record pointers will be saved in each bucket, overflow bucket will be generated when more than 38 records in bucket);
* startOverFlowBlock: (**unsigned short**) currently, the start of the overflow block, if the `n` is close to this number, move everything below startOverFlowBlock future down, and update this value;
* shiftTonextAvailableOverFlowBlock: (**unsigned short**) if a bucket requires an overflow block, it will read the header block, occupy the **block #(startOverFlowBlock + shiftTonextAvailableOverFlowBlock)**, and then increase **shiftTonextAvailableOverFlowBlock** by one;
* pre-defined threshold is 1.7 (does not need to save to header)