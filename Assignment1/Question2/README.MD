# Assignment2 Question2 (Linear Hashtable)
Yaowen Mei (1177855) Fall 2022
## Design
There are totally 46882 records, these records are saved to a database file, _**database_file.txt**_, in blocks of 1024 bytes;

A linear hash table is generated as index file to access the data saved in this _**database_file.txt**_ file. The linear hash table is then saved as _**linked_hashtable.txt**_file in the hard disk with block size 200 bytes.

The linear hash table is designed as the following:
1. A **hash function** convert 9 bytes char to unsigned short: there are only 46K records, the keys are uniformly generated 9 bytes chars, so unsigned short number (from 0 to 65535) should be able to serve as the hash key. In order to get the hashed shrot nubmer, the 9 bytes char first hashed to unsigned long with the STL hash function, and then take a mode with 60029 (a prime number) to convert the hash result into the range of unsigned short;
2. Bucket Structure:
   1. Each bucket is in the size of 200 bytes, the first two bytes are # of records it contains, the following two bytes is a hard disk pointer (unsigned short) to its overflow bucket; then each record is saved as 14 bytes chars (9 bytes field1 + 2 bytes database file bucket num [short]  + 2 bytes database file record num[short]).
   2. The max record can be stored in a bucket is set to be 10 (4 + 10*13 = 134 < 200), if more than 10 hash collision happens, the new records are saved to the overflow buckets until split;
   3. The first block of the Hashmap file is a header file, contains i, n, r, and where to insert next overflow bucket. This header need to be read everytime when we do search (to get current i value), and insert (to get the current i value and where to insert the overflow bucket). After insert is done, this header block also need to be updated.
   4. The threshold value is set to be 5, which means on average case, there should be 5 records in a bucket. I set the bucket size to be 200 bytes and set the limit of max_record_num is 10 (twice of the threshold to cover worst case). Given the fact that after read the data into the RAM, it is very fast to loop and find the target result from all the records in a bucket, to have a slightly large threshold value can trade off the cost of a lot of disk reads when the overflow chain is getting too big when threshold is small. Based on the 46882 records we have, I found that the largest bucket was chained by two block and contains 17 records, the smallest bucket contains only one record, and there are 136/9377 (1.4%) buckets are empty. This means my design is successful: hash function is uniform, and super long chain of overflow buckets are avoided. 

## How to run this program:
1. unzip the file into a folder in CoCS linux;
2. make a build folder `mkdir build`;
3. cd to the build folder, and build: `cd build`, then `cmake..` and `make`;
4. Then you can run this program with `./Question2`
5. After the program is running, you can use `h` to see the help, briefly, you can type `b` to build all the files that need to be saved on disk; `i` for insert, and `s` for search;
6. Please note that, at the very beginning,  you must run `b` to generate the disk files before you can search and insert. You can run `b` whenever you want to reset the disk files (remove all the new records you manually inserted).
7. Please do not insert any existing key twice, I didn't check and throw this as this is not required in this assignment.

## Briefly compare search performance:
Under the scale of current input dataset, the disk read num for B+Tree is **8 times**: 

- which includes 7 time BTree datafile read (the tree is 7 levels high), 
- and then after got the database file block num from the btree, we need to do one more time disk read on the database file to get the field2 and field 3 of the record back;

The disk read time for Linear Hashtable is **roughly 3 times**:

- The 1st time we read the hashmap header file to get the current `i` value;
- On average, we will then do another 1 disk read on the hashmap file to get about 5 records (threshold is 5) in RAM, and select out the one we want.
- In case of overflow bucket exist or unsplited parent bucket exist, we need to read a few more times;
- Finally, we need to use the index (bucket num, and record num) obtained from hashmap to perform one time disk read on the database file.

In summary, BTree disk read times are very stable (you will need to insert another 40K records to increase the hight of the tree by one), while Linear Hashtable disk read times is not very stable (overflow buckets and non-split parent buckets can exist). However, when set B+Tree m = 8, and set Linear Hashtable threshold = 5, on average, B+Tree disk read time is larger than Linear Hashtable.
**Based on the sample input data size, Linear Hash Table consumes less disk reads than B+Tree.**