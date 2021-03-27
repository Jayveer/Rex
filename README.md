
# Rex


Rex is a free open source tool designed to be used with the game Metal Gear Solid on the original Playstation. It allows the user to extract Dir and Dar archive files packaged with the game

### To Do
 - Add multithreaded extract
 - Add error handling
 - Create GUI variant
 - Clean up the code

##  Usage

Currently only a CLI version of the application exists. The program is primitive and just takes the filename as the only required argument. An optional output directory can also be added. It is also possible to just drag the file you wish to extract on the executable.

```
Rex.exe "path\to\stage.dir"
```
The above instruction will extract all files from stage.dir to the current directory.

```
Rex.exe "path\to\res_mdl0.dar" 
```
The same can be used for Qar, Dar or Slot files.

```
Rex.exe "path\to\stg_tex1.dar" "path\to\output"
```
An optional output path can be added at the end, if it is not included then it will extract to the directory of the file being extracted.

## License
[MIT](LICENSE.md)
This project falls under the MIT license.