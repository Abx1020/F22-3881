#include "Header.h"

void Graph::addEdge(const string& user, const string& follows) {
    adjList[user].push_back(follows);
}
bool Graph::userExists(const string& user) {
    return adjList.find(user) != adjList.end();
}
void Graph::followUser(const string& user, const string& follows) {
    readCSV("users.csv");
    if (userExists(user)) {
        addEdge(user, follows);
        cout << "You are now following " << follows << endl;
    }
    else {
        cout << "User '" << user << "' does not exist." << endl;
    }
}
void Graph::unfollowUser(const string& user, const string& follows) {
    readCSV("users.csv");
    if (userExists(user)) {
        auto it = find(adjList[user].begin(), adjList[user].end(), follows);
        if (it != adjList[user].end()) {
            adjList[user].erase(it);
            cout << "You have unfollowed " << follows << endl;
        }
        else {
            cout << "You were not following " << follows << endl;
        }
    }
    else {
        cout << "User '" << user << "' does not exist." << endl;
    }
}
void Graph::readCSV(const string& filename) {
    adjList.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string user, follows;
        getline(ss, user, ',');
        getline(ss, follows, ',');
        addEdge(user, follows);
    }

    file.close();
}
int HashTable::hash(const std::string& key) const {
    int hashValue = 0;
    for (char c : key) {
        hashValue += static_cast<int>(c);
    }
    return hashValue % TABLE_SIZE;
}

int HashTable::gethash(string& key) {
    return hash(key);
}
HashTable::HashTable() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

void HashTable::insert(const User& user) {
    int index = hash(user.username);
    Node* newNode = new Node(user);
    newNode->next = table[index];
    table[index] = newNode;
}

User* HashTable::search(const std::string& username) const {
    int index = hash(username);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->user.username == username) {
            return &(current->user);
        }
        current = current->next;
    }
    return nullptr;
}

void HashTable::remove(const std::string& username) {
    int index = hash(username);
    Node* current = table[index];
    Node* prev = nullptr;
    while (current != nullptr) {
        if (current->user.username == username) {
            if (prev == nullptr) {
                table[index] = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

UserManagement::UserManagement() : userCount(0), filename("users.csv"), hashTable() {
    loadUsers();
}

UserManagement::UserManagement(const string& file) : userCount(0), filename(file), hashTable() {
    loadUsers();
}

void UserManagement::loadUsers()
{
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line) && userCount < MAX_USERS) {
            size_t commaIndex = line.find(',');
            string username = line.substr(0, commaIndex);
            string password = line.substr(commaIndex + 1);
            hashTable.insert(User(username, password));
            users[userCount].username = username;
            users[userCount].password = password;
            ++userCount;
        }
        file.close();
    }
}


void UserManagement::saveUsers() const {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < userCount; ++i) {
            file << users[i].username << "," << users[i].password << endl;
        }
        file.close();
    }
    else {
        cerr << "Error: Could not create or open file for writing." << endl;
    }
}


bool UserManagement::registerUser(const string& username, const string& password) {
    if (userCount < MAX_USERS) {
        for (int i = 0; i < MAX_USERS; ++i) {
            if (users[i].username == username) {
                cout << "Username already exists. Please choose a different one." << endl;
                return false;
            }
        }
        for (int i = 0; i < MAX_USERS; ++i) {
            if (users[i].username.empty()) {
                users[i].username = username;
                users[i].password = password;
                ++userCount;
                saveUsers();
                cout << "User " << username << " registered successfully." << endl;
                return true;
            }
        }
    }
    else {
        cout << "Maximum number of users reached. Cannot register new user." << endl;
    }
    return false;
}

bool UserManagement::loginUser(const string& username, const string& password) {
    User* user = hashTable.search(username);
    if (user != nullptr && user->password == password) {
        user->loggedIn = true;
        saveUsers(); // Save user data after logging in
        cout << "User " << username << " logged in successfully." << endl;
        return true;
    }
    cout << "Invalid username or password." << endl;
    return false;
}

void UserManagement::logoutUser(const string& username) {
    User* user = hashTable.search(username);
    if (user != nullptr) {
        user->loggedIn = false;
        saveUsers(); // Save user data after logging out
        cout << "User " << username << " logged out successfully." << endl;
    }
    else {
        cout << "User not found." << endl;
    }
}


User* UserManagement::getUser(const string& username) {
    return hashTable.search(username);
}
Node* AVLTree::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T = x->right;

    x->right = y;
    y->left = T;

    return x;
}

