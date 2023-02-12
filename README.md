# Klonoa 2: Lunatea's Veil (PS2) Decompilation
Decompilation project for Klonoa 2: Lunatea's Veil (`SLUS_201.51`).

Structs, function names, and variable names are taken from decompiling the C# assembly of Klonoa Phantasy Reverie Series. Decompilation is done using [Ghidra](https://ghidra-sre.org/) with the [ghidra-emotionengine](https://github.com/chaoticgd/ghidra-emotionengine-reloaded) extension. This is **not** a matching decompilation, as there is no publicly available compiler to do so.

Support the series by purchasing & playing [Klonoa Phantasy Reverie Series](https://klonoa-at-blog.tumblr.com/post/145593146333/here-are-the-current-ways-you-can-officially)! :D

## Disclaimer
This repository does not contain any game assets, thus a legal copy of the game is required for this decompilation to work. Do not request for these files to be provided.

## Building
1. Setup environment:
    * ~~Ensure the `PS2DEV` and `PS2SDK` environment variables are properly set.~~
        * PS2SDK is currently unsupported.
    * You can also set the `SCESDK` environment variable to the root of the official PS2 SDK to use that instead.
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

## Other Tools
* [kl2_pine](https://github.com/entriphy/kl2_pine): Memory editor/visualizer for Klonoa 2 using [PINE](https://github.com/GovanifY/pine)
* [BinarySerializer.Klonoa](https://github.com/BinarySerializer/BinarySerializer.Klonoa): Serializes various file formats from Klonoa 2 (in addition to other Klonoa games)
* [klonoa2_tools](https://github.com/entriphy/klonoa2-tools): KLDATA unpacker and model extractor
