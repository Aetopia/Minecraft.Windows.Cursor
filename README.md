# Minecraft for Windows Cursor
Fixes wonky cursor issues with Minecraft: Bedrock Edition (GDK).

## Usage
- Locate the game's installation drive & folder.
- Download the latest release from [GitHub Releases](https://github.com/Aetopia/Minecraft.Windows.Cursor/releases/latest).
- Place the dynamic link library in the game's installation folder.

## Build
1. Install & update [MSYS2](https://www.msys2.org):

    ```bash
    pacman -Syu --noconfirm
    ```

3. Install [GCC](https://gcc.gnu.org) & [MinHook](https://github.com/TsudaKageyu/minhook):

    ```bash
    pacman -Syu mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-MinHook --noconfirm
    ```

3. Start MSYS2's `UCRT64` environment & run `Build.cmd`.