Node* AVLTree::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T = y->left;

    y->left = x;
    x->right = T;

    return y;
}

int AVLTree::getHeight(Node* node) {
    if (node == nullptr)
        return 0;
    return max(getHeight(node->left), getHeight(node->right)) + 1;
}

int AVLTree::getBalanceFactor(Node* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

Node* AVLTree::insertNode(Node* node, const string& value) {
    if (node == nullptr) {
        node = new Node();
        node->project = value;
        return node;
    }

    if (value < node->project)
        node->left = insertNode(node->left, value);
    else if (value > node->project)
        node->right = insertNode(node->right, value);
    else
        return node;

    int balance = getBalanceFactor(node);

    if (balance > 1 && value < node->left->project)
        return rotateRight(node);

    if (balance < -1 && value > node->right->project)
        return rotateLeft(node);

    if (balance > 1 && value > node->left->project) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && value < node->right->project) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

Node* AVLTree::findMin(Node* node) {
    if (node == nullptr)
        return nullptr;
    while (node->left != nullptr)
        node = node->left;
    return node;
}

Node* AVLTree::deleteNode(Node* node, const string& value) {
    if (node == nullptr)
        return node;

    if (value < node->project)
        node->left = deleteNode(node->left, value);
    else if (value > node->project)
        node->right = deleteNode(node->right, value);
    else {
        if (node->left == nullptr || node->right == nullptr) {
            Node* temp = node->left ? node->left : node->right;
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            }
            else
                *node = *temp;
            delete temp;
        }
        else {
            Node* temp = findMin(node->right);
            node->project = temp->project;
            node->right = deleteNode(node->right, temp->project);
        }
    }

    if (node == nullptr)
        return node;

    int balance = getBalanceFactor(node);

    if (balance > 1 && getBalanceFactor(node->left) >= 0)
        return rotateRight(node);

    if (balance > 1 && getBalanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && getBalanceFactor(node->right) <= 0)
        return rotateLeft(node);

    if (balance < -1 && getBalanceFactor(node->right
    ) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void AVLTree::inorderTraversal(Node* node) {
    if (node != nullptr) {
        inorderTraversal(node->left);
        cout << node->project << endl;
        inorderTraversal(node->right);
    }
}

void AVLTree::inorderTraversalToFile(Node* node, ofstream& file) {
    if (node != nullptr) {
        inorderTraversalToFile(node->left, file);
        file << node->project << endl;
        inorderTraversalToFile(node->right, file);
    }
}

AVLTree::AVLTree() : root(nullptr) {}

void AVLTree::addFile(const string& value) {
    root = insertNode(root, value);
}

void AVLTree::deleteFile(const string& value) {
    root = deleteNode(root, value);
}

void AVLTree::displayRepository() {
    inorderTraversal(root);
}

void AVLTree::saveRepositoryToFile(const string& fileName) {
    ofstream file(fileName);
    if (!file) {
        cerr << "Error: Could not create file." << endl;
        return;
    }
    inorderTraversalToFile(root, file);
    file.close();
}

Repository::Repository() : currentIndex(0) {
    for (int i = 0; i < 100; ++i)
        repositories[i] = nullptr;
}

void Repository::createRepository(const string& name, int hashVal) {
    // Construct file names based on the hash value
    string repoFileName = "repositoryname" + to_string(hashVal) + ".txt";
    string privateRepoFileName = "privaterepository" + to_string(hashVal) + ".txt";

    // Open repository file to count the number of lines
    ifstream repoNamesFile(repoFileName);
    int lineCount = 0;
    string line;
    while (getline(repoNamesFile, line)) {
        if (!line.empty()) {
            lineCount++;
        }
    }
    repoNamesFile.close();

    // Set currentIndex based on the line count
    int currentIndex = lineCount;

    if (currentIndex >= 100) {
        cout << "Cannot create more repositories. Maximum limit reached." << endl;
        return;
    }

    // Ask the user whether the repository should be private or public
    int privacyChoice;
    do {
        cout << "Do you want to make the repository private or public?" << endl;
        cout << "Enter 1 for public, 0 for private: ";
        cin >> privacyChoice;
        if (privacyChoice == 0 || privacyChoice == 1) {
            break; // Valid choice, exit loop
        }
        else {
            cout << "Invalid choice. Please enter 1 for public or 0 for private." << endl;
        }
    } while (true);

    string password;
    if (privacyChoice == 0) {
        cout << "Enter the password for the private repository: ";
        cin >> password;
    }

    // Store the repository name in repositorynames.txt
    ofstream repoNamesFileAppend(repoFileName, ios::app);
    if (!repoNamesFileAppend.is_open()) {
        cerr << "Error: Unable to open " << repoFileName << " for writing." << endl;
        return;
    }
    repoNamesFileAppend << name << endl;
    repoNamesFileAppend.close();

    // Store the repository name and password (if private) in privaterepository.txt
    if (privacyChoice == 0) {
        ofstream privateRepoFile(privateRepoFileName, ios::app);
        if (!privateRepoFile.is_open()) {
            cerr << "Error: Unable to open " << privateRepoFileName << " for writing." << endl;
            return;
        }
        privateRepoFile << name << "," << password << endl;
        privateRepoFile.close();
    }

    // Store the repository at the current index and increment currentIndex
    repositories[currentIndex] = new AVLTree(); // Assuming this line was here in your original code
    cout << "Repository '" << name << "' created at index: " << currentIndex << endl;
    currentIndex++;
}

void Repository::addFileToRepository(int index, const string& projectName, int hashVal) {
    if (index < 0 || index >= 100) {
        cout << "Invalid repository index." << endl;
        return;
    }

    // Construct repository file name based on the hash value
    string repoFileName = "repositoryname" + to_string(hashVal) + ".txt";

    // Open repository file to find the repository name for the given index
    ifstream repoNameFile(repoFileName);
    string repoName;
    if (repoNameFile.is_open()) {
        for (int i = 0; i <= index; ++i) {
            getline(repoNameFile, repoName);
        }
        repoNameFile.close();
    }
    else {
        cerr << "Error: Unable to open " << repoFileName << "." << endl;
        return;
    }

    // Construct repository file name with the repository name
    string repositoryFileName = repoName + ".txt";

    // Open the repository file for the specified repository name
    ofstream repositoryFile(repositoryFileName, ios::app);
    if (!repositoryFile.is_open()) {
        cout << "Error: Unable to open repository file for writing." << endl;
        return;
    }

    // Add the project name to the repository file
    repositoryFile << projectName << endl;
    repositoryFile.close();

    // Create a file with the project name and ask the user to input the content
    ofstream projectFile(projectName + ".txt");
    if (projectFile.is_open()) {
        cout << "Enter the content of the file for project " << projectName << ":" << endl;
        cin.ignore(); // Ignore any newline characters in the input buffer
        string content;
        getline(cin, content); // Get the content from the user
        projectFile << content; // Write the content to the file
        projectFile.close();
        cout << "File added to repository '" << repoName << "' at index: " << index << endl;
    }
    else {
        cout << "Error: Unable to create project file." << endl;
    }
}



void Repository::deleteFileFromRepository(int index, const string& value, int hashVal) {
    if (index < 0 || index >= 100 || repositories[index] == nullptr) {
        cout << "Invalid repository index." << endl;
        return;
    }

    // Construct repository file name based on the hash value
    string repoFileName = "repositoryname" + to_string(hashVal) + ".txt";

    // Open repository file to find the repository name for the given index
    ifstream repoNameFile(repoFileName);
    string repoName;
    if (repoNameFile.is_open()) {
        for (int i = 0; i <= index; ++i) {
            getline(repoNameFile, repoName);
        }
        repoNameFile.close();
    }
    else {
        cerr << "Error: Unable to open " << repoFileName << "." << endl;
        return;
    }

    // Open the repository file for the specified repository name
    string repositoryFileName = repoName + ".txt";
    ifstream inputFile(repositoryFileName);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open repository file for reading." << endl;
        return;
    }

    // Create a temporary file to store the updated repository content
    ofstream tempFile("temp.txt");
    if (!tempFile.is_open()) {
        cerr << "Error: Unable to create temporary file." << endl;
        inputFile.close();
        return;
    }

    // Search for the project name in the repository file
    string line;
    bool projectFound = false;
    while (getline(inputFile, line)) {
        if (line == value) {
            // Found the project, delete its corresponding file
            if (remove((value + ".txt").c_str()) != 0) {
                cerr << "Error: Unable to delete project file." << endl;
            }
            else {
                cout << "Project file '" << value << ".txt' deleted successfully." << endl;
            }
            projectFound = true;
        }
        else {
            // Write non-matching lines to the temporary file
            tempFile << line << endl;
        }
    }

    inputFile.close();
    tempFile.close();

    // Replace the original repository file with the temporary file
    if (projectFound) {
        if (remove(repositoryFileName.c_str()) != 0) {
            cerr << "Error: Unable to delete original repository file." << endl;
        }
        else {
            if (rename("temp.txt", repositoryFileName.c_str()) != 0) {
                cerr << "Error: Unable to rename temporary file." << endl;
            }
            else {
                cout << "Project removed from repository at index: " << index << endl;
            }
        }
    }
    else {
        cout << "Project '" << value << "' not found in repository at index: " << index << endl;
    }
}



string Repository::getLineFromFile(const string& fileName, int lineNum) {
    ifstream file(fileName);
    string line;
    for (int i = 0; i < lineNum; ++i) {
        if (!getline(file, line)) {
            return ""; // Return empty string if lineNum exceeds the number of lines in the file
        }
    }
    return line;
}

void Repository::displayRepository(int index, int hashVal) {
    if (index < 0 || index >= 100) {
        cout << "Invalid repository index." << endl;
        return;
    }

    // Construct repository file name based on the hash value
    string repoFileName = "repositoryname" + to_string(hashVal) + ".txt";

    // Open the repository file to find the repository name for the given index
    ifstream repoNameFile(repoFileName);
    string repoName;
    if (repoNameFile.is_open()) {
        for (int i = 0; i <= index; ++i) {
            getline(repoNameFile, repoName);
        }
        repoNameFile.close();
    }
    else {
        cerr << "Error: Unable to open " << repoFileName << "." << endl;
        return;
    }

    // Construct privaterepository file name based on the hash value
    string privateRepoFileName = "privaterepository" + to_string(hashVal) + ".txt";

    // Open privaterepository file to check if the repository is private
    ifstream privateRepoFile(privateRepoFileName);
    string line;
    bool isPrivate = false;
    string password;
    if (privateRepoFile.is_open()) {
        while (getline(privateRepoFile, line)) {
            size_t commaIndex = line.find(',');
            string repo = line.substr(0, commaIndex);
            if (repo == repoName) {
                isPrivate = true;
                password = line.substr(commaIndex + 1);
                break;
            }
        }
        privateRepoFile.close();
    }
    else {
        cerr << "Error: Unable to open " << privateRepoFileName << "." << endl;
        return;
    }

    // Ask for password if the repository is private
    if (isPrivate) {
        string inputPassword;
        cout << "Enter the password for the private repository '" << repoName << "': ";
        cin >> inputPassword;
        if (inputPassword != password) {
            cout << "Incorrect password. Access denied." << endl;
            return;
        }
    }

    // Open the repository file for the specified repository name
    ifstream repositoryFile(repoName + ".txt");
    if (!repositoryFile.is_open()) {
        cerr << "Error: Unable to open repository file for reading." << endl;
        return;
    }

    // Display contents of the repository file
    cout << "Contents of repository '" << repoName << "' at index " << index << ":" << endl;
    string projectName;
    int projectIndex = 0;
    while (getline(repositoryFile, projectName)) {
        cout << "Project " << ++projectIndex << ": " << projectName << endl;
    }
    repositoryFile.close();

    // Ask the user which project's contents they want to see
    int projectNumber;
    do {
        cout << "Enter the project number you want to display (0 to return to the main menu): ";
        cin >> projectNumber;
        if (projectNumber == 0) {
            return; // Return to main menu
        }
        else if (projectNumber < 1 || projectNumber > projectIndex) {
            cerr << "Invalid project number. Please enter a valid project number." << endl;
        }
        else {
            // Open and display contents of the selected project file
            ifstream projectFile(repoName + ".txt");
            if (projectFile.is_open()) {
                string projectLine;
                for (int i = 0; i < projectNumber; ++i) {
                    getline(projectFile, projectLine);
                }
                cout << "Contents of project '" << projectLine << "':" << endl;
                projectFile.close();
                ifstream projectContentFile(projectLine + ".txt");
                if (projectContentFile.is_open()) {
                    string line;
                    while (getline(projectContentFile, line)) {
                        cout << line << endl;
                    }
                    projectContentFile.close();
                    return; // Exit function after displaying project contents
                }
                else {
                    cerr << "Error: Unable to open project file for reading." << endl;
                    return;
                }
            }
            else {
                cerr << "Error: Unable to open repository file for reading." << endl;
                return;
            }
        }
    } while (true); // Repeat until a valid project number is entered or the user chooses to return to the main menu
}


void Repository::setRepositoryName(const string& name) {
    repositoryName = name;
}

string Repository::getRepositoryName(int index) {
    if (index < 0 || index >= 100 || repositories[index] == nullptr) {
        cout << "Invalid repository index." << endl;
        return "";
    }
    // Return the repository name directly from the Repository class
    return repositoryName;
}