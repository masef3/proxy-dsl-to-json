# proxy-dsl-to-json

A simple custom Domain-specific Language compiler written in C++ that parses reverse-proxy configurations and generates
JSON representation.

## Motivation

This was created as for-fun project for me to test my C++ and compiler knowledge. I have been trying to understand compilers and its implementation for some time,
and this is a try to finally make one.

## Current Limitations
* **Basic Error Handling:** If you miss a bracket or misspell a keyword, the parser will catch it, but it might not give you the most elegant line-number error messages yet.
* **Strict Grammar:** The parser expects the exact structure shown below and does not gracefully recover from weird edge-case formatting.


## Example

**TJS FILE**

```
program {
    port = 8080
    rules {
        case "/login" {
            split  "http://localhost:3000" = 80,
                   "http://localhost:3001" = 20

        }

        case "/home" {
            target "http://localhost:3002"
        }

        case "/api" {
            block "http://localhost:3003"
                  "http://localhost:3004"
        }
    }
}
```

**JSON OUTPUT**
```
{
  "port": 8080,
  "rules": [
    {
      "case": "/login",
      "function": "split",
      "targets": [
        {
          "possibility": 80,
          "url": "http://localhost:3000"
        },
        {
          "possibility": 20,
          "url": "http://localhost:3001"
        }
      ]
    },
    {
      "case": "/home",
      "function": "target",
      "targets": "http://localhost:3002"
    },
    {
      "case": "/api",
      "function": "block",
      "targets": [
        "http://localhost:3003",
        "http://localhost:3004"
      ]
    }
  ]
}
```

## Build instructions

### Prerequisites
* C++23 compatible compiler
* cmake (3.20 or higher)
* nlohmann/json library


## Install

Using given script should be enough:

```
./install.sh
```


