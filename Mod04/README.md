Para a compilação do código, utilizar o comando 
`gcc -o experiment fs_experiment.c -lm`

No arquivo fs_experiment.c há 4 constants
```
#define PATH_NTFS "/media/wallacemanzano/NTFS"
#define PATH_FAT  "/media/wallacemanzano/FAT"
#define PATH_EXT4 "/media/wallacemanzano/EXT"

#define REP 10
```
A constante `PATH_NTFS` contem o caminho para a paratição NTFS,  constante `PATH_FAT` para a partição FAT e a `PATH_EXT4` para a partição ext4. A constante `REP` contem a quantidade de repetições que o experimento irá executar.
