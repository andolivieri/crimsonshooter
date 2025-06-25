# Env

* VisualStudio >= 2017
* cmake

# Build

```
cd build
cmake ..
cmake --build .
```

# Misc

launch.json for vscode
```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "crimsonshooter",
            "type": "cppvsdbg",
            "request": "launch",
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "program": "build/dist/Debug/crimsonshooter.exe"
        }
    ]
}
```
# Links
https://www.youtube.com/watch?v=YrWQsuDT3NE&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx&index=4
