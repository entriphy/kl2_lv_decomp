# Klonoa 2: Lunatea's Veil (PS2) Decompilation
Decompilation project for Klonoa 2: Lunatea's Veil (`SLUS_201.51`).

Structs, classes, and function names are taken from decompiling the C# assembly of Klonoa Phantasy Reverie Series. 
While this does not aim to be a fully matching decompilation, `ee-gcc 2.9-991111` (available on [decomp.me](https://decomp.me)) seems to produce matching code most of the time, though it is not 100% certain if this was the compiler used to compile the game.

Support the series by purchasing & playing [Klonoa Phantasy Reverie Series](https://klonoa-at-blog.tumblr.com/post/145593146333/here-are-the-current-ways-you-can-officially)! :D

## Disclaimer
This repository does not contain any game assets, thus a legal copy of the game is required for this decompilation to work. Do not request for these files to be provided.

## Building
**NOTE: This project is in VERY early stages and may fail to build.**

1. Setup environment:
    * Set the `SCESDK` environment variable to the root of the official PS2 SDK (usually `/usr/local/sce`). Download links to the SDK will not be provided in this repository.
2. Clone this repository:
    ```bash
    git clone https://github.com/entriphy/kl2_lv_decomp.git
    cd kl2_lv_decomp
    ```
3. Build the ELF file:
    ```bash
    cmake -Bbuild
    cd build
    make
    ```
4. The resulting ELF file will be named `kl2_lv_decomp.elf`. You can launch it with PCSX2 using the following command:
    ```bash
    pcsx2 -elf kl2_lv_decomp.elf <disc_image_file>
    ```

## Contributing
(todo)

[Decompilation Progress (Google Sheets)](https://docs.google.com/spreadsheets/d/190e6yPZklUfM2ye2Pkpta4B5KNp9lJFB0Ldx90eCVzU/edit?usp=sharing)

## Other Tools
* [kl2_pine](https://github.com/entriphy/kl2_pine): Memory editor/visualizer for Klonoa 2 using [PINE](https://github.com/GovanifY/pine)
* [BinarySerializer.Klonoa](https://github.com/BinarySerializer/BinarySerializer.Klonoa): Serializes various file formats from Klonoa 2 (in addition to other Klonoa games)
* [klonoa2-tools](https://github.com/entriphy/klonoa2-tools): KLDATA unpacker and model extractor
* [spimdisasm](https://github.com/Decompollaborate/spimdisasm): MIPS disassembler
* [m2c](https://github.com/matt-kempster/m2c): MIPS decompiler
* [ghidra-emotionengine-reloaded](https://github.com/chaoticgd/ghidra-emotionengine-reloaded): Ghidra extension for Playstation 2 executables
