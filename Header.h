#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include<sstream>
using namespace std;
class Graph {
private:
    unordered_map<string, vector<string>> adjList;

public:
    void addEdge(const string& user, const string& follows);
    bool userExists(const string& user);
    void followUser(const string& user, const string& follows);
    void unfollowUser(const string& user, const string& follows);
    void readCSV(const string& filename);
};
struct User {
    string username;
    string password;
    bool loggedIn;
    User() : username(""), password(""), loggedIn(false) {}

    User(const string& name, const string& pass) : username(name), password(pass), loggedIn(false) {}
};

class HashTable {
private:
    static const int TABLE_SIZE = 100;
    struct Node {
        User user;
        Node* next;
        Node(const User& u) : user(u), next(nullptr) {}
    };
    Node* table[TABLE_SIZE];

    int hash(const std::string& key) const;

public:
    HashTable();
    ~HashTable();
    void insert(const User& user);
    User* search(const std::string& username) const;
    void remove(const std::string& username);
    int gethash(string& key);
};

class UserManagement {
private:
    static const int MAX_USERS = 100;
    User users[MAX_USERS];
    int userCount;
    string filename;
    HashTable hashTable;

    void loadUsers();
    void saveUsers() const;

public:
    UserManagement();
    UserManagement(const string& filename);
    bool registerUser(const string& username, const string& password);
    bool loginUser(const string& username, const string& password);
    void logoutUser(const string& username);
    User* getUser(const string& username);
};

struct Node {
    string project;
    Node* left, * right;
    Node() : project(""), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    Node* root;

    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);
    int getHeight(Node* node);
    int getBalanceFactor(Node* node);
    Node* insertNode(Node* node, const string& value);
    Node* findMin(Node* node);
    Node* deleteNode(Node* node, const string& value);
    void inorderTraversal(Node* node);
    void inorderTraversalToFile(Node* node, ofstream& file);

public:
    AVLTree();
    void addFile(const string& value);
    void deleteFile(const string& value);
    void displayRepository();
    void saveRepositoryToFile(const string& fileName);
};

class Repository {
private:
    AVLTree* repositories[100];
    int currentIndex;
    string repositoryName;

public:
    Repository();
    void createRepository(const string&, int hashVal);
    void addFileToRepository(int index, const string& projectName, int hashVal);
    void deleteFileFromRepository(int index, const string& value, int hashVal);
    void displayRepository(int index, int hashVal);
    void setRepositoryName(const string& name);
    string getRepositoryName(int);
    string getLineFromFile(const string& fileName, int lineNum);

};