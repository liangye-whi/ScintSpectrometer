基于GEANT4的闪烁体能谱仪模拟程序
================================

## 依赖
编译基于geant4-v11.0.1，CERN ROOT 6.28。

## 编译
```bash
$ cd ScintSpectrometer
$ mkdir build
$ cd build
$ make 
```

## 运行
加载比色皿和探测器几何，并可视化运行：
``` bash
$ cd build
$ ./OpNovice -g ../gdml/ScintDev.gdml
```

## 输出
``` bash
$ cd build
$ ./OpNovice -g ../gdml/ScintDev.gdml -o output.root
```
