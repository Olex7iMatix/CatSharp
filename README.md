# CatSharp

CatSharp (cat#, cats) is a programming language based on JS, C# etc

# Installation
## Things You need:
* Makefile
* gcc
## Compile:
Download repo from **https://github.com/** and unzip it to your folder. Then open directory with Command Prompt and type ```make```.

# Examples
This application will print in Console message "Hello CatSharp!"
```cat#
pack Example {
  # import statements can have ";" but don't have to
  imp Console from CatSharp
  class Main {
    var name = "CatSharp";
    void start() {
      Console.log("Hello " + name + "!");
    }
  }
}
```
# Docs
To start coding in Cat# you need to know how the base file look like. At first you need to have:
```cat#
pack <packageName> {

}
```
In package you need to declare minimum 1 class with *public* **void** called *start*. That should look like this:
```cat#
class <Class> {
  void start() {

  }
}
```
this is requirenments of .cat# application
