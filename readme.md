Programming Factory Recipes
=============================

编程工厂 (Programming Factory) is quite funny game available on [Steam](https://store.steampowered.com/app/2340280/_/). 

Programming Factory Recipes is going to  
✅ read available value  
✅ read available operation  
✅ read needed value  
✅ show shorted recipe of needed value  
🟩 show whole recipe of needed value  
🟩 read available values  
🟩 read available operations  
🟩 read needed values  
🟩 remove value  
🟩 remove operation  
🟩 remove all values  
🟩 remove all operations  

Interface
---------
Program reads line and execute it. In one line can be one command. Command name is separated with white spaces from argument, there are only 2 command names `add` and `find`. Argument can be identifier, string or number. Eg.:
- there is a number 1 on map
```
add 1
```
- there is a string 1 on map
```
add "1"
```
- there is a string "one" on map
```
add "one"
```
or
```
add one
```
- you can place a reminder
```
add reminder
```
- you want a recipe for a string "o.o"
```
find "o.o"
```

### Commands
command names are case sensitive.

#### Command `add`
`add` allows to specify what values and operations are available on map. Names of operations were taken from workshop map files, i.e.:
- `add`
- `reminder`
- `square`
- `cutter`
- `index`
- `ASCII`
Names are case sensitive.
If you doesn't add any operation, program suppose you mean all operations.

#### Command `find`
`find` starts searching recipe for given argument. If it finds any writes it in format `expected: operation values`, where `expected` is argument passed to `find`, `operation` is short representation for operations added by `add` and `values` is list space separated of values passed to operation.  
operation | short representation
----------|---------------------
`add`     | `+`
`reminder`| `%`
`square`  | `■`
`cutter`  | `[]`
`index`   | `─O`
`ASCII`   | `ASCII`

### tokens
#### Number
Number is sequence of digits,`-`(for negative values),`.`(for separation decimal part),`e`(for separation exponent). `INF`,`INFINITY` case insensitive represents infinity value and is number not identifier. `-INF`,`-INFINITY` case insensitive represents negative infinity value. `NAN` case insensitive represents not a number value.
#### String
String is character sequence between two `'` or `"`. In order to escape `'` in `'` limited string use `\`, the same for `"` in `"` limited string.

#### Identifier
Identifier is sequence of nonwhite characters not fitting in preceding categories.  
`find` command always treats identifier as string, but `add` differ identifier from string if it represents operation.