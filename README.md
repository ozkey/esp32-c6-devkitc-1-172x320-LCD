## VS CODE for esp32-c6 with a 172x320 lcd screen

PlatformIO plugin install and enable on workspace

## IntelliJ as an editor for PlatformIO

Use the package.json to run the PlatformIO commands from the terminal,
as there is no PlatformIO plugin for IntelliJ, only for Clion.
 
🚀 Why this approach is actually pretty nice
Portable — anyone cloning your repo gets the same commands
Simple — no IntelliJ plugin magic, just scripts
Flexible — you can add custom tasks (clean, test, open serial, etc.)
Works across IDEs — IntelliJ, WebStorm, VS Code, etc.
It’s not the “intended” PlatformIO workflow, but it’s absolutely functional.
