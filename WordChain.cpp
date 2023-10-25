// Indix - Code Kata 19
// String Handling
// One edit distance queries.
// Complexity O(wnlogn + (25w**2)n) for preprocessing & O(n + nw) per query.
// n = total number of dictionary words.
// w = word length.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#define ALPHABETS 26
#define INDEX(c) c - 'a'
#define MAXN 5005

using namespace std;

// Mapping words to integers.
map<string, int> wordMap;
//

// BFS Implementation.
int graphNodesCount = 0, graphEdgesCount = 0;
vector<int> adjacents[MAXN];

int previousNode[MAXN] = {0};

struct GraphNode
{
  int nodeID;
  int cost;
};

int BFS(int source, int destination)
{
  struct GraphNode parent, child;
  queue<GraphNode> Q;
  vector<int>::iterator it;
  bool visited[MAXN] = {0};

  if(source == destination) return 0;

  parent.nodeID = source;
  parent.cost = 0;
  Q.push(parent);
  while(!Q.empty())
  {
    parent = Q.front();
    Q.pop();
    visited[parent.nodeID] = 1;
    if(parent.nodeID == destination) return parent.cost;

    for(it = adjacents[parent.nodeID].begin(); it != adjacents[parent.nodeID].end(); ++it)
    {
      if(!visited[*it])
      {
        child.nodeID = *it;
        child.cost = parent.cost + 1;
        Q.push(child);

        // To trace the path...
        previousNode[*it] = parent.nodeID;
      }
    }
  }

  return -1; // Coudn't reach.
}
//

// Trie Implementation.
int totalTrieNodes = 0;

struct TrieNode
{
  int value;
  struct TrieNode *children[ALPHABETS];
};
 
typedef struct Trie TrieADT;
struct Trie
{
  struct TrieNode *root;
  int count;
};

struct TrieNode *getNode(void)
{
  struct TrieNode *pNode = NULL;
 
  pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
  if(pNode)
  {
    pNode->value = 0;
    for(int i = 0; i < ALPHABETS; ++i)
     pNode->children[i] = NULL;
  }
 
  ++totalTrieNodes;
  return pNode;
}

void initialize(TrieADT *pTrie)
{
  pTrie->root = getNode();
  pTrie->count = 0;
}

void insert(TrieADT *pTrie, char key[])
{
  int level, index;
  int length = strlen(key);
  struct TrieNode *pCrawl;
 
  ++pTrie->count;
  pCrawl = pTrie->root;
  for(level = 0; level < length; ++level)
  {
    index = INDEX(key[level]);
    if(!pCrawl->children[index])
     pCrawl->children[index] = getNode();
 
    pCrawl = pCrawl->children[index];
  }
 
  pCrawl->value = pTrie->count;
}

int search(TrieADT *pTrie, char key[])
{
  int level;
  int length = strlen(key);
  int index;
  struct TrieNode *pCrawl;
 
  pCrawl = pTrie->root; 
  for(level = 0; level < length; ++level)
  {
    index = INDEX(key[level]);
 
    if(!pCrawl->children[index]) return 0;
    pCrawl = pCrawl->children[index];
  }
 
  return (0 != pCrawl && pCrawl->value);
}

int dictionaryCount = 0, queriesCount = 0;
char dictionaryWord[MAXN][20];
TrieADT trie;
//

// Generate 1-edit words.
int stringLength;
char originalWord[20] = {0}, editedWord[20] = {0};

void generateEdits(int position)
{
  for(char ch = 'a'; ch <= 'z'; ++ch)
  {
    editedWord[position] = ch;
    if(originalWord[position] == editedWord[position]) continue;
    else if(search(&trie, editedWord)) // Present in the trie.
    {
      string a(originalWord), b(editedWord);
      int u = wordMap[a], v = wordMap[b];

      adjacents[u].push_back(v);
      adjacents[v].push_back(u);
      ++graphEdgesCount;
    }
  }
}

void oneEdit()
{
  stringLength = strlen(originalWord);
  strcpy(editedWord, originalWord);

  for(int i = 0; i < stringLength; ++i)
  {    
    generateEdits(i);
    editedWord[i] = originalWord[i]; // Replace the original character.
  }
}
//

// Print the shortest path.
string pathNodes[MAXN];
int pathLength;
ofstream out("Output.txt");

void printPath(int source, int dest)
{
  int previous = dest;

  if(source == dest)
  {
    out << dictionaryWord[source - 1] << endl;
    return;
  }

  pathLength = 0;
  pathNodes[pathLength++] = dictionaryWord[dest - 1];
  previous = previousNode[previous];
  while(1)
  { 
    if(previous != source)
    {
      pathNodes[pathLength++] = dictionaryWord[previous - 1];
      previous = previousNode[previous];
    }
    else
    {
      pathNodes[pathLength++] = dictionaryWord[previous - 1];
      break; 
    }
  }
  
  out << pathNodes[pathLength - 1];
  for(int i = pathLength - 2; i >= 0; --i)
   out << "->" << pathNodes[i];
  out << endl;
}
//

int main()
{
  ifstream in1("Dictionary.txt");
  ifstream in2("Input.txt");

  // Build the Trie.
  initialize(&trie);
  while(in1 >> dictionaryWord[dictionaryCount])
  {
    insert(&trie, dictionaryWord[dictionaryCount]);

    // Map the word to number.
    string s(dictionaryWord[dictionaryCount]);
    wordMap[s] = dictionaryCount + 1;

    ++dictionaryCount;
    ++graphNodesCount;
  }  
  //

  // Build the Graph.
  for(int i = 0; i < dictionaryCount; ++i)
  {
    strcpy(originalWord, dictionaryWord[i]);
    oneEdit();
  }
  //

  // Process the queries...
  string sourceString, destString;
  while(in2 >> sourceString >> destString)
  {    
    out << sourceString << " " << destString << endl;
    int cost = BFS(wordMap[sourceString], wordMap[destString]);

    out << cost << " ";
    if(cost >= 0) printPath(wordMap[sourceString], wordMap[destString]);
    else out << "Not reachable !" << endl;
  }

  in1.close();
  in2.close();
  out.close();
  return 0;
}
