![inkwellBanner](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/1af85195-624d-4294-8182-dd2553df7e85)

# State of the API
The Inkwell API is in a functional state. It can parse, store, modify and write an Inkwell JSON.

# Future plans
In the future, custom entries should be introduced in the Inkwell interface.

# Documentation. How to utilize the API

## I. Structure

The main unit of the Inkwell API is the ***Project,*** which stores ***Tables.***

The Tables contain ***Events, Facts and Rules*** respectively.

The API uses ***shared_ptr's*** of these classes.

![image](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/a4acc71e-c8e1-433d-92a4-5d83e8b92d75)

## II. Parsing / Deserializing the Inkwell file

The file can be read from any type of input stream.

As an example, in the image below, a file stream is opened and ***the >> (extraction) operator is utilized on the Project pointer.***

![image](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/72e1fc6a-ddcd-44a9-9a49-98455d31245a)

_(Note that the file extension is not relevant in the parsing of the Project.)_

_The parsing of the JSON structure itself is done utilizing the following library:
https://github.com/nlohmann/json_

## III. Project, Table, Entry

***The Project object contains the Tables*** within the Inkwell file, ***as well as any extra information*** included in the file.

A Table is a ***container that holds Entries.***

An Entry represents a ***functional component of the API,*** which has its own value, trigger other entries or dispatch callbacks, depending on the Entry type.

## IV. Triggering entries

This diagram shows what happens when an event is triggered:

![image](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/7977290b-04d8-48a3-9e94-d26e4347e0c2)

_Events and rules can be triggered. Facts do not have this functionality._

## V. Serialization

Serialization of the Project can be done ***via the << (insertion) operator.***

As with the extraction operator, it can be used with any type of output stream.

![image](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/0a898e5f-403e-495d-9c92-6bf70f16d88d)






