# Klonoa 2: Lunatea's Veil (PS2) """Decompilation"""
Decompilation project for Klonoa 2: Lunatea's Veil (`SLUS_201.51`) using [PS2SDK](https://github.com/ps2dev/ps2sdk). 

Structs, function names, and variable names are taken from decompiling the C# assembly from the Steam version of Klonoa Phantasy Reverie Series. The game's IOP module (`KL2.IRX`) exports symbols, which are used to name functions and global variables. Decompilation is done using [Ghidra](https://ghidra-sre.org/) with the [ghidra-emotionengine](https://github.com/chaoticgd/ghidra-emotionengine-reloaded) extension. This is **not** a matching decompilation, as there is no publicly available compiler to create a matching decompilation.

Support the series by purchasing & playing [Klonoa Phantasy Reverie Series](https://klonoa-at-blog.tumblr.com/post/145593146333/here-are-the-current-ways-you-can-officially)! :D

## Disclaimer
This repository does not contain any game assets, thus a legal copy of the game is required for this decompilation to work. Do not request for these files to be provided.

## Building
1. Setup PS2DEV environment.
    * Ensure the `PS2DEV` and `PS2SDK` environment variables are properly set.
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
* For known structs/functions/variables, simply use the original name (ex. `hCDDATA`/`hInitBoot`/`CdData`).
* For unknown structs, give it a name and append `__unk` to the end (ex. `hAC3DATA__unk`). All unknown struct fields must follow the format `field_0x%x`, where `%x` is the offset of the field in hex.
* For unknown functions/variables, name it with a rough guess/description of what the function does, along with the address of the function/variable (ex. `hReadFile__00165ae0`/`RpcRecvBuf__00417200`) or  use the name that Ghidra gives it (ex. `FUN_00165ae0`/`DAT_00417200`).
* lowerCamelCase is preferred when naming unknown values, though other naming conventions may be okay in certain areas given that the known function/variable names do not follow a common naming convention.

## Other Tools
* [kl2_pine](https://github.com/entriphy/kl2_pine): Memory editor/visualizer for Klonoa 2 using [PINE](https://github.com/GovanifY/pine)
* [BinarySerializer.Klonoa](https://github.com/BinarySerializer/BinarySerializer.Klonoa): Serializes various file formats from Klonoa 2 (in addition to other Klonoa games)
* [klonoa2_tools](https://github.com/entriphy/klonoa2-tools): KLDATA unpacker and model extractor
