### Program that solves word-chain puzzle

There’s a type of puzzle where the challenge is to build a chain of words, starting with one particular word and ending with another. Successive entries in the chain must all be real words, and each can differ from the previous word by just one letter. For example, you can get from "cat" to "dog" using the following chain.

```
  cat -> cot -> cog -> dog
```

### Solution
In this solution, we use Breadth First Search to construct a graph, in which every node is a word and all the adjacents of a word are 1-edit dictionary words. Also using a Trie for word lookup. For answering queries we again use the constructed graph to perform BFS.

### Other Approach
Strings can be "hashed" without using "tries". Adjacency matrix can then be constructed.

### Related Problem

List all the anagrams of a given word. Can be solved efficiently using hashing.
Feel free to explore and adapt this solution to tackle similar word-related challenges efficiently.

Happy coding! 🚀
