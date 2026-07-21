# Build instructions

* cmake

## Windows

* VisualStudio >= 2017

## Linux

```
# debian/ubuntu
sudo apt install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev 
# fedora/rhel
sudo dnf install -y gcc-c++ @development-tools cmake SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel
```

## Build

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
