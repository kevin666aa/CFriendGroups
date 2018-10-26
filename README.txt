README

included:
All source files (.cpp, .h)
this README
Makefile
Data (.csv)
Some early versions(.cpp)

Proj2 ECS60

I put a lot of time into this project.

This project is easy to finish, but hard to optimize. For version1, 2s, my search function is O(n^2), which is quite slow and couldn't finish friendslg.csv. 

I did a version 3 which is a weird complicate structure that runs very slow. 

Version4 I tried binary search tree and binary tree, but still not right. 

Then I started using hash table. It took me two days to understand what it is exactly. But my first edition is rather slow. Then I deleted a vector that used to keep track of roots, it really speeded things up, but still slower that baseline. I adjusted the structure twice and finally, I have
name_tb, a vector of value(name, id of what it connected, depth) that don't change
hash_tb, a hash table of integers(the index of name_tb)
So every time I need the name, I will go into name_tb to find it and use

The algorithms used are weighted quick-union(with pass compression) and hash table.
 
Partner: Quanzhou Chen

Reference: 
	hash structure: Algorithms 4th edition Chapter hash table
	hash function: https://research.cs.vt.edu/AVresearch/hashing/strings.php
	quadratic probing: https://www.sanfoundry.com/c-program-implement-hash-tables-quadratic-probing/


Yiran Wu (Kevin)  Aug.22, 2018
	