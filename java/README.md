#javac

`javac`用于编译`java`源文件，生成`.class`文件。

语法：`javac  [option]  source`

常用的`[option]`选项：

- `-classpath` <路径>（`-cp`缩写）：指定要使用的类路径或`jar`包路径，使用后会覆盖`CLASSPATH`的设定。

- `-d` <路径> ：指定源文件编译后生成`.class`文件的存放位置。


#java

`java`用于运行`javac`编译后生成的`.class`文件。

语法：`java   [option]  classname  [arguments]`

常用的`[option]`选项：

- `-classpath` <路径>（`-cp`缩写）：指定要使用的类路径或`jar`包路径，使用后会覆盖`CLASSPATH`的设定。

- `[arguments]`：传递给`main`函数的参数。