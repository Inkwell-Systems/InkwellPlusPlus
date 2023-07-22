![inkwellBanner](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/1af85195-624d-4294-8182-dd2553df7e85)

# ⭐ State of the API
The Inkwell API is in a functional state. It can parse, store, modify and write an Inkwell JSON.

# ⏭️ Future plans
In the future, custom entries should be introduced in the Inkwell interface.
Also, scopes will be made functional.

# 📃 Documentation. How to utilize the API
The following section will introduce the basics regarding InkwellPlusPlus.

## 🏗️ I. Structure

The main unit of the Inkwell API is the `Project` , which stores `Tables` .

The Tables contain `Events, Facts and Rules` respectively.

The API uses `shared_ptr's` of these classes.

```cpp
std::shared_ptr<Project> myProject = std::make_shared<Project>;
```

## 📚 II. Parsing / Deserializing the Inkwell file

The file can be read from any type of input stream.

As an example, in the image below, a file stream is opened and the `>> (extraction)` operator is utilized on the Project pointer.

```cpp
std::shared_ptr<Project> myProject = std::make_shared<Project>;

std::ifstream fileInput("myExampleFile.inkwell");

fileInput >> myProject;
```

_(Note that the file extension is not relevant in the parsing of the Project.)_

_The parsing of the JSON structure itself is done utilizing the following library:
https://github.com/nlohmann/json_

## 🌐 III. Project, Table, Entry

The Project object contains the Tables within the Inkwell file, as well as any extra information included in the file.

A Table is a container that holds Entries.
Tables are accessed using the `T(std::string key)` or `T(int id)` Project method.

```cpp
myProject->T("Sample Table");    // returns pointer to Table with Key equivalent to that parameter
myProject->T(42);                // returns pointer to Table with equivalent ID
```

An Entry is an abstract class.
It has the following fields and methods:

```cpp
protected:
bool initialized;
int id;
std::string key;
int guard;

public:
int ID();             // Gets the ID of an Entry (or Table)

std::string KEY();    // Gets the Key of an Entry (or Table)

int GRD();            // Gets an Entry's Guard. Currently redundant
```

There are three types of Entries:

### 1. Event
   
   Events have the role of triggering other Entries.
   Events are accessed using the `E(std::string key)` or `E(int id)` Table method.

   ```cpp
   myProject->T("Sample Table")->E("My event");    // pointer to Event with equivalent Key
   myProject->T("Sample Table")->E(12);            // pointer to Event with equivalent ID
   ```
   
### 2. Fact
   
  Facts have the role of storing information.
  Facts are accessed using the `F(std::string key)` or `F(int id)` Table method.

  ```cpp
  myProject->T("Sample Table")->F("My fact");
  myProject->T("Sample Table")->F(0);
  ```

### 3. Rule

   Rules have the role of dispatching callbacks, modifying and triggering other Entries.
   Rules are accessed using the `R(std::string key)` or `R(int id)` Table method.

  ```cpp
  myProject->R("Sample Table")->R("My rule");
  myProject->R("Sample Table")->R(10);
  ```

## 📝 IV. Serialization

Serialization of the Project can be done via the `<< (insertion)` operator.

As with the extraction operator, it can be used with any type of output stream.

```cpp
std::ofstream fileOutput("myOutput.inkwell");

fileOutput << myProject;
```

## ✍️ V. Json Structure

The input file is structured as such:

```
{
    // This is extra information used by the Inkwell website. 
    // It can be discarded during the parsing by using Project flags.

    cloud
    entryMap
    inviteCode
    owner
    projectBanner
    projectDescription
    projectId
    projectName
    projectCreatedAt
    members

    // Table scopes, Currently redundant

    scopes
    [
        {
            id
            key
            level
        }
    ]

    // The tables of the project

    tables
    [
        {
            id
            key
            events
            [
                {
                    id
                    key
                    value
                    eventGuard // Currently redundant
                    triggers
                }
            ]
            facts
            [
                {
                    id
                    key
                    value
                    factGuard // Currently redundant
                }
            ]
            rules
            [
                {
                    id
                    key
                    value
                    ruleGuard // currently redundant
                    criteria
                    [
                        {
                            comparedEntry
                            compareValue
                            comparisonOperator
                        }
                    ]
                    modifications
                    [
                        {
                            modifiedEntry
                            modifyWithValue
                            modificationOperator
                        }
                    ]
                }
            ]
        }
    ]
}
```

