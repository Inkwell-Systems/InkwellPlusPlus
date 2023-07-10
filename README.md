# InkwellPlusPlus
A C++ API for the Inkwell project.

# State of the API
I am still working on it. Currently, it can do the following:
- Deserialize and Serialize an Inkwell JSON
- Utilize Facts, Events and Rules, stored in a Table, which itself is present within a Project
- Access, read and modify entries
- Trigger entries and dispatch callbacks

# Future plans
- Add custom entries
- Clean up code



# How to use



# I. Structure

Inkwell uses the following class structure:

- Project
  - Table
    - Event
    - Fact
    - Rule
      - Criterion
      - Modification
     
The API uses shared pointers of these classes, which are defined as <class-name> + "Ref".

![image](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/7d10dd47-7d23-43fe-9f5c-8f32360272fb)

# II. Deserialization

The Deserializer class facilitates the parsing of an Inkwell JSON file.
A Deserializer object may be constructed with the path to the JSON as the constructor argument.

![image](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/5bed300a-07f1-4d83-a500-85a48bac6444)

To parse the Project present within a JSON, one must call the parseProject method of the Deserializer object as such:

![image](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/d26a51aa-cb1f-46ed-9378-9c47ca6f39eb)

Alternatively, this can be done in one line:

![image](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/f2f6648c-fc50-4b2f-ab14-bd74661b41a3)

Once parsed, the Project is now structured internally and its fields may be accessed.
The deserializer expects a *correct* JSON.

# III. Project

The Project object has the following fields:

- Project
  - name
  - description
  - createdAtNano
  - tables (name-table map)
    - id
    - key
    - idToKey (map)
    - events (name-event map)
      - id
      - key
      - value
      - triggers (what rules it triggers upon event activation)
    - facts (name-fact map)
      - id
      - key
      - value
    - rules (name-rule map)
      - id
      - key
      - value
      - triggers (a vector of entries triggered upon rule activation)
      - criteria (to verify for the rule to activate)
        - comparedEntry
        - compareValue
        - comparisonOperator
      - modifications
        - modifiedEntry
        - modificationOperator
        - modifyWithValue
      - callbacks

As the project is structured using shared pointers, the -> operator must be used for field access.

Example of accessing a project's field:

![image](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/119c75c3-5126-4377-aee5-e9ce351da2d1)

In this example, a callback (in the form of a lambda) is being added to a Rule named "myRule". 
The rule is contained within the "myTable" table, and the "myTable" table is part of the project object.

# IV. Triggering entries

This diagram shows what happens when an event is triggered:

![image](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/7977290b-04d8-48a3-9e94-d26e4347e0c2)

Events and rules can be triggered. Facts do not have this functionality.

# V. Serialization

The Serializer object has the purpose of formatting the Project object and its contents into a JSON text file.
A Serializer object may be constructed with the path to the output file as its argument.

![image](https://github.com/Inkwell-Systems/InkwellPlusPlus/assets/111175088/21ef3ae3-90fa-45bf-a23c-71ffcc7c043e)





