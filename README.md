# CatSharp

CatSharp (cat#, cats) is a programming language based on JS, C# etc

# Support

CatSharp Support Server (Discord): https://discord.io/catsharp

# Instalation
## You need:
* Makefile
* gcc
## Compile:
Download repo from **https://github.com/** and unzip it to your folder. Then open directory with Command Prompt and type ```make```.

# Examples
## Hello World
### main.cats
```cats
pack Example {
  class HelloWorld {
    imp CatSharp;

    LogInfo("Hello World!");
  };
};
```
### libs.toml
```toml
[package]
name = "Examples"
author = "CatSharp"
version = "1.0.0"

[modules]
CatSharp = "<Your CatSharp path>"
```
# Credits

* tomlc99
* Ianertson