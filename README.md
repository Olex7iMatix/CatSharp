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
# CatSharp packages

## CatSharp
This is an official package from CatSharp! If you don't want to copy code or download older/newer versions feel free to use CatLib
### CatSharp.cats
```cats
pack CatSharp {
    class Console {
        
        void Log(msg) {
            log("[CatSharp]", msg);
        };

        void LogInfo(msg) {
            log("[INFO]", msg);
        };

        void LogError(msg) {
            log("[ERROR]", msg);
        }

    };
};
```
### libs.toml
```toml
[package]
name = "CatSharp"
version = "0.0.0.1"
author = "Olex7iMatix"

[modules]
```
# Credits

* tomlc99
* Ianertson